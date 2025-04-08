#include"lpcv/gaussian.h"
#include"lpcv.h"
#include"lpcv/viewer.h"
#include<expected>
#include<iostream>
#include<cmath>

int mirrorIndex(int i, int bound) {
	if (i < 0) return ~i;
	if (i > bound) return bound - std::abs(bound - i) + 1;
	
	return i;

}

std::expected<int, lpcv::Status> applyKernal(const lpcv::Image& image, std::vector<std::vector<float>> kernel, uint32_t x, uint32_t y, uint8_t channel) {
	int index = y * image.getWidth() + x * image.getChannelCount() + channel;
	int kernal_height = kernel.size(); 
	int kernal_width = kernel[0].size(); 
	if (kernal_height % 2 == 0 || kernal_width % 2 == 0) return std::unexpected(lpcv::ERROR_KERNAL_EVEN); 
	int width_from_centre = (kernal_width-1)/2; 
	int height_from_centre = (kernal_height - 1) / 2; 
	float total = 0;

	for (int kernely = -height_from_centre; kernely < height_from_centre; kernely++) {
		for (int kernelx = -width_from_centre; kernelx < width_from_centre; kernelx++) {
			total += (kernel[kernely+height_from_centre][kernelx+width_from_centre]) * image(mirrorIndex(y+kernely, image.getHeight()-1), mirrorIndex(x+kernelx, image.getWidth()-1), channel);
		}
	}
	return (uint8_t)std::round(total);

}

void normaliseKernel(std::vector<std::vector<float>>& kernel) {
	int sum = 0;
	for (int y = 0; y < kernel.size(); y++) {
		for (int x = 0; x < kernel[y].size(); x++) {
			sum += kernel[y][x];
		}
	}
	for (int y = 0; y < kernel.size(); y++) {
		for (int x = 0; x < kernel[y].size(); x++) {
			kernel[y][x] /= sum;
		}
	}

}


std::expected<void, lpcv::Status> lpcv::gaussian(Image& image, Size size) {
	lpcv::Image* originalImage = new lpcv::Image(image);

	std::vector<std::vector<float>> kernel = {
		{2,4,5,4,2},
		{4,9,12,9,4},
		{5,12,15,12,5},
		{4,9,12,9,4},
		{2,4,5,4,2}
	};
	normaliseKernel(kernel);

	for (int y = 0; y < image.getHeight(); y++) {
		for (int x = 0; x < image.getWidth(); x++) {
			for (int channel = 0; channel < image.getChannelCount(); channel++) {
				auto newvalue = applyKernal(*originalImage, kernel, x, y, channel);
				if (!newvalue) return std::unexpected(newvalue.error());
				image(y, x, channel) = *newvalue;
			}

		}
	}
	
}

