#pragma once

#include <numbers>
#include<stdexcept>

namespace lpcv {
	

	inline double radToDeg(double rad) {
		return rad * (180 / std::numbers::pi);
	}

	inline double DegToRad(double deg) {
		return deg * (std::numbers::pi / 180);
	}

	inline int mirrorIndex(int i, int bound) {
		if (bound < 0) throw std::invalid_argument("Negative bound");
		if (i < 0) return ~i;
		if (i > bound) return bound - std::abs(bound - i);

		return i;
	}


}