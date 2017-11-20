#include "common.h"

#ifndef MEMORY_SIZE
#define MEMORY_SIZE 81920
#endif

static char memory[MEMORY_SIZE];

void *get_memory_adr() {
	return memory;
}

size_t get_memory_size() {
	return MEMORY_SIZE;
}
