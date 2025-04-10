#include"lpcv.h"
#include<expected>
#include<iostream>
#include <random>
#include<cmath>

int mirrorIndex(int i, int bound) {
	if (i < 0) return ~i;
	if (i > bound) return bound - std::abs(bound - i) + 1;
	
	return i;

}


std::expected<lpcv::Image, lpcv::Status> convoluteKernal(const lpcv::Image& image, const lpcv::Kernel kernel) {
	
	lpcv::Image newImage = image;

	int widthFromCentre = (kernel.getWidth() - 1) / 2;
	int heightFromCentre = (kernel.getHeight() - 1) / 2;
	for (int y = 0; y < image.getHeight(); y++) {
		for (int x = 0; x < image.getWidth(); x++) {
			for (int channel = 0; channel < image.getChannelCount(); channel++) {

				float total = 0;
				for (int kernely = -heightFromCentre; kernely <= heightFromCentre; kernely++) {
					for (int kernelx = -widthFromCentre; kernelx <= widthFromCentre; kernelx++) {

						total += kernel.get<float>(kernely + heightFromCentre, kernelx + widthFromCentre) * image.getSubPixel_U64(mirrorIndex(y + kernely, image.getHeight() - 1), mirrorIndex(x + kernelx, image.getWidth() - 1), channel);
					}
				}
				newImage.setValue(total, y, x, channel);
				 
			}

		}
	}
	return newImage;
}



std::expected<lpcv::Image, lpcv::Status> lpcv::gaussian(const Image& image) {

	Kernel kernel({
		{2,4,5,4,2},
		{4,9,12,9,4},
		{5,12,15,12,5},
		{4,9,12,9,4},
		{2,4,5,4,2}
	}); 

	/*Kernel kernel({
		{0.0625,0.125,0.0625},
		{0.125,0.25,0.125},
		{0.0625,0.125,0.0625}
		});*/

	//std::vector<std::vector<float>> kernel = {
	//	{-1,-1,-1},
	//	{-1,8,-1},
	//	{-1,-1,-1}

	//};

	/*lpcv::Kernel kernel({
		{-1,0,1},
		{-2,0,2},
		{-1,0,1}
	});*/
	/*std::vector<std::vector<float>> kernel({
		{-1,0,1},
		{-2,0,2},
		{-1,0,1}
		});*/
	

	kernel.l1Normalise();
	//kernel.standardisationNormalise();


	return convoluteKernal(image, kernel);
	
}

