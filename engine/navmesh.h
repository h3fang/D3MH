#ifndef NAVMESH_H
#define NAVMESH_H

#include <memory>
#include <vector>
#include <unordered_map>

#include "structs.h"

namespace D3 {

class SceneData
{
public:
    int id;
    int sno_id;
    int levelArea_sno_id;
    Vec3 min;
    Vec3 max;
    std::vector<NavCell> cells;
    bool finished;

public:
    SceneData(const Scene &s);

    void loadFromMemory(const Scene &s);
};

class NavMesh
{
public:
    static std::unordered_map<int, DWORD> snoSceneIdAddrMap;
    std::unordered_map<int, SceneData*> sceneData;

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
