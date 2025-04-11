#pragma once
#include<vector>

namespace lpcv {
	typedef unsigned char uchar;
	typedef std::vector<uchar> byteArray;

	enum Type { TYPE_NONE, TYPE_U8, TYPE_U16, TYPE_U32, TYPE_U64, TYPE_FLOAT };


	enum ColourSpace {
		NONE,
		RGB,
		RGBA,
		G,
		GA
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