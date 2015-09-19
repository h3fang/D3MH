#include "navmesh.h"

#include <stdio.h>
#include <math.h>

#include "process/memoryreader.h"
#include "process/pointer.h"

#include "engine.h"

namespace D3 {

std::map<DWORD, DWORD> NavMesh::snoSceneIdAddrMap;

SceneData::SceneData(const Scene &s)
{
    id = s.x000_Id;
    sno_id = s.x0E8_SceneSnoId;
    area_sno_id = s.x018_LevelAreaSnoId;

    min.x = s.x0FC_MeshMinX;
    min.x = s.x100_MeshMinY;
    min.x = s.x104_MeshMinZ;

    max.x = s.x174_MeshMaxX;
    max.y = s.x178_MeshMaxY;
    max.z = s.x104_MeshMinZ; //there is no max z, so consider all grid cells flat
}

SceneSnoData::SceneSnoData(int sno_id) :
    id(sno_id)
{
    DWORD ss_addr = NavMesh::snoSceneIdAddrMap[sno_id];

    if (ss_addr == 0) {
        fprintf(stderr, "No record for SnoScene id [%d] in NavMesh::snoSceneIdAddrMap\n", sno_id);
    }

    SnoScene ss = Pointer<SnoScene>()(ss_addr);

    if (!NavMesh::getSerializedRecords(cells, ss.NavZone.NavCells, ss_addr)) {
        fprintf(stderr, "Failed to call NavMesh::getSerializedRecords()\n");
        return;
    }

    if (cells.empty()) {
        fprintf(stderr, "Empty result from NavMesh::getSerializedRecords()\n");
        return;
    }
    else {
        fprintf(stderr, "Got %d serialized records\n", cells.size());
    }
}

template<class T>
bool NavMesh::getSerializedRecords(std::vector<T> &out, DataPtr2 ptr, DWORD dwBase)
{
    if (!dwBase) {
        return false;
    }

    //Real data start
    DWORD dwDataStart = dwBase + ptr.file_offset;

    //Read records
    while ( dwDataStart < dwBase + ptr.file_offset + ptr.size )
    {
        T data;

        if (!MemoryReader::instance()->read(&data, (void*)dwDataStart, sizeof(T))) {
            out.clear();
            return false;
        }

        out.push_back(data);

        dwDataStart += sizeof(T);
    }

    return true;
}

void NavMesh::update()
{
    parseMemorySnoScene();

    sceneData.clear();
    sceneSnoData.clear();

    Container<Scene> c = Pointer<Container<Scene>>()(Addr_ObjectManager+offsetof(ObjectManager, x998_Scenes), 0);
    for (int i = 0; i < c.x108_MaxIndex; ++i) {
        Scene s = Pointer<Scene>()(c.x11C_PtrItems+i*c.x104_ItemSize);

        SceneData sd(s);
        SceneSnoData ssd(s.x0E8_SceneSnoId);

        sceneData.push_back(sd);
        sceneSnoData.push_back(ssd);
    }
}

void NavMesh::parseMemorySnoScene()
{
    Container<SnoDefinition> c = Pointer<Container<SnoDefinition>>()(Addr_SnoGroupByCode+sizeof(void*)*(int)(SnoGroupId_Scene),
                                                                     offsetof(SnoGroupManager, x10_Container),
                                                                     0);

    if (c.x108_MaxIndex < 0 || c.x11C_PtrItems == 0) {
        return;
    }

    for (int i = 0; i < c.x108_MaxIndex; ++i) {
        SnoDefinition d = Pointer<SnoDefinition>()((DWORD)c.x11C_PtrItems+i*c.x104_ItemSize);
        if(d.x00_Id == -1 || d.x07_SnoGroupId != (char)SnoGroupId_Scene){
            continue;
        }
        snoSceneIdAddrMap[Pointer<DWORD>()(d.pSNOAddr)] = d.pSNOAddr;
    }
}

}
