cmake_minimum_required (VERSION 3.15)
set(CMAKE_CXX_STANDARD 23)

set(VCPKG_TARGET_TRIPLET "x64-windows-static")
set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE FILEPATH "Path to toolchain")

project ("lpcv")

find_package(PNG REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS WIDGETS)


add_executable (lpcv "main.cpp" "lpcv.cpp" "lpcv.h" "imagereader.cpp" "lpcv/imagereader.h" "viewer.cpp" "lpcv/viewer.h" "image.cpp" "lpcv/image.h" "gaussian.cpp" "lpcv/gaussian.h" "lpcv/vec.h" "lpcv/kernel.h")
set_property(TARGET lpcv PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

target_link_libraries(lpcv PNG::PNG Qt6::Widgets)