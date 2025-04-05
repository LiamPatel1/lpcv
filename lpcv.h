#pragma once

#include<vector>
#include<cstddef>

namespace lpcv {
	typedef unsigned char uchar;
	typedef std::vector<uchar> byteArray;

	enum ColourSpace {
		NONE,
		RGB,
		RGBA,
		G,
		GA
	};

	enum Status {
		SUCCESS,
		ERROR_OPEN_FILE,
		ERROR_NOT_PNG,
		ERROR_PNG_READ_CREATION,
		ERROR_PNG_INFO_CREATION,
		ERROR_UNSUPPORTED_COLOUR_SPACE,
		ERROR_SHOW_INVALID_FORMAT
	};

	class Size {
		public:
			uint32_t x;
			uint32_t y;
			Size(uint32_t x, uint32_t y);
	};
}