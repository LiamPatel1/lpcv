#pragma once

#include"lpcv/enum.h"
#include"lpcv/image.h"
#include<expected>

namespace lpcv {
	std::expected<Image, Status> gaussian(const Image& image);
	int mirrorIndex(int i, int bound);
}