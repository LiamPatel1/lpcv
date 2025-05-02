#pragma once
#include<vector>

namespace lpcv {
		
	enum ColourType {
		NONE,
		RGB8,
		RGB16,
		RGBF,
		RGBA8,
		RGBA16,
		RGBAF,
		G8,
		G16,
		GF,
		GA8,
		GA16,
		GAF
	};

	enum Dimension {
		DIMENSION_X,
		DIMENSION_Y
	};

	enum Status {
		SUCCESS,
		ERROR_OPEN_FILE,
		ERROR_NOT_PNG,
		ERROR_PNG_READ_CREATION,
		ERROR_PNG_INFO_CREATION,
		ERROR_UNSUPPORTED_COLOUR_SPACE,
		ERROR_SHOW_INVALID_FORMAT,
		ERROR_KERNAL_EVEN
	};

}