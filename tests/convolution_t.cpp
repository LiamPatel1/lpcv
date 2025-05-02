#include <catch2/catch_test_macros.hpp>

#include<vector>
#include<expected>
#include"lpcv.h"
#include"internal.h"

TEST_CASE("Mirror Index", "") {

	REQUIRE(lpcv::mirrorIndex(0, 20) == 0);
	REQUIRE(lpcv::mirrorIndex(1, 20) == 1);
	REQUIRE(lpcv::mirrorIndex(-1, 20) == 0);
	REQUIRE(lpcv::mirrorIndex(-5, 20) == 4);
	REQUIRE(lpcv::mirrorIndex(-25, 20) == 24);
	REQUIRE(lpcv::mirrorIndex(50, 50) == 50);
	REQUIRE(lpcv::mirrorIndex(51, 50) == 49);
	REQUIRE(lpcv::mirrorIndex(60, 50) == 40);
	REQUIRE(lpcv::mirrorIndex(0, 0) == 0);
	REQUIRE(lpcv::mirrorIndex(2, 0) == -2);
	REQUIRE_THROWS(lpcv::mirrorIndex(-8, -5));
	REQUIRE_THROWS(lpcv::mirrorIndex(8, -5));
}
