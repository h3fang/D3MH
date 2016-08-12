#ifndef UTILITIES_POINTER_H
#define UTILITIES_POINTER_H

#include "memoryreader.h"

template<class T>
class Pointer
{
public:
    Pointer() :
        current_ptr(0)/*,
        mem_reader(MemoryReader::instance())*/
    {}

    template<class P>
    T operator()(const P ptr)
    {
        T result;
        memset(&result, 0, sizeof(T));
        mem_reader->read(&result, (void*)(current_ptr+ptr), sizeof(T));
        return result;
    }

    template<class P, class... Ptrs>
    T operator()(const P ptr, const Ptrs... ptrs)
    {
        current_ptr += ptr;
        mem_reader->read(&current_ptr, (void*)current_ptr, sizeof(void*));
        return this->operator ()(ptrs...);
    }

private:
    unsigned long current_ptr;
    static MemoryReader *mem_reader;
};

template<class T>
MemoryReader* Pointer<T>::mem_reader = MemoryReader::instance();

#endif // UTILITIES_POINTER_H
