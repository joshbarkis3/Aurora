#ifndef memory_h
#define memory_h

#include "common.h"

#define GROW_CAPACITY(capacity) ((capacity) < 8 ? 8 : (capacity) * 2)
#define GROW_ARR(type, ptr, oldCnt, newCnt) (type*)reallocate(ptr, sizeof(type) * (oldCnt), sizeof(type) * (newCnt))

void* reallocate(void* ptr, usize oldSize, usize newSize);

#endif