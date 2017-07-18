cmake_minimum_required(VERSION 3.0)

project(ox)

option(ENABLE_TEST "Enable to build test" ON)

set(root ${CMAKE_CURRENT_LIST_DIR})
list(APPEND CMAKE_PREFIX_PATH ${root}/deps ${root}/cmake)
list(APPEND CMAKE_MODULE_PATH ${root}/cmake)

find_package(cxxopts REQUIRED)

include(common-cxx-flags)

add_executable(ox src/main.cc)
target_compile_options(ox PUBLIC ${COMMON_CXX_FLAGS})
target_include_directories(ox PUBLIC
  ${root}/src
  ${root}/include
  ${cxxopts_INCLUDE_DIR}
)

if(ENABLE_TEST)
  enable_testing()
  add_subdirectory(${root}/deps/googletest)
  add_executable(ox-test test/main.cc test/ox.cc)
  target_compile_options(ox-test PUBLIC ${COMMON_CXX_FLAGS})
  target_include_directories(ox-test PUBLIC
    ${root}/include
  )
  target_link_libraries(ox-test gtest)
endif()
