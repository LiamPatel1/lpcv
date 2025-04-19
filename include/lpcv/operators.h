#pragma once

#include "lpcv/image.h"

namespace lpcv {



	Image greyscale(const Image& image);
	lpcv::Vec pythag(lpcv::Vec& i1, lpcv::Vec& i2);
	lpcv::Vec findAngles(lpcv::Vec& ImgX, lpcv::Vec& ImgY);
		lpcv::Image edgeSuppression(const lpcv::Vec& magnitudes, const lpcv::Vec& angles);



}