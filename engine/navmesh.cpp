#include "navmesh.h"

#include <stdio.h>

#include "process/memoryreader.h"

namespace D3 {


SceneData::SceneData(const Scene &s)
{
    id = s.x000_Id;
    sno_id = s.x0E8_SceneSnoId;
    area_sno_id = s.x018_LevelAreaSnoId;

    min.x = s.x0FC_MeshMinX;
    min.x = s.x100_MeshMinY;
    min.x = s.x104_MeshMinZ;

    max.x = s.x174_MeshMaxX;
    max.y = s.x178_MeshMaxY;
    max.z = s.x104_MeshMinZ; //there is no max z, so consider all grid cells flat
}

SceneSnoData::SceneSnoData(const SnoScene &s)
{
    id = s.header.id;

    if (!NavMesh::getSerializedRecords(cells, s.NavZone.NavCells, 0)) {
        fprintf(stderr, "Failed to call NavMesh::getSerializedRecords()");
        return;
    }

    if (cells.empty()) {
        fprintf(stderr, "Empty result from NavMesh::getSerializedRecords()");
        return;
    }
}

template<class T>
bool NavMesh::getSerializedRecords(std::vector<T> &out, DataPtr2 ptr, DWORD dwBase)
{
    //Real data start
    DWORD dwDataStart = dwBase + ptr.file_offset;

    //Read records
    while ( dwDataStart < dwBase + ptr.file_offset + ptr.size )
    {
        T data;

        if (!MemoryReader::instance()->read((void*)dwDataStart, &data, sizeof(T))) {
            out.clear();
            return false;
        }

        out.push_back(data);

        dwDataStart += sizeof(T);
    }

    return true;
}

}
