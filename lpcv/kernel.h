#pragma once

#include"../lpcv.h"
#include"../lpcv/vec.h"

namespace lpcv {
	class Kernel : public Vec {
		
	public:
		Kernel(std::vector<std::vector<float>> data) {
			std::vector<unsigned char> newData = {};
			for (int y = 0; y < data.size(); y++) {
				for (int x = 0; x < data[y].size(); x++) {
					unsigned char newValue[4];
					memcpy(&newValue, &data[y][x], 4);
					newData.insert(newData.end(), newValue, newValue+4);
				}
			}
			this->data = newData;
			this->measurements = { (uint32_t)data.size(), (uint32_t)data[0].size()};
		}

		const uint32_t getHeight() const {
			return this->measurements[0];

		}
		const uint32_t getWidth() const {
			return this->measurements[1];

		}



	};
}