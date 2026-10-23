//
// Created by rtsboom on 2026-10-22.
//

#include <ezrt4x/core/memory/MemoryAllocator.h>
#include <iostream>
#include <cassert>

int main() {

	using namespace ezrt4x::memory;

	// fake global heap pool
	HeapPool g_heap_pool { };

	Heap* heap = g_heap_pool.acquire();
	assert(heap != nullptr);

	g_heap_pool.release(heap);
	Heap* heap2 = g_heap_pool.acquire();
	assert(heap == heap2);

	Heap* heap3 = g_heap_pool.acquire();
	assert(heap != heap3);

	g_heap_pool.release(heap2);
	g_heap_pool.release(heap3);

	Heap* heap4 = g_heap_pool.acquire();
	Heap* heap5 = g_heap_pool.acquire();

	assert(heap4 == heap3);
	assert(heap5 = heap2);

	std::cout << "test succeeded" << std::endl;
	return 0;
}