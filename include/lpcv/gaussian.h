#pragma once

#include"../lpcv.h"
#include"lpcv/image.h"
#include<expected>

namespace lpcv {
	std::expected<Image, Status> gaussian(const Image& image);
}