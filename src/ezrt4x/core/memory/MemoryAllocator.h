//
// Created by rtsboom on 2026-10-22.
//
#pragma once


#include <windows.h>

namespace ezrt4x {

class MemoryAllocator {
public:
	static void* Allocate(size_t size, size_t alignment = 16);

	static void Deallocate(void* ptr);
};
} // ezrt4x

namespace ezrt4x::memory {



}