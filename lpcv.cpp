#include "lpcv.h"
#include <cstddef>
#include<vector>
#include <iterator>

int lpcv::channelCount(int space) {
	switch (space) {
	case RGB: return 3;
	case RGBA: return 4;
	case G: return 1;
	case GA: return 2;
	default: return 0;

	}
}

lpcv::image::image(byteArray data, uint8_t colourSpace, uint8_t colourDepth, uint64_t width, uint64_t height)
	: data(data), colourSpace(colourSpace), colourDepth(colourDepth), width(width), height(height) {}

lpcv::image::image() 
	: data({}), colourSpace(ColourSpace::NONE), colourDepth(0), width(0), height(0) {}

void lpcv::image::appendData(byteArray appendedData) {
	data.insert(data.end(), appendedData.begin(), appendedData.end());
}

void lpcv::image::appendData(char* appendedData, int size) {
	byteArray convertedData = {};
	for (int i = 0; i < size; i++) {
		convertedData.push_back((std::byte)appendedData[i]);
	}
	lpcv::image::appendData(convertedData);
}

uint8_t lpcv::getColourDepth() {
	return this->colourDepth;
}
uint64_t lpcv::getWidth() {
	return this->width;
}
uint64_t lpcv::getHeight() {
	return this->height;
}
uint8_t lpcv::getColourSpace() {
	return this->colourSpace;
}
lpcv::byteArray lpcv::getData() {
	return this->data;
}