#include"lpcv/image.h"
#include<iostream>
#include<cmath>
#include<algorithm>

//class Rows {
//	Rows(lpcv::Image& image, unsigned char* data) : image(image), data(data) {}
//	lpcv::Image& image;
//	unsigned char* data;
//	unsigned char* operator[](size_t i){
//		return 
//	}
//};
//
//






lpcv::Image::Image(byteArray* data, uint8_t colourSpace, uint8_t colourDepth, uint64_t width, uint64_t height, bool copyData)
	: data(data), colourSpace(colourSpace), colourDepth(colourDepth), width(width), height(height) {

	if (copyData) {
		data = new byteArray(*data);
	}
	
	
}

lpcv::Image::Image()
	: data(new byteArray({})), colourSpace(ColourSpace::NONE), colourDepth(0), width(0), height(0) {
}
lpcv::Image::Image(uint8_t colourSpace, uint8_t colourDepth, uint64_t width, uint64_t height)
	: data(new byteArray({})), colourSpace(colourSpace), colourDepth(colourDepth), width(width), height(height) {
}
lpcv::Image::Image(const lpcv::Image& other)
	: data(new byteArray(*other.data)), colourSpace(other.colourSpace), colourDepth(other.colourDepth), width(other.width), height(other.height) {}


void lpcv::Image::appendData(byteArray appendedData) {
	data->insert(data->end(), appendedData.begin(), appendedData.end());
}
void lpcv::Image::appendData(char* appendedData, int size) {
	byteArray convertedData = {};
	for (int i = 0; i < size; i++) {
		convertedData.push_back(appendedData[i]);
	}
	lpcv::Image::appendData(convertedData);
}

unsigned char& lpcv::Image::operator()(uint32_t y, uint32_t x, uint8_t channel) const {
	return (*data)[(y * getWidth() + x) * getChannelCount() + channel];
}

lpcv::Image& lpcv::Image::operator= (Image other)
{
	data = other.data;
	colourDepth = other.colourDepth;
	colourSpace = other.colourSpace;
	width = other.width;
	height = other.height;
	return *this;
}


uint8_t lpcv::Image::getColourDepth() const {
	return this->colourDepth;
}
uint64_t lpcv::Image::getWidth() const {
	return this->width;
}
uint64_t lpcv::Image::getHeight() const {
	return this->height;
}
uint8_t lpcv::Image::getColourSpace() const {
	return this->colourSpace;
}
uint8_t lpcv::Image::getChannelCount() const {
	return lpcv::getChannelCount(this->getColourSpace());
}
uint64_t lpcv::Image::getBytesPerLine() const {
	return this->getWidth() * this->getChannelCount() * std::ceil(this->getColourDepth()/8);
}
uint64_t lpcv::Image::getValue(int y, int x, int channel) const {
	return (*data)[y * getBytesPerLine() + x*getChannelCount()* channel];
}


int lpcv::getChannelCount(uint8_t type) {
	switch (type) {
	case lpcv::RGB: return 3;
	case lpcv::RGBA: return 4;
	case lpcv::G: return 1;
	case lpcv::GA: return 2;
	default: return 0;

	}
}