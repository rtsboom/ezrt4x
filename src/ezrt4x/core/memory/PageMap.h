//
// Created by rtsboom on 2026-10-29.
//

/*
 * PageMap Leaf Design
 *
 * - Leaves exist conceptually; no separate PageMapLeaf structure is needed.
 * - Each Arena stores PageEntry* arrays in its metadata region.
 * - These arrays are divided into logical Leaves and registered in the
 *   corresponding slots of the global PageMap.
 * - Each Leaf occupies 64 KiB and covers 512 MiB of virtual address space.
 * - Arenas must be aligned to 512 MiB, with sizes that are multiples of 512 MiB.
 * - All required Leaves are committed and initialized during Arena creation,
 *   before being registered in the PageMap.
 *
 * This design aligns Arena boundaries with PageMap Leaf boundaries,
 * eliminating per-Leaf allocation and commit synchronization.
 */


#pragma once

#include <cstdint>
#include <atomic>

namespace ezrt4x::memory {
struct Block;
struct PageEntry {
	uint16_t block_count;
	uint16_t block_used_count;
	uint16_t block_init_count;
	uint8_t size_class;
	uint8_t is_full : 1;
	uint8_t is_free : 1;
	uint8_t is_decommitted : 1;

	PageEntry* next;
	PageEntry* prev;

	Block* local_free;
	alignas(64) std::atomic_uintptr_t remote_free;
};
static_assert(sizeof(PageEntry) == 128);

inline constexpr size_t kAddressBits = 47;
inline constexpr size_t kPageSize = 1ULL << 16; // 64K
inline constexpr size_t kPageMapLeafSize = kPageSize;
inline constexpr size_t kEntriesPerLeaf = kPageMapLeafSize / sizeof(PageEntry*);
inline constexpr size_t kLeavesPerPageMap = (1ULL << kAddressBits) / kPageSize / kEntriesPerLeaf;

//
// struct PageMapLeaf {
// 	PageEntry* entries[kEntriesPerLeaf];
// };
//
// struct PageMap {
// 	std::atomic_uintptr_t* leaves;
//
// 	PageMapLeaf* get_leaf(size_t index) noexcept;
// 	bool set_leaf(size_t index, PageMapLeaf* leaf) noexcept;
// };

} // ezrt4x::memory
