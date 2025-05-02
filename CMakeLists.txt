cmake_minimum_required (VERSION 3.29)
set(CMAKE_CXX_STANDARD 23)

set(VCPKG_TARGET_TRIPLET "x64-windows-static")
set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE FILEPATH "Path to toolchain")
 
project ("lpcv")

find_package(PNG REQUIRED)
find_package(glfw3 REQUIRED)
find_package(Catch2 3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glad REQUIRED)


set(source_files
	"src/main.cpp"
	"src/imagereader.cpp"
	"src/convolution.cpp"
	"src/viewer.cpp"
    "src/operators.cpp"
)
set(header_files
	"include/lpcv.h"
	"include/lpcv/viewer.h"
	"include/lpcv/imagereader.h"
	"include/lpcv/convolution.h"
	
	"include/lpcv/kernel.h"
	"include/lpcv/image.h"
	"include/lpcv/enum.h"
    "include/lpcv/operators.h" 
	
	"include/lpcv/misc.h"
)

set(tests_header_files
	"include/tests/convolution_t.h"
)

set(tests_source_files
	"tests/convolution_t.cpp"
)

add_library (lpcv STATIC ${source_files} ${header_files})
target_include_directories(lpcv PRIVATE include)
target_link_libraries(lpcv PNG::PNG glfw opengl32 glad::glad)
set_property(TARGET lpcv PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

add_executable (main "src/main.cpp")
target_include_directories(main PRIVATE include)
target_link_libraries(main lpcv)
set_property(TARGET main PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

add_executable (tests ${tests_source_files} ${tests_header_files} ${header_files})	
target_include_directories(tests PRIVATE include)
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain lpcv)
set_property(TARGET tests PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")