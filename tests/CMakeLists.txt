add_executable(MemoryTests MemoryTests.cpp)

target_link_libraries(MemoryTests
        PRIVATE ezrt4x_core
)

add_test(
        NAME MemoryTests COMMAND MemoryTests
)