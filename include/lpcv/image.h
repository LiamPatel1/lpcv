#pragma once

#include"lpcv/enum.h"
#include"lpcv/misc.h"
#include<cmath>
#include<algorithm>
#include<string>
#include<variant>
#include<stdexcept>
 

namespace lpcv {

	class Image {

	protected:
		std::string name = "";
		std::vector<unsigned char> data;
		uint32_t height;
		uint32_t width;
		ColourType colourType;

	public:

		Image(std::vector<unsigned char> data, uint32_t height, uint32_t width, ColourType colourType, std::string name = "image") 
		: data(data), height(height), width(width), colourType(colourType), name(name) {
		
		}

		uint32_t getHeight() const {
			return height;
		}

		uint32_t getWidth() const {
			return width;
		}

		ColourType getColourType() const {
			return colourType;
		}

		bool hasAlpha() const {
			switch (getColourType()) {
			case RGB8:
			case RGB16:
			case RGBF:
			case G8:
			case G16:
			case GF:
				return false;
			case RGBA8:
			case RGBA16:
			case RGBAF:
			case GA8:
			case GA16:
			case GAF:
				return true;
			}
			throw std::invalid_argument("colour type invalid");

		}

		uint8_t getChannelCount() const {
			switch (getColourType()) {
			case RGB8:
			case RGB16:
			case RGBF:
				return 3;
			case RGBA8:
			case RGBA16:
			case RGBAF:
				return 4;
			case G8:
			case G16:
			case GF:
				return 1;
			case GA8:
			case GA16:
			case GAF:
				return 2;
			}
			throw std::invalid_argument("colour type invalid");
		}
		
		uint8_t getNonAlphaChannelCount() const {
			if (hasAlpha())
				return getChannelCount() - 1;
			return getChannelCount();
		}

		uint8_t getBitDepth() const {
			switch (getColourType()) {
			case RGB8:
			case RGBA8:
			case G8:
			case GA8:
				return 8;
			case RGB16:
			case RGBA16:
			case G16:
			case GA16:
				return 16;
			case RGBF:
			case RGBAF:
			case GF:
			case GAF:
				return sizeof(float)*8;
			}
			throw std::invalid_argument("colour type invalid");

		}

		uint64_t getBytesPerLine() const {
			return getWidth() * getChannelCount() * getByteDepth();
		}
		
		bool hasFloat() const {
			switch (getColourType()) {
			case RGBF:
			case RGBAF:
			case GF:
			case GAF:
				return true;
			case RGB8:
			case RGB16:
			case RGBA8:
			case RGBA16:
			case G8:
			case G16:
			case GA8:
			case GA16:
				return false;
			}
			throw std::invalid_argument("colour type invalid");
		}


		uint8_t getByteDepth() const {
			return getBitDepth() / 8;
		}
		
		std::string getName() const {
			return name;
		}

		uint64_t getSubPixelCount() const {
			return getWidth() * getHeight() * getChannelCount();
		}

		unsigned char& at(int i1, int i2, int i3) {
			return data[i1*getBytesPerLine() + i2*getChannelCount()*getByteDepth() + i3* getByteDepth()];
		}
		const unsigned char& at(int i1, int i2, int i3) const {
			return data[i1 * getBytesPerLine() + i2 * getChannelCount() * getByteDepth() + i3*getByteDepth()];
		}

		float get(int i1, int i2=0, int i3=0) const {
			if (hasFloat()) {
				float newVal = 0;
				std::memcpy(&newVal, &at(i1, i2, i3), sizeof(float));
				return newVal;
			}
			else if (getByteDepth() == 2) {
				uint16_t newVal = 0;
				std::memcpy(&newVal, &at(i1, i2, i3), sizeof(uint16_t));
				return (float)newVal;
			}
			else if (getByteDepth() == 1) {
				return (float)at(i1, i2, i3);
			}

			throw std::invalid_argument("colour type invalid");

		}

		void write(float value, int i1, int i2=0, int i3=0) {
			
			if (isnan(value)) throw std::invalid_argument("Value to be written is NaN");

			if (hasFloat()) {
				memcpy(&at(i1,i2,i3), &value, sizeof(float));
	
			}
			else if (getByteDepth()==1) {
				const float clamped = std::clamp(std::round(value), 0.0f, 255.0f);
				uint8_t newval = static_cast<uint8_t>(clamped);
				memcpy(&at(i1,i2,i3), &newval, 1);
		
			}
			else if (getByteDepth() == 2) {
				const float clamped = std::clamp(std::round(value), 0.0f, 65535.0f);
				uint16_t newval = static_cast<uint16_t>(clamped);
				memcpy(&at(i1,i2,i3), &newval, 2);
			}
			
			else { throw std::invalid_argument("colour type invalid"); }

		}

		uint64_t getBytesPerRow() const {
			return getWidth() * getChannelCount() * getByteDepth();
		}

		/*!
		 * @brief returns a reference to the vector of bytes
		 */
		const std::vector<unsigned char>& getData() const {
			return data;
		}

	

