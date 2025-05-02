#pragma once

#include "lpcv/image.h"

namespace lpcv {


	/*!
	 * @brief Converts to greyscale by finding the mean of all channels, except the alpha channel which is ignored.
	 */
	Image greyscale(const Image& image);


	/*!
	 * @brief applies Pythagorean theorem to find image gradients.
	 */
	lpcv::Image pythag(const lpcv::Image& i1, const lpcv::Image& i2);


	lpcv::Image findAngles(const lpcv::Image& ImgX, const lpcv::Image& ImgY);


	lpcv::Image edgeSuppression(const lpcv::Image& magnitudes, const lpcv::Image& angles);

	lpcv::Image doubleThreshold(const lpcv::Image& image, float low, float high);

}