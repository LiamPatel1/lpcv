#pragma once

#include"lpcv.h"
#include<expected>

namespace lpcv {
	std::expected<Image, Status> gaussian(const Image& image);
}