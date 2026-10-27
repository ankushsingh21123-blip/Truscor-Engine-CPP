cmake_minimum_required(VERSION 3.20)
project(TruscorEngine VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Optimization flags for high-throughput numerics
if(MSVC)
    add_compile_options(/O2 /Oi /Ot /fp:fast)
else()
    add_compile_options(-O3 -march=native -ffast-math -Wall -Wextra -pthread)
endif()

include_directories(include)

add_executable(truscor_engine
    src/main.cpp
    src/risk_engine.cpp
)

find_package(Threads REQUIRED)
target_link_libraries(truscor_engine PRIVATE Threads::Threads)
