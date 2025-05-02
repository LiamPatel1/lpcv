#pragma once

#include"lpcv/enum.h"
#include"lpcv/image.h"
#include<expected>

namespace lpcv {
	Image convoluteKernal(const Image& image, const Kernel& kernel);

	Image gaussian(const Image& image, double sigma = 1.0);
	Image canny(const Image& image);



	}