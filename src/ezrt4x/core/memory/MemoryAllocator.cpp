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

#include <cassert>

namespace ezrt4x {
void* MemoryAllocator::Allocate(const size_t size, size_t alignment) {
	return std::malloc(size);
}

void MemoryAllocator::Deallocate(void* ptr) {
	std::free(ptr);
}
} // ezrt4x

namespace ezrt4x::memory {
namespace {
	HeapPool g_heap_pool;
}

Heap* HeapPool::acquire() noexcept {
	{
		std::scoped_lock lk { m_mutex };
		++m_acquire_count;

		if (Heap* heap = m_head) {
			m_head = m_head->m_next_heap;
			heap->m_next_heap = nullptr;
			return heap;
		}

		++m_create_count;
	}

	return createHeap();
}
void HeapPool::release(Heap* heap) noexcept {
	assert(heap != nullptr);

	std::scoped_lock lk { m_mutex };
	heap->m_next_heap = m_head;
	m_head = heap;

	++m_release_count;
}
Heap* HeapPool::createHeap() noexcept {
	constexpr size_t size = sizeof(Heap);
	constexpr DWORD flag = MEM_RESERVE | MEM_COMMIT;
	void* ptr = ::VirtualAlloc2(
	    nullptr,
	    nullptr,
	    size,
	    flag,
	    PAGE_READWRITE,
	    nullptr,
	    0);

	assert(ptr != nullptr);
	return new (ptr) Heap { };
}
}