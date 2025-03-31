cmake_minimum_required (VERSION 3.8)
set(CMAKE_CXX_STANDARD 23)

set(VCPKG_TARGET_TRIPLET "x64-windows-static")
set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE FILEPATH "Path to toolchain")

project ("lpcv")

find_package(PNG REQUIRED)

add_executable (lpcv "lpcv.cpp" "lpcv.h" "imagereader.cpp" "main.cpp" "imagereader.h")

target_link_libraries(lpcv PNG::PNG)