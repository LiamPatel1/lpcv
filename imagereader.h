#pragma once
#include <string>
#include<expected>
#include"lpcv.h"


std::expected<lpcv::image, lpcv::Status> loadPNG(std::string fileName);