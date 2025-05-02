#pragma once

#include"lpcv/image.h"
#include <cstdint>
#include <iostream>

namespace lpcv {

    void display(const Image& originalImage, std::string title = "_");

}