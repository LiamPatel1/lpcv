set(VCPKG_TARGET_ARCHITECTURE x64)


if(${PORT} MATCHES "qtbase|libpng")
	set(VCPKG_CRT_LINKAGE dynamic)
	set(VCPKG_LIBRARY_LINKAGE dynamic)

elseif(${PORT} MATCHES "catch2")
	set(VCPKG_CRT_LINKAGE dynamic)
	set(VCPKG_LIBRARY_LINKAGE dynamic)

endif()