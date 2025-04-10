#pragma once
#include<cstddef>
#include<vector>
#include<variant>
#include<array>
#include <cstdarg>
#include<algorithm>
#include"lpcv.h"

namespace lpcv {



	class Vec {

	protected:
		std::vector<unsigned char> data;
		std::vector<uint32_t> measurements;
		lpcv::Type type;

	public:


		const uint8_t getBitDepth() const {
			switch (type) {
				case lpcv::TYPE_U8:
					return 8;
				case lpcv::TYPE_U16:
					return 16;
				case lpcv::TYPE_U32:
					return 32;
				case lpcv::TYPE_U64:
					return 64;
				case lpcv::TYPE_FLOAT:
					return 32;
				default:
					return 0;
			}
			
		}

		const uint8_t getByteDepth() const {
			return (getBitDepth() / 8);
		}

		const uint8_t getDimensions() const {
			return measurements.size();
		}
		const uint64_t getSize() const {
			return data.size();
		}

		const std::vector<unsigned char>& getData() const {
			return this->data;
		}

		template <typename T>
		T at(const int i1, const int i2, const int i3) const {
			if (measurements.size() != 3) throw std::invalid_argument("invalid dimension count for access");

			unsigned char* addr = this->data[getByteDepth * (measurements[2] * (i1 * measurements[1] + i2) + i3)];

			T value;
			memcpy(value, addr, sizeof(value));
			return value;
		}

		template <typename T>
		T at(const int i1, const int i2) const {
			if (measurements.size() != 2) throw std::invalid_argument("invalid dimension count for access");

			const unsigned char* addr = &this->data[getByteDepth() * (i1 * measurements[1] + i2)];

			T value;
			//std::memcpy(&value, &addr, sizeof(value));

			uint64_t tmp = 0;
			for (int i = 0; i < sizeof(T); i++) {
				tmp = (tmp << 8) + addr[i];
			}
			
			std::memcpy(&value, tmp + (sizeof(tmp)-sizeof(T)), sizeof(T));
			return value;
		}

			

		

		Vec(std::vector<unsigned char> data, lpcv::Type type, std::vector<uint32_t> measurements)
			: measurements(measurements), data(data), type(type) {}

		Vec() : measurements({}), type(lpcv::TYPE_NONE), data({}) {}

		Vec(unsigned char* data, lpcv::Type type, std::vector<uint32_t> measurements)
			: measurements(measurements), type(type) {

			uint64_t dataSize = 0;
			for (int i = 0; i < measurements.size(); i++) 
				dataSize *= measurements[i];
			dataSize *= (getBitDepth()/8);
			for (int i = 0 ; i < dataSize ; i++) {
				this->data.push_back(data[i]);
			
			}
		}

	};
}