#include <catch2/catch_test_macros.hpp>

#include<vector>
#include<expected>
#include"lpcv.h"
#include"internal.h"

TEST_CASE("Mirror Index", "") {

	REQUIRE(mirrorIndex(0, 20) == 0);
	REQUIRE(mirrorIndex(1, 20) == 1);
	REQUIRE(mirrorIndex(-1, 20) == 0);
	REQUIRE(mirrorIndex(-5, 20) == 4);
	REQUIRE(mirrorIndex(-25, 20) == 24);
	REQUIRE(mirrorIndex(50, 50) == 50);
	REQUIRE(mirrorIndex(51, 50) == 49);
	REQUIRE(mirrorIndex(60, 50) == 40);
	REQUIRE(mirrorIndex(0, 0) == 0);
	REQUIRE(mirrorIndex(2, 0) == -2);
	REQUIRE_THROWS(mirrorIndex(-8, -5));
	REQUIRE_THROWS(mirrorIndex(8, -5));
}
