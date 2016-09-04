#ifndef UTILITIES_POINTER_H
#define UTILITIES_POINTER_H

#include <cstring>

#include "memoryreader.h"

template<class T>
class Pointer
{
public:
    Pointer() :
        current_ptr(0)
    {}

    template<class P>
    T operator()(const P ptr)
    {
        T result;
        if (!MemoryReader::instance()->read(&result, (void*)(current_ptr+ptr), sizeof(T))) {
            memset(&result, 0xff, sizeof(T));
        }
        return result;
    }

    template<class P, class... Ptrs>
    T operator()(const P ptr, const Ptrs... ptrs)
    {
        current_ptr += ptr;
        if (MemoryReader::instance()->read(&current_ptr, (void*)current_ptr, sizeof(void*))) {
            return this->operator ()(ptrs...);
        }
        else {
            T result;
            memset(&result, 0xff, sizeof(T));
            return result;
        }
    }

private:
    unsigned long current_ptr;
};

#endif // UTILITIES_POINTER_H
