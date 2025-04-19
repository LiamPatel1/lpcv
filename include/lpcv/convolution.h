#pragma once

#include"lpcv/enum.h"
#include"lpcv/image.h"
#include"lpcv/vec.h"
#include<expected>

namespace lpcv {
	Image convoluteKernal(const Image& image, const Kernel kernel);
	Vec convoluteKernal(const Vec& image, const Kernel kernel);

	Image gaussian(const Image& image);
	Image canny(const Image& image);



	}