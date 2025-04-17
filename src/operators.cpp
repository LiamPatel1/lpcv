#include "lpcv.h"
#include<vector>

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

lpcv::Vec lpcv::findAngles(lpcv::Image& ImgX, lpcv::Image& ImgY) {
	if (ImgX.getDataSize() != ImgY.getDataSize()) throw std::invalid_argument("images have different sizes");
	if (ImgX.getHeight() != ImgY.getHeight()) throw std::invalid_argument("images have different sizes");
	if (ImgX.getWidth() != ImgY.getWidth()) throw std::invalid_argument("images have different sizes");

	lpcv::Vec angles(std::vector<unsigned char>(ImgX.getSubPixelCount() * 1), TYPE_U8, { ImgX.getHeight(), ImgX.getWidth(), ImgX.getChannelCount() });

	for (int y = 0; y < ImgX.getHeight(); y++) {
		for (int x = 0; x < ImgX.getWidth(); x++)
			for (int sub = 0; sub < ImgX.getChannelCount(); sub++) {
				float newVal = std::atan2(ImgY.getSubPixel_U64(y, x, sub), ImgX.getSubPixel_U64(y, x, sub));
				newVal = lpcv::radToDeg(newVal);
				if (newVal < 22.5 || newVal >= 157.5) newVal = 0;
				else if (newVal < 67.5 && newVal >= 22.5) newVal = 45;
				else if (newVal < 112.5 && newVal >= 67.5) newVal = 90;
				else if (newVal < 157.5 && newVal >= 112.5) newVal = 135;
				else throw std::invalid_argument("angle out of range");

				angles.setValue(newVal, y, x, sub);
			}
	}
	return angles;
}