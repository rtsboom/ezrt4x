//
// Created by rtsboom on 2026-10-22.
//

#include <ezrt4x/core/memory/MemoryAllocator.h>
#include <iostream>
#include <cassert>
#include <barrier>
#include <thread>
#include <vector>

static void TestHeapPoolConcurrency() {
	constexpr size_t thread_count = 16;
	constexpr size_t iteration_count = 100000;

	ezrt4x::memory::HeapPool pool;
	std::barrier start_barrier { thread_count };

	std::vector<std::jthread> threads;
	for (size_t i { }; i < thread_count; ++i) {
		threads.emplace_back([&] {
			for (size_t round { }; round < iteration_count; ++round) {
				start_barrier.arrive_and_wait();
				auto* heap = pool.acquire();
				assert(heap != nullptr);
				std::this_thread::yield();

				pool.release(heap);
			}
		});
	}

	threads.clear();
	std::cout << "TestHeapPoolConcurrency completed." << std::endl;
}

int main() {
	TestHeapPoolConcurrency();

	return 0;
}