#pragma once

#include"lpcv/enum.h"
#include<vector>

namespace lpcv {

	class ByteArray : public std::vector<unsigned char> {



	public:
		Type type;

		ByteArray() : type(lpcv::TYPE_NONE) {}

		ByteArray(std::vector<unsigned char> data, Type type)
			: std::vector<unsigned char>(data), type(type) {}



		void setValue(float value, int i) {

			switch (type) {

			case TYPE_FLOAT: {
				memcpy(&this[i * 4], (&value), 4);
				break;
			}
			case TYPE_U8: {
				const float clamped = std::clamp(std::round(value), 0.0f, 255.0f);
				uint8_t newval = static_cast<uint8_t>(clamped);
				memcpy(&this[i], &newval, 1);
				break;
			}
			case TYPE_U16: {
				const float clamped = std::clamp(std::round(value), 0.0f, 65535.0f);
				uint16_t newval = static_cast<uint16_t>(clamped);
				memcpy(&this[i * 2], &newval, 2);
				break;
			}
			}
		}

		void setValue(float value, unsigned char* addr) {

			switch (type) {

			case TYPE_FLOAT: {
				memcpy(addr, (&value), 4);
				break;
			}
			case TYPE_U8: {
				const float clamped = std::clamp(std::round(value), 0.0f, 255.0f);
				uint8_t newval = static_cast<uint8_t>(clamped);
				memcpy(addr, &newval, 1);
				break;
			}
			case TYPE_U16: {
				const float clamped = std::clamp(std::round(value), 0.0f, 65535.0f);
				uint16_t newval = static_cast<uint16_t>(clamped);
				memcpy(addr, &newval, 2);
				break;
			}
			}
		}


		

	
	};
}
