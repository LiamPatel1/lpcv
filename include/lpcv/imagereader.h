#pragma once
#include <string>
#include<expected>
#include"../lpcv.h"


std::expected<lpcv::Image, lpcv::Status> loadPNG(std::string fileName);