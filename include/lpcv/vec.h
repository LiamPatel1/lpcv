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

namespace lpcv {



	class Vec {

	protected:
		std::vector<unsigned char> data;
		std::vector<uint32_t> measurements;
		Type type;

	public:


		const uint8_t getBitDepth() const {
			switch (type) {
				case TYPE_U8:
					return 8;
				case TYPE_U16:
					return 16;
				case TYPE_U32:
					return 32;
				case TYPE_U64:
					return 64;
				case TYPE_FLOAT:
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
		const uint64_t getDataSize() const {
			return data.size();
		}

		const std::vector<unsigned char>& getData() const {
			return this->data;
		}

		void insertData(std::vector<unsigned char> newData) {
			data.insert(data.end(), newData.begin(), newData.end());
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
		
			return data.data() + byte_offset;
		}
		

		template<typename... Indices>
		unsigned char* at(Indices... indices) {
			return const_cast<unsigned char*>(
				static_cast<const Vec&>(*this).at(indices...));
		}


	
		template<typename T, typename... Indices>
		const T get(Indices... indices) const {
	
			T value;
			std::memcpy(&value, at(indices...), sizeof(T));
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