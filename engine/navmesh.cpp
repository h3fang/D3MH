#include "navmesh.h"

#include <cstdio>
#include <cmath>

#include <algorithm>

#include "process/memoryreader.h"
#include "process/pointer.h"

#include "engine.h"

namespace D3 {

std::unordered_map<DWORD, SceneSnoData> NavMesh::snoSceneIdAddrMap;

SceneSnoData::SceneSnoData() :
    sno_id(-1)
{
}

SceneSnoData::SceneSnoData(DWORD sno_ptr)
{
    AssetScene s = Pointer<AssetScene>()(sno_ptr);

    sno_id = s.header.x00_SnoId;

    NavMesh::getSerializedRecords(cells, s.NavZone.NavCells, sno_ptr);

    if (cells.empty()) {
        fprintf(stderr, "Got 0 serialized records for Sno Id [%u]\n", sno_id);
    }
    else {
        cells.erase(std::remove_if(cells.begin(), cells.end(), [](const NavCell& c){
            return (c.flag & (NavCellFlagW_AllowWalk/* | NavCellFlagDW_AllowFlier*/)) == 0;
        }), cells.end());
    }
}

SceneData::SceneData(const Scene &s)
{
    loadFromMemory(s);
}

void SceneData::loadFromMemory(const Scene &s)
{
    id = s.x000_Id;
    sno_id = s.x0E8_SceneSnoId;
    levelArea_sno_id = s.x018_LevelAreaSnoId;

    min.x = s.x0FC_MeshMinX;
    min.y = s.x100_MeshMinY;
    min.z = s.x104_MeshMinZ;

    max.x = s.x174_MeshMaxX;
    max.y = s.x178_MeshMaxY;
    max.z = s.x104_MeshMinZ; //there is no max z, so consider all grid cells flat

    SceneSnoData ss = NavMesh::snoSceneIdAddrMap[sno_id];

    if (ss.sno_id == -1) {
        fprintf(stderr, "No record for AssetScene id [%u] in NavMesh::snoSceneIdAddrMap\n", sno_id);
        return;
    }

    cells = ss.cells;
}

template<class T>
bool NavMesh::getSerializedRecords(std::vector<T> &out, DataPtr2 ptr, DWORD dwBase)
{
    if (!dwBase) {
        return false;
    }

    //Real data start
    DWORD dwDataStart = dwBase + ptr.file_offset;

    out.reserve(ptr.size/sizeof(T)+1);

    //Read records
    while ( dwDataStart < dwBase + ptr.file_offset + ptr.size )
    {
        T data;

        if (!MemoryReader::instance()->read(&data, (void*)dwDataStart, sizeof(T))) {
            fprintf(stderr, "Failed to read memory in NavMesh::getSerializedRecords()\n");
            out.clear();
            return false;
        }

        out.push_back(data);

        dwDataStart += sizeof(T);
    }

    return true;
}

NavMesh::NavMesh() :
    cleared(true)
{
}

void NavMesh::update()
{
    parseMemorySnoScene();

    int level_area_sno_id = Pointer<int>()(Addr_LevelArea,0x044);

    if(level_area_sno_id != last_level_area_sno_id){
        last_level_area_sno_id = level_area_sno_id;
        clearScene();
    }

    Container<Scene> c = Pointer<Container<Scene>>()(Addr_ObjectManager,offsetof(ObjectManager, x998_Scenes), 0);

    if (c.x108_MaxIndex < 0 || c.x11C_PtrItems == 0) {
        return;
    }

    for (int i = 0; i < c.x108_MaxIndex; ++i) {
        Scene s = Pointer<Scene>()(c.x11C_PtrItems+i*c.x104_ItemSize);

        if (s.x000_Id < 0) {
            continue;
        }

        SceneData* sd = sceneData[s.x0E8_SceneSnoId];
        if (sd == 0) {
            sceneData[s.x0E8_SceneSnoId] = new SceneData(s);
        }
        else {
            sd->loadFromMemory(s);
        }
    }

    cleared = false;
}

void NavMesh::clear()
{
    if (!cleared) {
        clearScene();
        cleared = true;
    }
}

void NavMesh::clearScene()
{
    for (auto it=sceneData.begin(); it!=sceneData.end(); ++it) {
        SceneData* sd = (*it).second;
        if(sd->levelArea_sno_id != last_level_area_sno_id){
            delete (*it).second;
            sceneData.erase(it);
        }
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
        SnoDefinition d = Pointer<SnoDefinition>()(c.x11C_PtrItems+i*c.x104_ItemSize);
        if(d.x00_Id == -1 || d.x07_SnoGroupId != (char)SnoGroupId_Scene){
            continue;
        }

        SceneSnoData s(d.x0C_pSNOAddr);
        snoSceneIdAddrMap[s.sno_id] = s;
    }
}

}
