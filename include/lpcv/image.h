#pragma once

#include"lpcv/vec.h"
#include"lpcv/enum.h"
#include"lpcv/misc.h"
#include<cmath>

namespace lpcv {
	class Image : public Vec {

		ColourSpace colourSpace;
		std::string name;

	public:

		Image(std::vector<unsigned char> data, uint8_t bitDepth, uint32_t height, uint32_t width, ColourSpace colourSpace, std::string name = "image")
			: name(name), colourSpace(colourSpace) {
		

			this->type = getImageTypeFromBitDepth(bitDepth);
			this->data = data;
			this->measurements = { height, width, getChannelCount() };
		}

		const uint32_t getHeight() const {
			return measurements[0];
		}

		

		const uint32_t getWidth() const {
			return measurements[1];
		}
		const uint32_t getChannelCount() const {
			switch (this->colourSpace) {
			case RGB:
				return 3;
			case RGBA:
				return 4;
			case G:
				return 1;
			case GA:
				return 2;
			}
			throw std::invalid_argument("colour space invalid");
		}



		const uint32_t getNonAlphaChannelCount() const {
			if (hasAlpha()) 
				return measurements[2] - 1;
			return measurements[2];
		}

		const uint64_t getSubPixel_U64(const int i1, const int i2, const int i3) const {
			uint64_t val = 0;
			std::memcpy(&val, this->at(i1, i2, i3), this->getByteDepth());
			return val;
		}
		const uint64_t getSubPixel_U64(const int i) const {
			uint64_t val = 0;
			std::memcpy(&val, &data[i*getByteDepth()], this->getByteDepth());
			return val;
		}


		const std::vector<unsigned char> getSubPixel_ByteArray(const int i) const {
			const unsigned char* addr = &data[i*getByteDepth()];
			std::vector<unsigned char> subPixelData;
			for (int i = 0; i < getByteDepth();i++) {
				subPixelData.push_back(addr[i]);
			}
			return subPixelData;
		}

		const std::vector<unsigned char> getSubPixel_ByteArray(const int i1, const int i2, const int i3) const {
			const unsigned char* addr = at(i1,i2,i3);
			std::vector<unsigned char> subPixelData;
			for (int i = 0; i < getByteDepth();i++) {
				subPixelData.push_back(addr[i]);
			}
			return subPixelData;
		}

		uint64_t getBytesPerRow() const {
			return getWidth()* getChannelCount()* getByteDepth();
		}

		ColourSpace getColourSpace() const {
			return this->colourSpace;
		}

		const std::string getName() const {
			return this->name;
		}

		uint64_t getPixelCount() const {
			return getHeight() * getWidth();
		}
		uint64_t getSubPixelCount() const {
			return getHeight() * getWidth() * getChannelCount();
		}

		bool hasAlpha() const {
			if (colourSpace == GA || colourSpace == RGBA) 
				return true;
			return false;
		}

		Image expand_bitDepth(uint8_t newBitDepth) {
			if (newBitDepth <= getBitDepth()) return Image(*this);
			if (newBitDepth % 8 != 0 || newBitDepth > 64) throw std::invalid_argument("invalid bitDepth to be expanded to");

			std::vector<unsigned char> newData;
			for (int i = 0; i < getDataSize(); i++) {
				for (int x = 0; x < (newBitDepth / getBitDepth()); x++) {
					newData.push_back(data[i]);
				}
			}
			
			return Image(newData, newBitDepth, getHeight(), getWidth(), getColourSpace());
		}

		Image expand_RGB_RGBA() {
			if (getColourSpace() != RGB) throw std::invalid_argument("Non-RGB image passed into expand_RGB_RGBA");
			std::vector<unsigned char> newData;
			for (int i = 0; i < getSubPixelCount(); i+=3) {
				std::vector<unsigned char> subPixelData;
				subPixelData = getSubPixel_ByteArray(i);
				newData.insert(newData.end(), subPixelData.begin(), subPixelData.end());
				subPixelData = getSubPixel_ByteArray(i+1);
				newData.insert(newData.end(), subPixelData.begin(), subPixelData.end());
				subPixelData = getSubPixel_ByteArray(i+2);
				newData.insert(newData.end(), subPixelData.begin(), subPixelData.end());

				std::vector<unsigned char> alphaChannel(getByteDepth(), 255);
				newData.insert(newData.end(), alphaChannel.begin(), alphaChannel.end());
			}

			return Image(newData, getBitDepth(), getHeight(), getWidth(), RGBA);

		}

		Image expand_G_RGB__GA_RGBA() {
			if (getColourSpace() != G && getColourSpace() != GA) throw std::invalid_argument("Non-grey image passed into expand_G_RGB__GA_RGBA");


			std::vector<unsigned char> newData;

			for (int i = 0; i < getSubPixelCount(); i+= getChannelCount()) {
				std::vector<unsigned char> pixelData = getSubPixel_ByteArray(i);
				newData.insert(newData.end(), pixelData.begin(), pixelData.end());
				newData.insert(newData.end(), pixelData.begin(), pixelData.end());
				newData.insert(newData.end(), pixelData.begin(), pixelData.end());


				if (hasAlpha()) {
					std::vector<unsigned char> pixelData = getSubPixel_ByteArray(i+1);
					newData.insert(newData.end(), pixelData.begin(), pixelData.end());
				}

			}
			ColourSpace newSpace = lpcv::RGB;
			if (hasAlpha()) {
				newSpace = RGBA;

			}
			return Image(newData, getBitDepth(), getHeight(), getWidth(), newSpace, getName());
		}

		


	};
};