/********************************************
 * Filename: memory_utils.h
 * Created by zhaojiadi on 2023/4/29
 * Description:
 ********************************************/
#pragma once
#include <stdio.h>
#define MEMORY_ALIGN_DEFAULT 64

namespace AVCPP_EXAMPLE
{

    void* memoryAllocAlign(size_t size, size_t align);
    void* memoryCallocAlign(size_t size, size_t align);
    void memoryFreeAlign(void* mem);

} //namespace AVCPP_EXAMPLE