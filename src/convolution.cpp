#include"lpcv.h"
#include<expected>
#include<iostream>
#include <random>
#include<cmath>
#include <cassert>


lpcv::Image lpcv::convoluteKernal(const lpcv::Image& image, const lpcv::Kernel& kernel) {
	
	lpcv::Image newImage = image;

	int widthFromCentre = (kernel.getWidth() - 1) / 2;
	int heightFromCentre = (kernel.getHeight() - 1) / 2;
	for (int y = 0; y < image.getHeight(); y++) {
		for (int x = 0; x < image.getWidth(); x++) {
			for (int channel = 0; channel < image.getChannelCount(); channel++) {
				long double total = 0;
				for (int kernely = -heightFromCentre; kernely <= heightFromCentre; kernely++) {
					for (int kernelx = -widthFromCentre; kernelx <= widthFromCentre; kernelx++) {
			
						total += kernel.get(kernely + heightFromCentre, kernelx + widthFromCentre) * image.get(mirrorIndex(y + kernely, image.getHeight() - 1), mirrorIndex(x + kernelx, image.getWidth() - 1), channel);
					}
				}
				newImage.write(total, y, x, channel);
				 
			}

		}
	}
	return newImage;
}


lpcv::Image lpcv::gaussian(const Image& image , double sigma) {

	int32_t length = 5;
	Kernel kernel(length); 

	int32_t fromCentre = (length - 1) / 2;

	double sigma2 = std::pow(sigma, 2);
	
	for (int32_t y = 0; y < length; y++) {
		for (int32_t x = 0; x < length; x++) {
			kernel.write(
				std::pow(std::numbers::e, -(
					(std::pow(x- fromCentre, 2) + std::pow(y- fromCentre, 2))
					/
					(2 * sigma2)
					))
				/
				(2 * std::numbers::pi * sigma2)
				, y, x);
		}

	}
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


	

	const Image noAlpha = image.removeAlpha();
	const Image grey = greyscale(noAlpha);
	const Image blur = gaussian(grey, 0.6);
	const Image toF = blur.toFLOAT();
	const Image newImagex = convoluteKernal(toF, x);
	const Image newImagey = convoluteKernal(toF, y);
	const Image magnitudes = lpcv::pythag(newImagex, newImagey);
	const Image angles = findAngles(newImagex, newImagey);
	const Image suppressed = edgeSuppression(magnitudes, angles);
	const Image thresh = doubleThreshold(suppressed, 0.1, 0.35);



	display(image, "Normal");
	display(grey, "Grey");
	display(blur, "Blur");
	display(magnitudes, "Magnitudes");
	display(angles, "Angles");
	display(suppressed, "Suppressed");
	display(thresh, "Threshold");

	return thresh;

}
