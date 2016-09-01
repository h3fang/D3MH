#include "snocache.h"

#include <fstream>
#include <algorithm>

#include <QDebug>
#include <QDir>

namespace D3 {

SnoCache::SnoCache(const std::string& path) :
    path(path)
{

}

bool SnoCache::build()
{
    QDir dir(path.data());

    if (!dir.exists()) {
        return false;
    }

    QStringList filters;
    filters << "*.scn";

    auto files = dir.entryList(filters, QDir::Files|QDir::NoSymLinks, QDir::Name);
    uint count = 0;
    for (QString s : files) {
        std::ifstream file(path + s.toStdString(), std::ifstream::binary);

        if (!file) {
            continue;
        }

        file.seekg (0, file.end);
        uint length = file.tellg();

        if (length < sizeof(SceneSnoFile)) {
            continue;
        }

        file.seekg (0, file.beg);

        char* buffer = new char[length];

        file.read( buffer, length);

        SceneSnoFile* ssf = (SceneSnoFile*)buffer;

        if (!file.good() || ssf->header.deadbeef != 0xDEADBEEF ||
                ssf->sceneSno.NavZone.NavCellCount <= 0 ||
                ssf->sceneSno.NavZone.NavCellCount > 1000 ||
                ssf->sceneSno.NavZone.NavCells.size != sizeof(NavCell)*ssf->sceneSno.NavZone.NavCellCount ||
                (sizeof(ssf->header) + ssf->sceneSno.NavZone.NavCells.size + ssf->sceneSno.NavZone.NavCells.file_offset) > length) {
            delete[] buffer;
            continue;
        }

        SceneSnoDataPtr ss = std::make_shared<SceneSnoData>(ssf);
        if (!ss->cells.empty()) {
            ss->save();
            ++count;
        }

        delete[] buffer;
    }

    qDebug("parsed %u SceneSno files", count);

    return true;
}

}

