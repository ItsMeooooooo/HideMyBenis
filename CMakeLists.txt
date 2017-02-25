cmake_minimum_required(VERSION 2.8)
project(HideMyBenis)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

find_package(Sodium REQUIRED)
include_directories(${SODIUM_INCLUDE_DIR})
include_directories(${STB_INCLUDE_DIR})
set(LIBS ${LIBS} ${SODIUM_LIBRARY_DIR})

set(SOURCE_FILES main.cpp benis_crypt.h benis_crypt.cpp)
add_executable(HideMyBenis ${SOURCE_FILES})
target_link_libraries(HideMyBenis ${LIBS})
