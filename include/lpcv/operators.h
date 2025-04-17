#pragma once

#include "lpcv/image.h"

namespace lpcv {



	Image greyscale(const Image& image);
	Image pythag(Image& i1, Image& i2);
	lpcv::Vec findAngles(lpcv::Image& i1, lpcv::Image& i2);




}