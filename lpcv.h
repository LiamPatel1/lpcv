#pragma once

#include<vector>
#include<cstddef>
#include<string>

namespace lpcv {

	typedef std::vector<std::byte> byteArray;

	enum ColourSpace {
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
		ERROR_PNG_INFO_CREATION
	};

	class image {

		int8_t colourSpace;
		uint64_t width;
		uint64_t height;
		uint8_t colourDepth;
		byteArray data;

		public:
			image(byteArray data, uint8_t colourSpace, uint8_t colourDepth, uint64_t width, uint64_t height);
			image();


	};
}