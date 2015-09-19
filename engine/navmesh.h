#ifndef NAVMESH_H
#define NAVMESH_H

#include <memory>
#include <vector>

#include "structs.h"

namespace D3 {

class SceneData
{
public:
    int id;
    int sno_id;
    int area_sno_id;
    Vec3 min;
    Vec3 max;

public:
    SceneData(const Scene &s);
};

class SceneSnoData
{
public:
    int id;
    std::vector<NavCell> cells;

public:
    SceneSnoData(const SnoScene &s);
};

class NavMesh
{
public:
    NavMesh();
    ~NavMesh();

    template<class T>
    static bool getSerializedRecords(std::vector<T> &out, DataPtr2 ptr, DWORD dwBase = 0);
};

}

#endif // NAVMESH_H
