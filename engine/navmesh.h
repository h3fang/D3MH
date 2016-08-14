#ifndef NAVMESH_H
#define NAVMESH_H

#include <cmath>
#include <memory>
#include <vector>
#include <unordered_map>
#include <set>

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

bool operator < (const Vec3& lhs, const Vec3& rhs);

class NavMesh
{
public:
    struct SceneDatacomp {
        bool operator() (const SceneDataPtr& lhs, const SceneDataPtr& rhs) const {
            if (lhs->sno_id == rhs->sno_id) {
                return lhs->min < rhs->min;
            }
            else {
                return lhs->sno_id < rhs->sno_id;
            }
        }
    };

    static std::unordered_map<uint, SceneSnoDataPtr> snoSceneIdAddrMap;
    std::set<SceneDataPtr, SceneDatacomp> sceneData;

public:
    NavMesh();

    void loadSceneSnoFiles();
    void update();
    void clear();
    void fetchScene();
    void fetchSceneSno();

    template<class T>
    static bool getSerializedRecords(std::vector<T> &out, DataPtr2 ptr, uint dwBase = 0);

private:
    bool cleared;
    uint last_level_area_sno_id;
};

}

#endif // NAVMESH_H
