#include"lpcv.h"
#include<expected>
#include<iostream>
#include <random>
#include<cmath>



lpcv::Image lpcv::convoluteKernal(const lpcv::Image& image, const lpcv::Kernel kernel) {
	
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



lpcv::Image lpcv::gaussian(const Image& image) {

	Kernel kernel({
		{2,4,5,4,2},
		{4,9,12,9,4},
		{5,12,15,12,5},
		{4,9,12,9,4},
		{2,4,5,4,2}
	}); 

	kernel.l1Normalise();

	return convoluteKernal(image, kernel);
	
}

lpcv::Image lpcv::canny(const Image& image) {

	Kernel x({
		{-1, 0, 1},
		{-2, 0, 2},
		{-1, 0, 1}
		});

	Kernel y({
		{-1, -2, -1},
		{ 0,  0,  0},
		{ 1,  2,  1}
		});


	Image grey = greyscale(image);
	grey = gaussian(grey);
	Image newImagex = convoluteKernal(grey, x);
	Image newImagey = convoluteKernal(grey, y);
	Image magnitudes = lpcv::pythag(newImagex, newImagey);
	auto angles = findAngles(newImagex, newImagey);
	auto newImage = edgeSuppression(magnitudes, angles);

	display(grey);
	display(magnitudes);
	display(newImage);

	return newImage;

}
