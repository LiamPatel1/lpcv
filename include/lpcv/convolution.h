#pragma once

#include"lpcv/enum.h"
#include"lpcv/image.h"
#include<expected>

namespace lpcv {
	std::expected<lpcv::Image, lpcv::Status> convoluteKernal(const lpcv::Image& image, const lpcv::Kernel kernel);

	std::expected<Image, Status> gaussian(const Image& image);
	std::expected<lpcv::Image, lpcv::Status> sobel(const Image& image);


	}