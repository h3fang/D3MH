#ifndef NAVMESH_H
#define NAVMESH_H

#include <cmath>
#include <memory>
#include <vector>
#include <unordered_map>
#include <unordered_set>

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
    SceneSnoData(SceneSno *sno_ptr);
    SceneSnoData(SceneSnoFile *s);

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
    uint navmesh_id;
    uint world_sno_id;
    Vec2 min;
    Vec2 max;

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
    struct KeyHasher {
        size_t operator()( const SceneDataPtr& k ) const {
            size_t res = 17 + k->navmesh_id;
            res = res * 31 + std::hash<float>()( k->min.x );
            res = res * 31 + std::hash<float>()( k->min.y );
            res = res * 31 + std::hash<float>()( k->max.x );
            res = res * 31 + std::hash<float>()( k->max.y );
            return res;
        }
    };

    struct KeyCmp {
        bool operator()( const SceneDataPtr& lhs, const SceneDataPtr& rhs) const {
            return lhs->navmesh_id == rhs->navmesh_id &&
                    std::fabs(lhs->min.x - rhs->min.x) < 0.1 &&
                    std::fabs(lhs->min.y - rhs->min.y) < 0.1 &&
                    std::fabs(lhs->max.x - rhs->max.x) < 0.1 &&
                    std::fabs(lhs->max.y - rhs->max.y) < 0.1;
        }
    };

    static std::unordered_map<uint, SceneSnoDataPtr> sceneSnoData;
    std::unordered_set<SceneDataPtr, KeyHasher, KeyCmp> sceneData;

public:
    NavMesh(Engine *e);
    ~NavMesh();

    void update();
    void clearAll();

private:
    void loadSceneSnoFiles();
    void clear();
    void fetchScene();
    void fetchSceneSno();

private:
    Engine* engine;
    uint current_world_sno_id;
};

}

#endif // NAVMESH_H
