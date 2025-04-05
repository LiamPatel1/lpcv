#pragma once
#include"lpcv.h"

namespace lpcv {

	class Image {

		int8_t colourSpace;
		uint32_t width;
		uint32_t height;
		uint8_t colourDepth;

	public:
		Image(byteArray* data, uint8_t colourSpace, uint8_t colourDepth, uint64_t width, uint64_t height, bool copyData=false);
		Image(uint8_t colourSpace, uint8_t colourDepth, uint64_t width, uint64_t height);

		Image();

		byteArray* data;

		void appendData(byteArray);
		void appendData(char*, int size);
		uint8_t getColourDepth();
		uint64_t getWidth();
		uint64_t getHeight();
		uint8_t getColourSpace();


	};
	int getChannelCount(uint8_t type);
}