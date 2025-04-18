#pragma once

#include"lpcv/enum.h"
#include <numbers>

namespace lpcv {
	inline lpcv::Type getImageTypeFromBitDepth(int bitDepth) {
		lpcv::Type newType;
		if (bitDepth == 8) {
			return lpcv::TYPE_U8;
		}
		else if (bitDepth == 16) {
			return lpcv::TYPE_U16;
		}
		else {
			throw std::invalid_argument("unsupported bitdepth");
		}
		return lpcv::TYPE_NONE;

	}


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