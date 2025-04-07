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

		unsigned char* operator()(int y, int x, int channel);


		void appendData(byteArray);
		void appendData(char*, int size);
		uint8_t getColourDepth() const;
		uint64_t getWidth() const;
		uint64_t getHeight() const;
		uint8_t getColourSpace() const;
		uint8_t getChannelCount() const;
		uint64_t getValue(int y, int x, int channel) const;
		uint64_t getBytesPerLine() const;




	};
	int getChannelCount(uint8_t type);
}