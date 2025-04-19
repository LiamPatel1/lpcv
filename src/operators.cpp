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


lpcv::Vec lpcv::pythag(lpcv::Vec& i1, lpcv::Vec& i2) {

	if (i1.getDataSize() != i2.getDataSize()) throw std::invalid_argument("images have different sizes");


	lpcv::Vec newImage(i1);

	for (int y = 0; y < i1.getMeasurements()[0]; y++) {
		for (int x = 0; x < i1.getMeasurements()[1]; x++)
			for (int sub = 0; sub < i1.getMeasurements()[2]; sub++) {
				float newVal = std::pow(i1.getValue_FLOAT(y,x,sub), 2) + std::pow(i2.getValue_FLOAT(y,x,sub), 2);
				newVal = std::sqrt(newVal);
				newImage.setValue(newVal, y, x, sub);
			}
	}
	return newImage;

}

lpcv::Vec lpcv::findAngles(lpcv::Vec& ImgX, lpcv::Vec& ImgY) {
	if (ImgX.getDataSize() != ImgY.getDataSize()) throw std::invalid_argument("images have different sizes");

	lpcv::Vec angles(std::vector<unsigned char>(ImgX.getDataSize()), TYPE_U8, ImgX.getMeasurements());

	for (int y = 0; y < ImgX.getMeasurements()[0]; y++) {
		for (int x = 0; x < ImgX.getMeasurements()[1]; x++) {
			for (int sub = 0; sub < ImgX.getMeasurements()[2]; sub++) {
				float newVal = std::atan2((float)(ImgY.getValue_FLOAT(y, x, sub) - 127.5), (float)(ImgX.getValue_FLOAT(y, x, sub) - 127.5));
				newVal = lpcv::radToDeg(newVal);
				if (newVal < 0) {
					newVal += 360;
				}

				if (newVal < 22.5 || newVal >= 157.5) newVal = 0;
				else if (newVal < 67.5 && newVal >= 22.5) newVal = 45;
				else if (newVal < 112.5 && newVal >= 67.5) newVal = 90;
				else if (newVal < 157.5 && newVal >= 112.5) { newVal = 135; }
				else throw std::invalid_argument("angle out of range");

				angles.setValue(newVal, y, x, sub);
			}
		}
	}
	return angles;
}

lpcv::Image lpcv::edgeSuppression(const lpcv::Vec& magnitudes, const lpcv::Vec& angles) {

	
	for (int y = 0; y < magnitudes.getHeight(); y++) {
		for (int x = 0; x < magnitudes.getWidth(); x++) {
			for (int sub = 0; sub < magnitudes.getChannelCount(); sub++) {
				int xoffset = 0;
				switch (angles.get<uint8_t>(y, x, sub)) {
				case 0: 
					continue;
				case 45:
					 xoffset = 1;
					break;
				case 90:
					 xoffset = 0;
					break;
				case 135:
					 xoffset = -1;
					break;
				default: throw std::invalid_argument("Invalid edge angle");
				}
				
				if (magnitudes.getSubPixel_U64(y, x, sub) < magnitudes.getSubPixel_U64(mirrorIndex(y + 1, magnitudes.getHeight()), mirrorIndex(x + xoffset, magnitudes.getWidth()), sub) 
					|| magnitudes.getSubPixel_U64(y, x, sub) < magnitudes.getSubPixel_U64(mirrorIndex(y - 1, magnitudes.getHeight()), mirrorIndex(x - xoffset, magnitudes.getWidth()), sub)) {
					newImage.setValue(0, y, x, sub);
				}
				



			}
		}
	}

	return newImage;
}