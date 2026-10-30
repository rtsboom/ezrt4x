//
// Created by rtsboom on 2026-10-22.
//

#include "MemoryAllocator.h"
#include <cstdlib>

namespace ezrt4x {
void* MemoryAllocator::Allocate(size_t const size, size_t alignment) {
	return std::malloc(size);
}

void MemoryAllocator::Deallocate(void* ptr) {
	std::free(ptr);
}
} // ezrt4x

