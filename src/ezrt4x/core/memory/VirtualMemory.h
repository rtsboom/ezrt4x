//
// Created by rtsboom on 2026-10-29.
//
#pragma once

namespace ezrt4x::virtual_memory {

[[nodiscard]]
void* reserve(size_t size, size_t alignment) noexcept;

bool commit(void* address, size_t size) noexcept;

bool decommit(void* address, size_t size) noexcept;

bool release(void* address) noexcept;

} // ezrt4x::virtual_memory
