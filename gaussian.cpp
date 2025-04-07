#include"lpcv/gaussian.h"
#include"lpcv.h"
#include"lpcv/viewer.h"
#include<expected>
#include<cmath>

int absIndex(int i, int bound) {
	if (i < 0) return ~i;
	if (i > bound) return bound - std::abs(bound - i) + 1;
	
	return i;

}

std::expected<int, lpcv::Status> applyKernal(const lpcv::Image& image, const std::vector<lpcv::byteArray>& kernal, uint32_t x, uint32_t y, uint8_t channel) {
	int index = y * image.getWidth() + x * image.getChannelCount() + channel;
	int kernal_height = kernal.size();
	int kernal_width = kernal[0].size();
	if (kernal_height % 2 == 0 || kernal_width % 2 == 0) return std::unexpected(lpcv::ERROR_KERNAL_EVEN); 
	int width_from_centre = (kernal_width-1)/2;
	int height_from_centre = (kernal_height - 1) / 2;
	int total = 0;

	for (int kernaly = 0; kernaly < kernal_height; kernaly++) {
		for (int kernalx = 0; kernalx < kernal_width; kernalx++) {
			total += 
				kernal[kernaly][kernalx] * (*image.data)[(y + absIndex(kernal_height - kernaly, image.getHeight())) * image.getWidth() 
				+ (x + absIndex(kernal_width - kernalx, image.getWidth())) * image.getChannelCount() 
				+ channel];
		}
	}
	return total;

}

std::expected<void, lpcv::Status> lpcv::gaussian(Image& image, Size size) {
	std::vector<byteArray> const kernal = { {0,1,0},{0,2,0},{0,3,0} };
	for (int y = 0; y < image.getHeight(); y++) {
		for (int x = 0; x < image.getWidth(); x++) {
			for (int channel = 0; channel < image.getChannelCount(); channel++) {
				auto newvalue = applyKernal(image, kernal, x, y, channel);
				if (!newvalue) return std::unexpected(newvalue.error());
				image(y, x, channel) = *newvalue;
			}
		}
	}
}