//
// Created by rtsboom on 2026-10-29.
//
#pragma comment(lib, "onecore.lib")

#include "VirtualMemory.h"

// Use VirtualAlloc2
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>

namespace ezrt4x::virtual_memory {

void* reserve(size_t const size, size_t const alignment) noexcept {
	MEM_ADDRESS_REQUIREMENTS requirements { };
	requirements.Alignment = alignment;

	MEM_EXTENDED_PARAMETER parameter { };
	parameter.Type = MemExtendedParameterAddressRequirements;
	parameter.Pointer = &requirements;

	void* address = VirtualAlloc2(
	    nullptr,
	    nullptr,
	    size,
	    MEM_RESERVE,
	    PAGE_NOACCESS,
	    &parameter,
	    1);

	return address;
}
bool commit(void* address, size_t const size) noexcept {
	return (nullptr != VirtualAlloc(address, size, MEM_COMMIT, PAGE_READWRITE));
}
bool decommit(void* address, size_t const size) noexcept {
	return VirtualFree(address, size, MEM_DECOMMIT);
}
bool release(void* address) noexcept {
	return VirtualFree(address, 0, MEM_RELEASE);
}
} // ezrt4x::virtual_memory