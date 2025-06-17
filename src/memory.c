#include "memory.h"

void* reallocate(void* ptr, usize oldSize, usize newSize) {
	if(newSize == 0) {
		free(ptr);
		return NULL;
	}
	void* result = realloc(ptr, newSize);
	return result;
}