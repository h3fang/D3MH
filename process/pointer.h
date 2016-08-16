#ifndef UTILITIES_POINTER_H
#define UTILITIES_POINTER_H

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
        memset(&result, 0, sizeof(T));
        MemoryReader::instance()->read(&result, (void*)(current_ptr+ptr), sizeof(T));
        return result;
    }

    template<class P, class... Ptrs>
    T operator()(const P ptr, const Ptrs... ptrs)
    {
        current_ptr += ptr;
        MemoryReader::instance()->read(&current_ptr, (void*)current_ptr, sizeof(void*));
        return this->operator ()(ptrs...);
    }

private:
    unsigned long current_ptr;
};

#endif // UTILITIES_POINTER_H
