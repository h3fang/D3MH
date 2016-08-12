#include "navmesh.h"

#include <cstdio>
#include <cmath>

#include <fstream>
#include <iostream>
#include <algorithm>

#include "process/memoryreader.h"
#include "process/pointer.h"

#include "engine.h"

namespace D3 {

std::unordered_map<DWORD, SceneSnoDataPtr> NavMesh::snoSceneIdAddrMap;

SceneSnoData::SceneSnoData() :
    sno_id(-1),
    cached(false)
{
}

SceneSnoData::SceneSnoData(int sno_id) :
    sno_id(-1)
{
    cached = load(sno_id);
}

SceneSnoData::SceneSnoData(AssetScene* sno_ptr) :
    sno_id(-1),
    cached(false)
{
    AssetScene s = Pointer<AssetScene>()(sno_ptr);

    sno_id = s.header.x00_SnoId;

    NavMesh::getSerializedRecords(cells, s.NavZone.NavCells, (DWORD)sno_ptr);

    if (cells.empty()) {
        fprintf(stderr, "Got 0 serialized records for Sno Id [%u]\n", sno_id);
    }
    else {
        cells.erase(std::remove_if(cells.begin(), cells.end(), [](const NavCell& c){
            return (c.flag & (NavCellFlagW_AllowWalk/* | NavCellFlagDW_AllowFlier*/)) == 0;
        }), cells.end());
    }
}

SceneSnoData::~SceneSnoData()
{
    if (!cached) {
        save();
    }
}

bool SceneSnoData::save()
{
    char path[128];
    snprintf(path, sizeof(path), "./cache/%d", sno_id);
    std::ofstream file(path, std::ofstream::binary | std::ofstream::trunc);
    file << cells.size();
    for (const NavCell &c : cells) {
        file.write((char *)&c, sizeof(c));
    }

    cached = file.good();
    return cached;
}

bool SceneSnoData::load(int sno_id)
{
    char path[128];
    snprintf(path, sizeof(path), "./cache/%d", sno_id);
    std::ifstream file(path, std::ifstream::binary);
    int size = 0;
    file >> size;

    if (!file.good() || size <=0 || size >1000) {
        return false;
    }

    cells.reserve(size);

    for (int i=0; i<size; ++i) {
        NavCell c;
        file.read((char *)&c, sizeof(c));

        if (file.fail() || file.bad()) {
            cells.clear();
            return false;
        }
    }

    cached = true;
    this->sno_id = sno_id;
    return true;
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

    sceneSnoDataPtr = NavMesh::snoSceneIdAddrMap[sno_id];

    if (!sceneSnoDataPtr) {
        SceneSnoDataPtr ss = std::make_shared<SceneSnoData>();
        if (ss->load(sno_id)) {
            NavMesh::snoSceneIdAddrMap[sno_id] = ss;
            sceneSnoDataPtr = ss;
        }
        else {
            fprintf(stderr, "Failed to load AssetScene for id [%u]\n", sno_id);
        }
    }
    else {
        fprintf(stderr, "No record for AssetScene id [%u] in NavMesh::snoSceneIdAddrMap\n", sno_id);
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

    int level_area_sno_id = Pointer<int>()(Addr_LevelArea, 0x44);

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

        SceneDataPtr sd = sceneData[s.x0E8_SceneSnoId];
        if (sd == 0) {
            sceneData[s.x0E8_SceneSnoId] = std::make_shared<SceneData>(s);
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
        SceneDataPtr sd = (*it).second;
        if(sd->levelArea_sno_id != last_level_area_sno_id){
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

        SceneSnoDataPtr s = std::make_shared<SceneSnoData>((AssetScene *)d.x0C_pSNOAddr);
        snoSceneIdAddrMap[s->sno_id] = s;
    }
}

}
