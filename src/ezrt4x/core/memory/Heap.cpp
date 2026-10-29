//
// Created by rtsboom on 2026-10-29.
//

#include "Heap.h"
#include "VirtualMemory.h"
#include <cassert>



namespace ezrt4x::memory {
static HeapPool g_heap_pool;

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
	constexpr size_t size = kArenaSize;
	void* address = ezrt4x::virtual_memory::reserve(kArenaSize, kArenaSize);

	assert(address != nullptr);
	ezrt4x::virtual_memory::commit(address, sizeof(Heap));

	return new (address) Heap { };
}
} // ezrt4x::memory