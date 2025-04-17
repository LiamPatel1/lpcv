#pragma once

#include"lpcv/enum.h"
#include"lpcv/image.h"
#include<expected>

namespace lpcv {
	Image convoluteKernal(const lpcv::Image& image, const lpcv::Kernel kernel);

	Image gaussian(const Image& image);
	Image sobel(const Image& image);


	}