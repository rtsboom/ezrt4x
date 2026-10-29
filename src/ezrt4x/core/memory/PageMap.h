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

class PageMapEntry {
public:
	// No constructor on purpose (keeps the type trivial); use make() instead.
	static constexpr PageMapEntry make(bool const allocated, bool const large, uint16_t const offset) noexcept {
		uint16_t value = offset;
		value |= allocated ? kAllocatedMask : 0;
		value |= large ? kLargeMask : 0;

		PageMapEntry entry { };
		entry.m_value = value;

		return entry;
	}

	[[nodiscard]]
	bool is_allocated() const noexcept { return (m_value & kAllocatedMask) != 0; }

	[[nodiscard]]
	bool is_large() const noexcept { return (m_value & kLargeMask) != 0; }

	[[nodiscard]]
	uint16_t offset() const noexcept { return (m_value & kOffsetMask); }

private:
	static constexpr uint16_t kAllocatedMask = 0x8000;
	static constexpr uint16_t kLargeMask = 0x4000;
	static constexpr uint16_t kOffsetMask = 0x3FFF;
	uint16_t m_value;
};
static_assert(std::is_trivially_default_constructible_v<PageMapEntry>);
static_assert(std::is_trivially_copyable_v<PageMapEntry>);
static_assert(std::is_standard_layout_v<PageMapEntry>);
static_assert(sizeof(PageMapEntry) == 2);

} // ezrt4x::memory
