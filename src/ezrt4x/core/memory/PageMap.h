//
// Created by rtsboom on 2026-10-29.
//

#pragma once

#include <cstdint>
#include <atomic>

namespace ezrt4x::memory {
struct Block;
struct Page {
	uint16_t block_count;
	uint16_t block_used_count;
	uint16_t block_init_count;
	uint8_t size_class;
	uint8_t is_full : 1;
	uint8_t is_free : 1;
	uint8_t is_decommitted : 1;

	Page* next;
	Page* prev;

	Block* local_free;
	alignas(64) std::atomic_uintptr_t remote_free;
};
static_assert(sizeof(Page) == 128);


} // ezrt4x::memory
