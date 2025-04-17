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

}