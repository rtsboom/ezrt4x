//
// Created by rtsboom on 2026-10-29.
//

#pragma once

#include <cstdint>
#include <mutex>

namespace ezrt4x::memory {

inline constexpr uintptr_t kArenaSizeShift = 31;
inline constexpr uintptr_t kChunkSizeShift = 25;
inline constexpr uintptr_t kPageSizeShift = 16;

inline constexpr size_t kArenaSize = 1ULL << kArenaSizeShift; // 2 GiB
inline constexpr size_t kChunkSize = 1ULL << kChunkSizeShift; // 32 MiB
inline constexpr size_t kPageSize = 1ULL << kPageSizeShift; // 64 KiB

inline constexpr uintptr_t kArenaSizeMask = ~(kArenaSize - 1);
inline constexpr uintptr_t kChunkSizeMask = ~(kChunkSize - 1);
inline constexpr uintptr_t kPageSizeMask = ~(kPageSize - 1);

inline constexpr size_t kArenaChunkCount = kArenaSize >> kChunkSizeShift; // 64
inline constexpr size_t kArenaPageCount = kArenaSize >> kPageSizeShift; // 32K
inline constexpr size_t kChunkPageCount = kChunkSize >> kPageSizeShift; // 512

struct Block {
	Block* next;
};

class Heap {
public:
	Heap() = default;
	~Heap() noexcept = default;

	void* alloc(size_t size, size_t alignment = 16) noexcept;
	void dealloc(void* ptr) noexcept;

private:
	friend class HeapPool;
	Heap* m_next_heap = nullptr;

private:
};

class HeapPool {
public:
	Heap* acquire() noexcept;

	void release(Heap* heap) noexcept;

private:
	static Heap* createHeap() noexcept;

private:
	std::mutex m_mutex;
	Heap* m_head = nullptr;

	int m_acquire_count { };
	int m_release_count { };
	int m_create_count { };
};

} // ezrt4x
