#include "lpcv.h"


lpcv::image::image(byteArray data, uint8_t colourSpace, uint8_t colourDepth, uint64_t width, uint64_t height) {
	this->data = data;
	this->colourSpace = colourSpace;
	this->colourDepth = colourDepth;

	this->width = width;
	this->height = height;


}

lpcv::image::image() {};

