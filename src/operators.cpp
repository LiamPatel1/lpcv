#include "lpcv.h"

lpcv::Image lpcv::greyscale(const lpcv::Image& image) {

	lpcv::ColourSpace newColourSpace;
	switch (image.getColourSpace()) {
	case RGB:
		newColourSpace = G;
		break;
	case RGBA:
		newColourSpace = GA;
		break;
	case G:
	case GA:
		return Image(image);
	default:
		throw std::invalid_argument("Unsupported colour space for operation: greyscale");
	}

	Image newImage({}, image.getBitDepth(), image.getHeight(), image.getWidth(), newColourSpace, image.getName());
	std::vector<unsigned char> newData;

	/*
	TODO:

	define byteArray class that inherites std::vector<unsigned char>

	add set value, similar to for images

	this is so a buffer can be defined then the image can be created, so the imges data can be made const
	
	
	*/


	return newImage;




}