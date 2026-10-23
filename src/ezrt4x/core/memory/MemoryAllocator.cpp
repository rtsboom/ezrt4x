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

		if (Heap* heap = m_next_free_heap) {
			m_next_free_heap = m_next_free_heap->m_next_heap;
			heap->m_next_heap = nullptr;
			return heap;
		}
	}

	return createHeap();
}
void HeapPool::release(Heap* heap) noexcept {
	assert(heap != nullptr);
	if (!heap) {
		return;
	}

	std::scoped_lock lk { m_mutex };
	heap->m_next_heap = m_next_free_heap;
	m_next_free_heap = heap;
}
Heap* HeapPool::createHeap() noexcept {
	constexpr DWORD flag = MEM_RESERVE | MEM_COMMIT;
	void* ptr = ::VirtualAlloc2(
	    nullptr,
	    nullptr,
	    4 * 1024,
	    flag,
	    PAGE_READWRITE,
	    nullptr,
	    0);

	if (!ptr) {
		return nullptr;
	}

	return new (ptr) Heap { };
}
}