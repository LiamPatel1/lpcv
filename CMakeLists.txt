cmake_minimum_required (VERSION 3.15)
set(CMAKE_CXX_STANDARD 23)

set(VCPKG_TARGET_TRIPLET "x64-windows-static")
set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE FILEPATH "Path to toolchain")

project ("lpcv")

find_package(PNG REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS WIDGETS)
find_package(Catch2 3 REQUIRED)


add_executable (lpcv
				"src/main.cpp"
				"src/imagereader.cpp"
				"src/viewer.cpp"
				"src/gaussian.cpp"
				"include/lpcv.h"
				"include/lpcv/viewer.h"
				"include/lpcv/imagereader.h"
				"include/lpcv/gaussian.h"
				"include/lpcv/vec.h"
				"include/lpcv/kernel.h"
				"include/lpcv/image.h"
				"include/lpcv/enum.h"
 "include/lpcv/enum.h")

target_include_directories(lpcv PRIVATE include)


target_link_libraries(lpcv PNG::PNG Qt6::Widgets)
set_property(TARGET lpcv PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")


add_executable (tests "test.cpp")
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)



include(CTest)
include(Catch)
catch_discover_tests(tests)


