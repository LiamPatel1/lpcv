#pragma once

#include"../lpcv.h"
#include"vec.h"
#include<cmath>

namespace lpcv {
	class Image : public Vec {

		ColourSpace colourSpace;

	public:

		Image(std::vector<unsigned char> data, uint8_t bitDepth, uint32_t height, uint32_t width, ColourSpace colourSpace) {

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
			
		const std::vector<unsigned char> getSubPixel(const int i1, const int i2, const int i3) const {
			const unsigned char* addr = &this->data[getByteDepth() * (measurements[2] * (i1 * measurements[1] + i2) + i3)];
			std::vector<unsigned char> data;
			for (int i = 0; i < getByteDepth();i++) {
				data.push_back(addr[i]);
			}
		}
		uint64_t getBytesPerRow() const {
			return getWidth()* getChannelCount()* getByteDepth();
		}

		ColourSpace getColourSpace() const {
			return this->colourSpace;
		}


	};
};