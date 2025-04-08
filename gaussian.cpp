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

std::vector<std::vector<float>> normalisedKernel(const std::vector<std::vector<float>>& kernel) {
	int sum = 0;
	for (int y = 0; y < kernel.size(); y++) {
		for (int x = 0; x < kernel[y].size(); x++) {
			sum += kernel[y][x];
		}
	}
	std::vector<std::vector<float>> newKernel = kernel;

	for (int y = 0; y < kernel.size(); y++) {
		for (int x = 0; x < kernel[y].size(); x++) {
			newKernel[y][x] /= sum;
		}
	}
	return newKernel;

}


std::expected<lpcv::Image, lpcv::Status> convoluteKernal(const lpcv::Image& image, const std::vector<std::vector<float>>& kernel) {
	
	lpcv::Image newImage(image);
	const std::vector<std::vector<float>> nKernel = normalisedKernel(kernel);

	int widthFromCentre = (nKernel[0].size() - 1) / 2;
	int heightFromCentre = (nKernel.size() - 1) / 2;
	for (int y = 0; y < image.getHeight(); y++) {
		for (int x = 0; x < image.getWidth(); x++) {
			for (int channel = 0; channel < image.getChannelCount(); channel++) {

				float total = 0;
				for (int kernely = -heightFromCentre; kernely < heightFromCentre; kernely++) {
					for (int kernelx = -widthFromCentre; kernelx < widthFromCentre; kernelx++) {
						total += (nKernel[kernely + heightFromCentre][kernelx + widthFromCentre]) * image(mirrorIndex(y + kernely, image.getHeight() - 1), mirrorIndex(x + kernelx, image.getWidth() - 1), channel);
					}
				}
				newImage(y, x, channel) = (uint8_t)std::round(total);
			}

		}
	}
	return newImage;
}



std::expected<lpcv::Image, lpcv::Status> lpcv::gaussian(const Image& image) {

	std::vector<std::vector<float>> kernel = {
		{2,4,5,4,2},
		{4,9,12,9,4},
		{5,12,15,12,5},
		{4,9,12,9,4},
		{2,4,5,4,2}
	};

	
	return convoluteKernal(image, kernel);
}

