#ifndef NAVMESH_H
#define NAVMESH_H

#include <cmath>
#include <memory>
#include <vector>
#include <unordered_map>

#include "structs.h"

namespace D3 {

class SceneSnoData
{
public:
    uint sno_id;
    std::vector<NavCell> cells;
    bool loaded;

public:
    SceneSnoData();
    SceneSnoData(uint sno_id);
    SceneSnoData(AssetScene *sno_ptr);

    ~SceneSnoData();

    bool save();
    bool load(uint sno_id);
};

typedef std::shared_ptr<SceneSnoData> SceneSnoDataPtr;

class SceneData
{
public:
    uint id;
    uint sno_id;
    uint levelArea_sno_id;
    Vec3 min;
    Vec3 max;

public:
    SceneData();
    SceneData(const Scene &s);

    void fromScene(const Scene &s);
    SceneSnoDataPtr findSceneSnoData();
};

typedef std::shared_ptr<SceneData> SceneDataPtr;

class Engine;

class NavMesh
{
public:
    static std::unordered_map<uint, SceneSnoDataPtr> snoSceneIdAddrMap;
    std::unordered_map<uint, SceneDataPtr> sceneData;

public:
    NavMesh(Engine *e);
    ~NavMesh();

    void loadSceneSnoFiles();
    void update();
    void clear();
    void fetchScene();
    void fetchSceneSno();

private:
    Engine* engine;
    uint last_world_sno_id;
};

}

#endif // NAVMESH_H
