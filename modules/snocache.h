#ifndef SNOCACHE_H
#define SNOCACHE_H

#include <string>

#include "engine.h"

namespace D3 {

class SnoCache
{
public:
    SnoCache(const std::string& path);

    bool build();

private:
    std::string path;
};

}

#endif // SNOCACHE_H
