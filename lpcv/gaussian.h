#pragma once

#include"lpcv.h"
#include"lpcv/image.h"
#include<expected>

namespace lpcv {
	std::expected<lpcv::Image, lpcv::Status> gaussian(const Image& image);
}