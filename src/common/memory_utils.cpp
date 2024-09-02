/********************************************
 * Filename: memory_utils.cpp
 * Created by zhaojiadi on 2023/4/29
 * Description:
 ********************************************/
#include <stdint.h>
#include <stdlib.h>
#include "common/memory_utils.h"
#include "common/log.h"
// #define DEBUG_MEMORY

namespace AVCPP_EXAMPLE
{

    static inline void **alignPointer(void **ptr, size_t alignment)
    {
        return (void **)((intptr_t)((unsigned char *)ptr + alignment - 1) & -alignment);
    }

    void *memoryAllocAlign(size_t size, size_t alignment)
    {
        CHECK(size > 0, "size is invalid");

    #ifdef DEBUG_MEMORY
        return malloc(size);
    #else
        void **origin = (void **)malloc(size + sizeof(void *) + alignment);
        CHECK(origin != nullptr, "malloc return nullptr");
        void **aligned = alignPointer(origin + 1, alignment);
        aligned[-1]    = origin;
        return aligned;
    #endif
    }

    void *memoryCallocAlign(size_t size, size_t alignment)
    {
        CHECK(size > 0, "size is invalid");

    #ifdef DEBUG_MEMORY
        return calloc(size, 1);
    #else
        void **origin = (void **)calloc(size + sizeof(void *) + alignment, 1);
        CHECK(origin != nullptr, "calloc return nullptr");
        void **aligned = alignPointer(origin + 1, alignment);
        aligned[-1]    = origin;
        return aligned;
    #endif
    }

    void memoryFreeAlign(void *aligned)
    {
    #ifdef DEBUG_MEMORY
        free(aligned);
    #else
        if (aligned) {
            void *origin = ((void **)aligned)[-1];
            free(origin);
        }
    #endif
    }

} //namespace AVCPP_EXAMPLE