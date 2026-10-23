add_library(ezrt4x_core STATIC
        memory/MemoryAllocator.cpp
)

target_compile_features(ezrt4x_core PUBLIC cxx_std_20)

target_include_directories(ezrt4x_core PUBLIC ${PROJECT_SOURCE_DIR}/src)

