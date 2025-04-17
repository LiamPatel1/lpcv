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

	uint64_t newSize = image.getHeight() * image.getWidth() * image.getByteDepth();
	if (newColourSpace == GA) newSize *= 2;

	Image newImage(std::vector<unsigned char>(newSize), image.getBitDepth(), image.getHeight(), image.getWidth(), newColourSpace, image.getName());

	for (int y = 0; y < image.getHeight(); y++) {
		for (int x = 0; x < image.getWidth(); x++) {
			float total = 0;
			for (int sub = 0; sub < image.getNonAlphaChannelCount(); sub++) {
				total += image.getSubPixel_U64(y, x, sub);
			}
			newImage.setValue(total / 3, y, x, 0);
			if (image.hasAlpha()) {
				newImage.setValue(image.getSubPixel_U64(y, x, image.getChannelCount()-1),y,x, 1);
			}
		}
	}


	return newImage;


}


lpcv::Image lpcv::pythag(lpcv::Image& i1, lpcv::Image& i2) {

	if (i1.getDataSize() != i2.getDataSize()) throw std::invalid_argument("images have different sizes");
	if (i1.getHeight() != i2.getHeight()) throw std::invalid_argument("images have different sizes");
	if (i1.getWidth() != i2.getWidth()) throw std::invalid_argument("images have different sizes");

	lpcv::Image newImage(i1);

	for (int y = 0; y < i1.getHeight(); y++) {
		for (int x = 0; x < i1.getWidth(); x++)
			for (int sub = 0; sub < i1.getChannelCount(); sub++) {
				float newVal = std::pow(i1.getSubPixel_U64(y,x,sub), 2) + std::pow(i2.getSubPixel_U64(y,x,sub), 2);
				newVal = std::sqrt(newVal);
				newImage.setValue(newVal, y, x, sub);
			}
	}
	return newImage;

}