//
// Created by rtsboom on 2026-10-22.
//
#pragma once

#include <cstdlib>
#include <mutex>
#include <windows.h>

namespace ezrt4x {

class MemoryAllocator {
public:
	static void* Allocate(const size_t size, size_t alignment = 16);

	static void Deallocate(void* ptr);
};
} // ezrt4x

namespace ezrt4x::memory {


class Heap {
public:
	Heap() = default;
	~Heap() noexcept = default;

	void* alloc(size_t size, size_t alignment = 16) noexcept;
	void dealloc(void* ptr) noexcept;


private:
	friend class HeapPool;
	Heap* m_next_heap = nullptr;
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

};


}