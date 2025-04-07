#pragma once

#include"lpcv.h"
#include"lpcv/image.h"
#include<expected>

namespace lpcv {
	std::expected<void, lpcv::Status> gaussian(Image &image, Size size);
}