		Image expand_G_RGB__GA_RGBA() const {
			ColourType newColourType;

			switch (getColourType()) {
			case RGB8:
			case RGB16:
			case RGBF:
			case RGBA8:
			case RGBA16:
			case RGBAF:
				return *this;
			case G8: newColourType = RGB8; break;
			case G16: newColourType = RGB16; break;
			case GF: newColourType = RGBF; break;
			case GA8: newColourType = RGBA8; break;
			case GA16:  newColourType = RGBA16; break;
			case GAF: newColourType = RGBAF; break;
			default: throw std::invalid_argument("Invalid colour space");
			}

			uint64_t newDataSize = getWidth() * getHeight() * 3 * getByteDepth();
			if (this->hasAlpha()) {
				newDataSize += getWidth() * getHeight() * getByteDepth();
			}

			Image newImage(std::vector<unsigned char>(newDataSize), getHeight(), getWidth(), newColourType, getName());

			for (int y = 0; y < getHeight(); y++) {
				for (int x = 0; x < getWidth(); x++) {
					for (int sub = 0; sub < newImage.getNonAlphaChannelCount(); sub++) {
						newImage.write(get(y, x, 0), y, x, sub);
					}
					if (hasAlpha()) newImage.write(get(y, x, 1), y, x, newImage.getChannelCount()-1);
				}
			}

			return newImage;
		}

		Image toUINT8() const {
			
			ColourType newColourType;
			float oldMax;

			switch (getColourType())
			{
			case lpcv::RGB8:
			case lpcv::RGBA8:
			case lpcv::G8:
			case lpcv::GA8:
				return *this;

			case lpcv::RGB16:
				oldMax = std::numeric_limits<uint16_t>::max();
				newColourType = RGB8;
				break;
			case lpcv::RGBF:
				oldMax = 1;
				newColourType = RGB8;
				break;
			case lpcv::RGBA16:
				oldMax = std::numeric_limits<uint16_t>::max();
				newColourType = RGBA8;
				break;
			case lpcv::RGBAF:
				oldMax = 1;
				newColourType = RGBA8;
				break;
			case lpcv::G16:
				oldMax = std::numeric_limits<uint16_t>::max();
				newColourType = G8;
				break;
			case lpcv::GF:
				oldMax = 1;
				newColourType = G8;
				break;
			case lpcv::GA16:
				oldMax = std::numeric_limits<uint16_t>::max();
				newColourType = GA8;
				break;
			case lpcv::GAF:
				oldMax = 1;
				newColourType = GA8;
				break;
			

			default: throw std::invalid_argument("Invalid colour space");

			}


			uint64_t newSize = getWidth() * getHeight() * getChannelCount();
			Image newImage(std::vector<unsigned char>(newSize), getHeight(), getWidth(), newColourType, getName());
			for (int y = 0; y < getHeight(); y++) {
				for (int x = 0; x < getWidth(); x++) {
					for (int sub = 0; sub < newImage.getChannelCount(); sub++) {
						uint8_t newVal = (get(y, x, sub)/oldMax) * std::numeric_limits<uint8_t>::max();
						newImage.write(newVal, y, x, sub);
					}
				}
			}
			return newImage;

		}

		Image toFLOAT() const {

			ColourType newColourType;
			switch (getColourType())
			{
			case lpcv::RGBF:
			case lpcv::RGBAF:
			case lpcv::GF:
			case lpcv::GAF:
				return *this;

			case lpcv::RGB16:
			case lpcv::RGB8:
				newColourType = RGBF;
				break;

			case lpcv::RGBA16:
			case lpcv::RGBA8:
				newColourType = RGBAF;
				break;

			case lpcv::G16:
			case lpcv::G8:
				newColourType = GF;
				break;
			case lpcv::GA8:
			case lpcv::GA16:
				newColourType = GAF;
				break;

			default: throw std::invalid_argument("Invalid colour space");

			}


			uint64_t newSize = getWidth() * getHeight() * getChannelCount() * 4;
			Image newImage(std::vector<unsigned char>(newSize), getHeight(), getWidth(), newColourType, getName());
			for (int y = 0; y < getHeight(); y++) {
				for (int x = 0; x < getWidth(); x++) {
					for (int sub = 0; sub < newImage.getChannelCount(); sub++) {
						float newVal = get(y, x, sub) / (std::pow(2, getBitDepth()) - 1);

						newImage.write(newVal, y, x, sub);
					}
				}
			}
			return newImage;

		}

		Image removeAlpha() const {
			ColourType newColourType;
			switch (getColourType())
			{
		
			case lpcv::RGB8:
			case lpcv::RGB16:
			case lpcv::RGBF:
			case lpcv::G8:
			case lpcv::G16:
			case lpcv::GF:
				return *this;

			case lpcv::RGBA8: 
				newColourType = RGB8;
				break;
			case lpcv::RGBA16:
				newColourType = RGB16;
				break;
			case lpcv::RGBAF:
				newColourType = RGBF;
				break;
			case lpcv::GA8:
				newColourType = G8;
				break;
			case lpcv::GA16:
				newColourType = G16;
				break;
			case lpcv::GAF:
				newColourType = GF;
				break;
			default: 
				throw std::invalid_argument("Invalid colour space");

			}

			uint64_t newSize = getWidth() * getHeight() * getNonAlphaChannelCount() * getByteDepth();
			Image newImage(std::vector<unsigned char>(newSize), getHeight(), getWidth(), newColourType, getName());

			for (uint32_t y = 0; y < newImage.getHeight(); y++) {
				for (uint32_t x = 0; x < newImage.getWidth(); x++) {
					for (uint32_t sub = 0; sub < newImage.getChannelCount(); sub++) {
						newImage.write(this->get(y, x, sub), y, x, sub);
					}
				}
			}
			return newImage;
		}

	};
};