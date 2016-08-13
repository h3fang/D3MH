#ifndef NAVMESH_H
#define NAVMESH_H

#include <memory>
#include <vector>
#include <unordered_map>

#include "structs.h"

namespace D3 {

class SceneSnoData
{
public:
    int sno_id;
    std::vector<NavCell> cells;

public:
    SceneSnoData();
    SceneSnoData(int sno_id);
    SceneSnoData(AssetScene *sno_ptr);

    ~SceneSnoData();

    bool save();
    bool load(int sno_id);
    inline bool isCached() { return cached; }

private:
    bool cached;
};

typedef std::shared_ptr<SceneSnoData> SceneSnoDataPtr;

class SceneData
{
public:
    int id;
    int sno_id;
    int levelArea_sno_id;
    Vec3 min;
    Vec3 max;
    SceneSnoDataPtr sceneSnoDataPtr;
    bool finished;

public:
    SceneData(const Scene &s);

    void loadFromMemory(const Scene &s);
};

typedef std::shared_ptr<SceneData> SceneDataPtr;

class NavMesh
{
public:
    static std::unordered_map<DWORD, SceneSnoDataPtr> snoSceneIdAddrMap;
    std::unordered_map<int, SceneDataPtr> sceneData;

public:
    NavMesh();

    void update();
    void clear();
    void clearScene();
    void clearSnoScene();
    void parseMemorySnoScene();

    template<class T>
    static bool getSerializedRecords(std::vector<T> &out, DataPtr2 ptr, DWORD dwBase = 0);

private:
    bool cleared;
    int last_level_area_sno_id;
};

}

#endif // NAVMESH_H
