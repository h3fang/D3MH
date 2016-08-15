#include "navmesh.h"

#include <cstdio>
#include <cmath>

#include <fstream>
#include <algorithm>

#include <QDebug>
#include <QDir>

#include "process/memoryreader.h"
#include "process/pointer.h"

#include "engine.h"

namespace D3 {

std::unordered_map<uint, SceneSnoDataPtr> NavMesh::snoSceneIdAddrMap;

SceneSnoData::SceneSnoData() :
    sno_id(INVALID_SNO_ID),
    cached(false)
{
}

SceneSnoData::SceneSnoData(uint sno_id) :
    sno_id(INVALID_SNO_ID)
{
    cached = load(sno_id);
}

SceneSnoData::SceneSnoData(AssetScene* sno_ptr) :
    sno_id(INVALID_SNO_ID),
    cached(false)
{
    AssetScene s = Pointer<AssetScene>()(sno_ptr);

    sno_id = s.header.x00_SnoId;

    if (s.NavZone.NavCellCount <= 0 || s.NavZone.NavCells.size != sizeof(NavCell)*s.NavZone.NavCellCount) {
        return;
    }

    getSerializedData(cells, s.NavZone.NavCells, (uint)sno_ptr);

//    qDebug("Sno Id [%u] lockcount %d flags %d cell count %d\n", sno_id, s.header.x04_LockCount, s.header.x08_Flags, s.NavZone.NavCellCount);

    if (cells.empty()) {
        qDebug("Got 0 serialized records for Sno Id [%u] lockcount %d flags %d cell count %d\n", sno_id, s.header.x04_LockCount, s.header.x08_Flags, s.NavZone.NavCellCount);
    }
    else {
        cells.erase(std::remove_if(cells.begin(), cells.end(), [](const NavCell& c){
            return (c.flag & NavCellFlagW_AllowWalk) == 0;
        }), cells.end());
    }
}

SceneSnoData::~SceneSnoData()
{
}

bool SceneSnoData::save()
{
    if (sno_id == INVALID_SNO_ID || cells.empty()) {
        return false;
    }

    char path[128];
    snprintf(path, sizeof(path), "./cache/%u", sno_id);
    std::ofstream file(path, std::ofstream::binary | std::ofstream::trunc);
    file << cells.size();
    for (const NavCell &c : cells) {
        file.write((char *)&c, sizeof(c));
    }

    cached = file.good();
    return cached;
}

bool SceneSnoData::load(uint sno_id)
{
    char path[128];
    snprintf(path, sizeof(path), "./cache/%u", sno_id);
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

SceneData::SceneData() :
    id(INVALID_SNO_ID),
    sno_id(INVALID_SNO_ID),
    levelArea_sno_id(INVALID_SNO_ID),
    min(Vec3{0, 0, 0}),
    max(Vec3{0, 0, 0}),
    good(false)
{
}

SceneData::SceneData(const Scene &s)
{
    fromScene(s);
}

void SceneData::fetchCurrent(uint sno_id)
{
    this->sno_id = sno_id;
    // NOTE:offset
    levelArea_sno_id = Pointer<int>()(Addr_LevelArea, offsetof(LevelArea, x044_SnoId));

    good = findSceneSnoData();
}

void SceneData::fromScene(const Scene &s)
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

    good = findSceneSnoData();
}

bool SceneData::findSceneSnoData()
{
    if (NavMesh::snoSceneIdAddrMap.find(sno_id) == NavMesh::snoSceneIdAddrMap.end()) {
        qDebug("No record for AssetScene id [%u] in NavMesh::snoSceneIdAddrMap", sno_id);
        return false;
    }
    else {
        sceneSnoDataPtr = NavMesh::snoSceneIdAddrMap[sno_id];
        return true;
    }
}

NavMesh::NavMesh() :
    cleared(true)
{
    loadSceneSnoFiles();
}

NavMesh::~NavMesh()
{
    for (const auto& pair : snoSceneIdAddrMap) {
        pair.second->save();
    }
}

void NavMesh::loadSceneSnoFiles()
{
    QDir dir("./cache/");
    auto files = dir.entryList(QDir::Files|QDir::NoSymLinks, QDir::Name);
    for (QString s : files) {
        bool ok = false;
        uint sno_id = s.toUInt(&ok);
        if (ok) {
            SceneSnoDataPtr ss = std::make_shared<SceneSnoData>(sno_id);
            if (ss->cached) {
                snoSceneIdAddrMap[sno_id] = ss;
            }
        }
    }

    qDebug("loaded %u SceneSno files", snoSceneIdAddrMap.size());
}

void NavMesh::update()
{
    fetchSceneSno();
    fetchScene();
}

void NavMesh::clear()
{
    if (!cleared) {
        sceneData.clear();
        cleared = true;
    }
}

void NavMesh::fetchScene()
{
    // NOTE:offset
    uint level_area_sno_id = Pointer<uint>()(Addr_LevelArea, offsetof(LevelArea, x044_SnoId));

    if(level_area_sno_id != last_level_area_sno_id){
        last_level_area_sno_id = level_area_sno_id;
        clear();
    }

    // NOTE:offset
    Container<Scene> c = Pointer<Container<Scene>>()(Addr_ObjectManager, offsetof(ObjectManager, x998_Scenes), 0);

    for (const auto& s : enumerate_container(c)) {
        if (s.x000_Id == INVALID_SNO_ID || s.x018_LevelAreaSnoId != last_level_area_sno_id) {
            continue;
        }

        if (sceneData.find(s.x0E8_SceneSnoId) == sceneData.end()) {
            sceneData[s.x0E8_SceneSnoId] = std::make_shared<SceneData>(s);
        }
    }

    // NOTE:offset
    uint sno_id = Pointer<uint>()(Addr_LocalData+0x08);

    if (sceneData.find(sno_id) == sceneData.end()) {
        SceneDataPtr current = std::make_shared<SceneData>();
        current->fetchCurrent(sno_id);
        if (current->good) {
            sceneData[sno_id] = current;
        }
    }

    cleared = false;
}

void NavMesh::fetchSceneSno()
{
    // NOTE:offset
    Container<SnoDefinition> c = Pointer<Container<SnoDefinition>>()(Addr_SnoGroupByCode+sizeof(void*)*(int)(SnoGroupId_Scene),
                                                                     offsetof(SnoGroupManager, x10_Container),
                                                                     0);

    for (const auto & d : enumerate_container(c)) {
        if(d.x00_Id == INVALID_SNO_ID || d.x07_SnoGroupId != (char)SnoGroupId_Scene){
            continue;
        }

        SceneSnoDataPtr s = std::make_shared<SceneSnoData>((AssetScene *)d.x0C_pSNOAddr);
        snoSceneIdAddrMap[s->sno_id] = s;
    }
}

}
