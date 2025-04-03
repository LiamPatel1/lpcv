#pragma once

#include<vector>
#include<cstddef>
#include<string>

namespace lpcv {

	typedef std::vector<std::byte> byteArray;

	enum ColourSpace {
		NONE,
		RGB,
		RGBA,
		G,
		GA
	};

	int channelCount(int space);
	

	enum Status {
		SUCCESS,
		ERROR_OPEN_FILE,
		ERROR_NOT_PNG,
		ERROR_PNG_READ_CREATION,
		ERROR_PNG_INFO_CREATION,
		ERROR_UNSUPPORTED_COLOUR_SPACE
	};

	class image {

		int8_t colourSpace;
		uint32_t width;
		uint32_t height;
		uint8_t colourDepth;
		lpcv::byteArray data;

		public:
			image(byteArray data, uint8_t colourSpace, uint8_t colourDepth, uint64_t width, uint64_t height);
			image();
			void appendData(byteArray);
			void appendData(char*, int size);
			uint8_t getColourDepth();
			uint64_t getWidth();
			uint64_t getHeight();
			uint8_t getColourSpace();
			lpcv::byteArray getData();


	};


}