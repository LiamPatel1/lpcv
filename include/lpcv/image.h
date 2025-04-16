#pragma once

#include"lpcv/vec.h"
#include"lpcv/enum.h"
#include<cmath>

namespace lpcv {
	class Image : public Vec {

		ColourSpace colourSpace;
		std::string name;

	public:

		Image(std::vector<unsigned char> data, uint8_t bitDepth, uint32_t height, uint32_t width, ColourSpace colourSpace, std::string name="image") {
			this->name = name;

			this->colourSpace = colourSpace;

			if (bitDepth <= 8) this->type = TYPE_U8;
			else if (bitDepth <= 16) this->type = TYPE_U16;
			else if (bitDepth <= 32) this->type = TYPE_U32;
			else if (bitDepth <= 64) this->type = TYPE_U64;

			uint32_t channelCount;
			switch (this->colourSpace) {
			case RGB: 
				channelCount = 3;
				break;
			case RGBA: 
				channelCount = 4;
				break;
			case G:
				channelCount = 1;
				break;
			case GA: 
				channelCount = 2;
				break;
			default: channelCount = 0;
			}

			this->measurements = { height, width, channelCount };


			this->data = data;
		}

		const uint32_t getHeight() const {
			return measurements[0];
		}
		const uint32_t getWidth() const {
			return measurements[1];
		}
		const uint32_t getChannelCount() const {
			return measurements[2];
		}
		const uint32_t getNonAlphaChannelCount() const {
			if (hasAplha()) 
				return measurements[2] - 1;
			return measurements[2];
		}

		const uint64_t getSubPixel_U64(const int i1, const int i2 = 0, const int i3 = 0) const {
			uint64_t val = 0;
			std::memcpy(&val, this->at(i1, i2, i3), this->getByteDepth());
			return val;
		}

	

		void setValue(float value, int i1, int i2, int i3) {

			switch (type) {
				case TYPE_FLOAT: {
					memcpy(this->at(i1, i2, i3), (&value), 4);
					break;
				}
				case TYPE_U8: {
					const float clamped = std::clamp(std::round(value), 0.0f, 255.0f);
					uint8_t newval = static_cast<uint8_t>(clamped);		
					unsigned char* dfgfd = this->at(i1, i2, i3);
					memcpy(this->at(i1, i2, i3), &newval, 1);
					break;
				}
				case TYPE_U16: {
					const float clamped = std::clamp(std::round(value), 0.0f, 65535.0f);
					uint16_t newval = static_cast<uint16_t>(clamped);
					memcpy(this->at(i1, i2, i3), &newval, 2);
					break;
				}
				case TYPE_U32: {
					const float clamped = std::clamp(std::round(value),
						0.0f,
						static_cast<float>(std::numeric_limits<uint32_t>::max())
					);
					uint32_t newval = static_cast<uint32_t>(clamped);
					memcpy(this->at(i1, i2, i3), &newval, 4);
					break;
				}
				case TYPE_U64: {
					const double clamped = std::clamp(
						static_cast<double>(std::round(value)),
						0.0,
						static_cast<double>(std::numeric_limits<uint64_t>::max())
					);
					uint64_t newval = static_cast<uint64_t>(clamped);
					memcpy(this->at(i1, i2, i3), &newval, 8);
					break;
				}

			}

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

		bool hasAplha() const {
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

	};
};