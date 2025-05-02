#include "lpcv.h"
#include<vector>


lpcv::Image lpcv::greyscale(const lpcv::Image& image) {

	lpcv::ColourType newColourType;
	uint8_t newByteDepth;
	switch (image.getColourType()) {

	case RGB8: newColourType = G8; break;
	case RGB16: newColourType = G16; break;
	case RGBF: newColourType = GF; break;
	case RGBA8: newColourType = GA8; break;
	case RGBA16: newColourType = GA16; break;
	case RGBAF: newColourType = GAF; break;
	case G8:
	case G16:
	case GF:
	case GA8:
	case GA16:
	case GAF:
		return Image(image);
	default:
		throw std::invalid_argument("Unsupported colour space for operation: greyscale");
	}

	uint64_t newDataSize = image.getHeight() * image.getWidth() * image.getByteDepth();
	if (image.hasAlpha()) newDataSize *= 2;

	lpcv::Image newImage(std::vector<unsigned char>(newDataSize), image.getHeight(), image.getWidth(), newColourType, image.getName());

	std::vector<float> newData;
	for (int y = 0; y < image.getHeight(); y++) {
		for (int x = 0; x < image.getWidth(); x++) {
			float total = 0;
			for (int sub = 0; sub < image.getNonAlphaChannelCount(); sub++) {
				total += image.get(y, x, sub);
			}

			total /= image.getNonAlphaChannelCount();
			newImage.write(total, y, x);

			if (newImage.hasAlpha()) {
				newImage.write(image.get(y, x, image.getChannelCount()-1), y, x, 1);
			}
		}
	}
	return newImage;
}

lpcv::Image lpcv::pythag(const lpcv::Image& i1, const lpcv::Image& i2) {


	lpcv::Image newImage(i1);

	for (int y = 0; y < i1.getHeight(); y++) {
		for (int x = 0; x < i1.getWidth(); x++)
			for (int sub = 0; sub < i1.getChannelCount(); sub++) {
				float newVal = std::pow(i1.get(y,x,sub), 2) + std::pow(i2.get(y,x,sub), 2);
				newVal = std::sqrt(newVal);
				newImage.write(newVal, y, x, sub);
			}	
	}
	return newImage;
}

lpcv::Image lpcv::findAngles(const lpcv::Image& ix, const lpcv::Image& iy) {
	if (ix.getData().size() != iy.getData().size()) throw std::invalid_argument("images are not compatible");
	if (ix.getWidth() != iy.getWidth()) throw std::invalid_argument("images are not compatible");
	if (ix.getHeight() != iy.getHeight()) throw std::invalid_argument("images are not compatible");
	if (ix.getChannelCount() != iy.getChannelCount()) throw std::invalid_argument("images are not compatible");
	if (ix.getColourType() != iy.getColourType()) throw std::invalid_argument("images are not compatible");

	ColourType newColourType;
	switch (ix.getColourType())
	{
	case lpcv::RGB8:
	case lpcv::RGB16:
	case lpcv::RGBF:
		newColourType = RGB8;
		break;
	case lpcv::RGBA8:
	case lpcv::RGBA16:
	case lpcv::RGBAF:
		newColourType = RGBA8;
		break;
	case lpcv::G8:
	case lpcv::G16:
	case lpcv::GF:
		newColourType = G8;
		break;
	case lpcv::GA8:
	case lpcv::GA16:
	case lpcv::GAF:
		newColourType = GA8;
		break;
	default:
		throw std::invalid_argument("Unsupported colour space for operation: greyscale");
	}

	uint32_t width = ix.getWidth();
	uint32_t height = ix.getHeight();
	uint8_t channels = ix.getChannelCount();

	uint64_t newSize =  width * height * channels;
	lpcv::Image newImage(std::vector<unsigned char>(newSize), height, width, newColourType, "angles");

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			for (int channel = 0; channel < channels; channel++) {

				float newVal = std::atan2(iy.get(y, x, channel), ix.get(y, x, channel));
				newVal = lpcv::radToDeg(newVal);
				newVal = std::fmod(newVal, 180.0f);
				if (newVal < 0) newVal += 180.0f;

				if (newVal < 22.5 || newVal >= 157.5) newVal = 0;
				else if (newVal < 67.5 && newVal >= 22.5) newVal = 45;
				else if (newVal < 112.5 && newVal >= 67.5) newVal = 90;
				else if (newVal < 157.5 && newVal >= 112.5) newVal = 135; 
				else throw std::invalid_argument("angle out of range");
				                                                  
				newImage.write(newVal, y, x, channel);
			}
		}
	}
	return newImage;
}

lpcv::Image lpcv::edgeSuppression(const lpcv::Image& magnitudes, const lpcv::Image& angles) {
	if (magnitudes.getWidth() != angles.getWidth()) throw std::invalid_argument("images are not compatible");
	if (magnitudes.getHeight() != angles.getHeight()) throw std::invalid_argument("images are not compatible");
	if (magnitudes.getChannelCount() != angles.getChannelCount()) throw std::invalid_argument("images are not compatible");

	uint32_t width = magnitudes.getWidth();
	uint32_t height = magnitudes.getHeight();
	uint8_t channels = magnitudes.getChannelCount();

	Image newImage(magnitudes);

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			for (int sub = 0; sub < channels; sub++) {


				uint8_t angle = angles.get(y, x, sub);
				int dx1 = 0, dy1 = 0;
				int dx2 = 0, dy2 = 0;

				switch (angle) {
				case 0: 
					dx1 = 1; dy1 = 0;
					dx2 = -1; dy2 = 0;
					break;
				case 45:
					dx1 = 1; dy1 = -1;
					dx2 = -1; dy2 = 1;
					break;
				case 90:
					dx1 = 0; dy1 = -1;
					dx2 = 0; dy2 = 1;
					break;
				case 135:
					dx1 = -1; dy1 = -1;
					dx2 = 1; dy2 = 1;
					break;
				default: 
					throw std::invalid_argument("Invalid edge angle");
				}
				
				int x1 = mirrorIndex(x + dx1, width - 1);
				int y1 = mirrorIndex(y + dy1, height - 1);
				int x2 = mirrorIndex(x + dx2, width - 1);
				int y2 = mirrorIndex(y + dy2, height - 1);


				if (magnitudes.get(y, x, sub) < magnitudes.get(y1,x1,sub) 
					|| magnitudes.get(y, x, sub) < magnitudes.get(y2,x2,sub)) {
					newImage.write(0, y, x, sub);
				}
			}
		}
	}

	return newImage;
}

lpcv::Image lpcv::doubleThreshold(const lpcv::Image& image, float low, float high) {

	if (low < 0 || high > 1 || low >= high) throw std::invalid_argument("Invalid bounds");

	lpcv::Image newImage(image);

	float max;
	float min;
	if (newImage.hasFloat()) {
		max = high;
		min = low;
	}
	else if (newImage.getByteDepth() == 1) {
		max = 255 * high;
		min = 255 * low;
	}
	else if (newImage.getByteDepth() == 2) {
		max = 65535 * high;
		min = 65535 * low;
	}
	else throw std::invalid_argument("Invalid image");


	for (int y = 0; y < newImage.getHeight(); y++) {
		for (int x = 0; x < newImage.getWidth(); x++) {
			for (int sub = 0; sub < newImage.getChannelCount(); sub++) {
				float val = newImage.get(y, x, sub);
				if (val < low) {
					newImage.write(0, y, x, sub);
				}


			}
		}
	}
	return newImage;
}
