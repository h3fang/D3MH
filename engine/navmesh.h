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
    bool cached;

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
    SceneSnoDataPtr sceneSnoDataPtr;
    bool good;

public:
    SceneData();
    SceneData(const Scene &s);

    void fetchCurrent(uint sno_id);
    void fromScene(const Scene &s);
    bool findSceneSnoData();
};

typedef std::shared_ptr<SceneData> SceneDataPtr;

class NavMesh
{
public:
    static std::unordered_map<uint, SceneSnoDataPtr> snoSceneIdAddrMap;
    std::unordered_map<uint, SceneDataPtr> sceneData;

public:
    NavMesh();
    ~NavMesh();

    void loadSceneSnoFiles();
    void update();
    void clear();
    void fetchScene();
    void fetchSceneSno();

private:
    bool cleared;
    uint last_level_area_sno_id;
};

}

#endif // NAVMESH_H
