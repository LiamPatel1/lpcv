#pragma once
#include<cstddef>
#include<vector>
#include<stdexcept>
#include<variant>
#include<cmath>
#include<array>
#include<cstdarg>
#include<algorithm>

#include"lpcv/enum.h"
#include "bytearray.h"

namespace lpcv {



	class Vec {

	protected:

		std::vector<unsigned char> data;
		Type type;
		std::vector<uint32_t> measurements;

	public:


		const uint8_t getBitDepth() const {
			switch (type) {
			case TYPE_U8:
				return 8;
			case TYPE_U16:
				return 16;
			case TYPE_FLOAT:
				return 32;
			default:
				throw std::invalid_argument("Invalid image type");
			}

		}

		const uint8_t getByteDepth() const {
			return (getBitDepth() / 8);
		}

		
	
		const uint8_t getDimensions() const {
			return measurements.size();
		}
		const uint64_t getDataSize() const {
			return data.size();
		}

		const std::vector<unsigned char>& getData() const {
			return this->data;
		}

		const std::vector<uint32_t> getMeasurements() const {
			return measurements;
		}

		template<typename... Indices>
		float getValue_FLOAT(Indices... indices) {
			if (type == TYPE_FLOAT) {
				float newVal;
				memcpy(newVal, at(indices...), 4);
				return newVal;
			}
			else {
				uint16_t newVal;
				memcpy(newVal, at(indices...), 4);
				return (float)newVal;
			}

		}
		template<typename... Indices>
		const float getValue_FLOAT(Indices... indices) const {
			if (type == TYPE_FLOAT) {
				float newVal;
				memcpy(newVal, at(indices...), 4);
				return newVal;
			}
			else {
				uint16_t newVal;
				memcpy(newVal, at(indices...), 4);
				return (float)newVal;
			}

		}



		template<typename... Indices>
		const unsigned char* at(Indices... indices) const {
			const size_t num_dims = measurements.size();
			const size_t num_indices = sizeof...(Indices);
			const uint32_t byte_depth = getByteDepth();

		
			if (num_indices > num_dims) throw std::invalid_argument("Too many indicies given");

			std::vector<uint32_t> index_vector(num_dims, 0);
			size_t i = 0;
			((index_vector[i++] = static_cast<uint32_t>(indices)), ...);

			for (size_t j = 0; j < num_dims; ++j) {
				if (index_vector[j] >= measurements[j]) throw std::invalid_argument("Index out of image range");
			}

			std::vector<uint32_t> strides(num_dims, 1);
			for (int j = static_cast<int>(num_dims) - 2; j >= 0; --j) {
				strides[j] = strides[j + 1] * measurements[j + 1];
			}

			uint32_t linear_index = 0;
			for (size_t j = 0; j < num_dims; ++j) {
				linear_index += index_vector[j] * strides[j];
			}

			const uint32_t byte_offset = linear_index * byte_depth;
			
			
			const unsigned char* g = data.data();

			return g + byte_offset;
		}
		

		template<typename... Indices>
		unsigned char* at(Indices... indices) {
			return const_cast<unsigned char*>(
				static_cast<const Vec&>(*this).at(indices...));
		}


	
		template<typename... Indices>
		void setValue(float value, Indices...indicies) {
			switch (type) {

			case TYPE_FLOAT: {
				memcpy(at(indicies...), (&value), 4);
				break;
			}
			case TYPE_U8: {
				const float clamped = std::clamp(std::round(value), 0.0f, 255.0f);
				uint8_t newval = static_cast<uint8_t>(clamped);
				unsigned char* dfgfdg = at(indicies...);
				memcpy(at(indicies...), &newval, 1);
				break;
			}
			case TYPE_U16: {
				const float clamped = std::clamp(std::round(value), 0.0f, 65535.0f);
				uint16_t newval = static_cast<uint16_t>(clamped);
				memcpy(at(indicies...), &newval, 2);
				break;
			}
			}
		}

	
		template<typename T, typename... Indices>
		const T get(Indices... indices) const {
	
			T value;
			std::memcpy(&value, at(indices...), sizeof(T));
			return value;
		}

		

		Vec(std::vector<unsigned char> data, lpcv::Type type, std::vector<uint32_t> measurements)
			: measurements(measurements), data(data), type(type) {}

		Vec(ByteArray data, std::vector<uint32_t> measurements) : data((std::vector<unsigned char>)data), measurements(measurements), type(data.type) {}

		Vec() : measurements({}), data({}), type(lpcv::TYPE_NONE) {}

	};
}