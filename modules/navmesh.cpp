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

std::unordered_map<uint, SceneSnoDataPtr> NavMesh::sceneSnoData;

SceneSnoData::SceneSnoData() :
    sno_id(INVALID_SNO_ID),
    loaded(false)
{
}

SceneSnoData::SceneSnoData(uint sno_id) :
    sno_id(INVALID_SNO_ID)
{
    loaded = load(sno_id);
}

SceneSnoData::SceneSnoData(SceneSno* sno_ptr) :
    sno_id(INVALID_SNO_ID),
    loaded(false)
{
    SceneSno s = Pointer<SceneSno>()(sno_ptr);

    sno_id = s.header.x00_SnoId;

    if (s.NavZone.NavCellCount <= 0) {
        return;
    }

    if (s.NavZone.NavCellCount > 2000 ||
            s.header.x04_LockCount > 10) {
         qDebug("Sno Id [%u] lockcount %d flags %d cell count %d\n", sno_id, s.header.x04_LockCount, s.header.x08_Flags, s.NavZone.NavCellCount);
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

SceneSnoData::SceneSnoData(SceneSnoFile *s) :
    sno_id(INVALID_SNO_ID),
    loaded(false)
{
    sno_id = s->sceneSno.header.x00_SnoId;

    if (s->sceneSno.NavZone.NavCellCount <= 0 || s->sceneSno.NavZone.NavCellCount > 2000 || s->sceneSno.header.x04_LockCount > 10) {
        return;
    }

    getSerializedDataFromFile(cells, s->sceneSno.NavZone.NavCells, (uint)(&s->sceneSno));

//    qDebug("Sno Id [%u] lockcount %d flags %d cell count %d\n", sno_id, s.header.x04_LockCount, s.header.x08_Flags, s.NavZone.NavCellCount);

    if (cells.empty()) {
        qDebug("Got 0 serialized records for Sno Id [%u] lockcount %d flags %d cell count %d\n",
               sno_id, s->sceneSno.header.x04_LockCount, s->sceneSno.header.x08_Flags, s->sceneSno.NavZone.NavCellCount);
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

    return file.good();
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

    NavCell c;

    for (int i=0; i<size; ++i) {
        file.read((char *)&c, sizeof(c));

        if (file.fail() || file.bad()) {
            cells.clear();
            return false;
        }

        cells.push_back(c);
    }

    this->sno_id = sno_id;
    return true;
}

SceneData::SceneData() :
    id(INVALID_SNO_ID),
    sno_id(INVALID_SNO_ID),
    navmesh_id(INVALID_SNO_ID),
    world_sno_id(INVALID_SNO_ID),
    min(Vec2{0, 0}),
    max(Vec2{0, 0})
{
}

SceneData::SceneData(const Scene &s)
{
    id = s.x000_Id;
    sno_id = s.x0E8_SceneSnoId;
    navmesh_id = s.x004_NavMeshId;
    world_sno_id = s.x050_WorldsSnoId;

    min.x = s.x0FC_MeshMinX;
    min.y = s.x100_MeshMinY;

    max.x = s.x174_MeshMaxX;
    max.y = s.x178_MeshMaxY;
}

SceneData::SceneData(const SceneRevealInfo &s)
{
    id = s.x04_SceneId_;
    sno_id = s.x00_SceneSnoId;
    navmesh_id = s.x00_SceneSnoId; // for convenience
    world_sno_id = INVALID_SNO_ID;

    min.x = s.x10_MinX;
    min.y = s.x14_MinY;

    max.x = s.x18_MaxX;
    max.y = s.x1C_MaxY;
}

SceneSnoDataPtr SceneData::findSceneSnoData()
{
    if (NavMesh::sceneSnoData.find(sno_id) == NavMesh::sceneSnoData.end()) {
        qDebug("No record for SceneSno id [%u] in NavMesh::SceneSnoData", sno_id);
        return SceneSnoDataPtr();
    }
    else {
        return NavMesh::sceneSnoData[sno_id];
    }
}

NavMesh::NavMesh(Engine *e) :
    engine(e),
    current_world_sno_id(INVALID_SNO_ID)
{
    loadSceneSnoFiles();
}

NavMesh::~NavMesh()
{
    for (const auto& pair : sceneSnoData) {
        if (!pair.second->loaded) {
            pair.second->save();
        }
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
            if (!ss->cells.empty()) {
                sceneSnoData[sno_id] = ss;
            }
        }
    }

    qDebug("loaded %u SceneSno files", sceneSnoData.size());

    uint min = 0xFFFFFFFF, max = 0, total = 0;
    for (const auto& p : sceneSnoData) {
        uint size = p.second->cells.size();
        max = size > max ? size : max;
        min = size < min ? size : min;
        total += size;
    }

    qDebug("Scene Sno Files cell count, min: %d, max: %d, average: %f", min, max, total/float(sceneSnoData.size()));
}

void NavMesh::update()
{
    fetchSceneSno();
    fetchScene();
}

void NavMesh::clearAll()
{
    sceneData.clear();
}

void NavMesh::clear()
{
    for (auto it=sceneData.begin(); it!=sceneData.end();) {
        if((*it)->world_sno_id != current_world_sno_id){
            it = sceneData.erase(it);
        }
        else {
            ++it;
        }
    }
}

void NavMesh::fetchScene()
{
    // NOTE:offset
    uint world_sno_id = engine->localData.x0C_WorldSnoId;

    if(world_sno_id != current_world_sno_id){
        current_world_sno_id = world_sno_id;
        clear();
    }

    // from ObjectManager
    // NOTE:offset
    Container<Scene> c = Pointer<Container<Scene>>()(Addr_ObjectManager, offsetof(ObjectManager, x998_Scenes), 0);

    for (const auto& s : enumerate_container(c)) {
        if (s.x000_Id == INVALID_SNO_ID ||
                s.x0E8_SceneSnoId == INVALID_SNO_ID ||
                s.x174_MeshMaxX <= s.x0FC_MeshMinX ||
                s.x178_MeshMaxY <= s.x100_MeshMinY) {
            continue;
        }

        auto sd = std::make_shared<SceneData>(s);

        // insert and replace the old one if it exists
        sceneData.erase(sd);
        sceneData.insert(sd);
    }

    // from LevelArea
    // NOTE:offset
    ListPack<SceneRevealInfo> sri = Pointer<ListPack<SceneRevealInfo>>()(Addr_LevelArea, offsetof(LevelArea, x010_ListPack_Allocator_10x68Bytes_RevealedScenes));

    if (sri.x10_NodeAllocator.x18_GoodFood != 0x600DF00D) {
        qDebug("NavMesh::fetchScene() LevelArea ListPack<SceneRevealInfo> bad food");
        return;
    }

    LinkedListNode<SceneRevealInfo>* node_ptr = sri.First;

    while (node_ptr) {
        LinkedListNode<SceneRevealInfo> node = Pointer<LinkedListNode<SceneRevealInfo>>()(node_ptr);

        node_ptr = node.Next;

        if (node.Value.x00_SceneSnoId == INVALID_SNO_ID ||
                node.Value.x18_MaxX <= node.Value.x10_MinX ||
                node.Value.x1C_MaxY <= node.Value.x14_MinY) {
            continue;
        }

        auto sd = std::make_shared<SceneData>(node.Value);

        // insert and replace the old one if it exists
        sceneData.erase(sd);
        sceneData.insert(sd);
    }
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

        SceneSnoDataPtr s = std::make_shared<SceneSnoData>((SceneSno *)d.x0C_pSNOAddr);
        if (!s->cells.empty()/* && sceneSnoData.find(s->sno_id) == sceneSnoData.end()*/) {
            sceneSnoData[s->sno_id] = s;
        }
    }
}

}
