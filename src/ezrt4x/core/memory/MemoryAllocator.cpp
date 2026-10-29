//
// Created by rtsboom on 2026-10-22.
//

#include "MemoryAllocator.h"

// Use VirtualAlloc2
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#pragma comment(lib, "onecore.lib")

#include <cstdlib>

namespace ezrt4x {
void* MemoryAllocator::Allocate(size_t const size, size_t alignment) {
	return std::malloc(size);
}

void MemoryAllocator::Deallocate(void* ptr) {
	std::free(ptr);
}
} // ezrt4x

