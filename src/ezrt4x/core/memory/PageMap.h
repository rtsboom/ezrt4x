//
// Created by rtsboom on 2026-10-29.
//

/*
 * PageMap Leaf Design
 *
 * - Leaves are embedded in the metadata region of each Arena.
 * - Each Leaf is a contiguous array of PageEntry pointers,
 *   without a separate PageMapLeaf structure.
 * - Each Leaf occupies 64 KiB and covers 512 MiB of virtual address space.
 * - Arenas must be 512 MiB-aligned, with sizes that are multiples of 512 MiB.
 * - All Leaves are committed and initialized during Arena creation,
 *   then registered in the corresponding global PageMap slots.
 *
 * This design aligns Arena boundaries with PageMap Leaf boundaries,
 * eliminating separate Leaf allocations and commit synchronization.
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
