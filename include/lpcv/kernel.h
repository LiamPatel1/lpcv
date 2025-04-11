#pragma once

#include"lpcv/vec.h"
#include<iostream>

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
			this->type = lpcv::TYPE_FLOAT;
			this->data = newData;
			this->measurements = { (uint32_t)data.size(), (uint32_t)data[0].size()};
		}

		const uint32_t getHeight() const {
			return this->measurements[0];

		}
		const uint32_t getWidth() const {
			return this->measurements[1];

		}

		void setValue(float value, int i1, int i2) {
			memcpy(this->at(i1, i2), (&value), 4);

		}

		void l1Normalise() {
			float sum = 0;
			for (int y = 0; y < getHeight(); y++) {
				for (int x = 0; x < getWidth(); x++) {
					sum += abs(get<float>(y,x));
				}
			}
			for (int y = 0; y < getHeight(); y++) {
				for (int x = 0; x < getWidth(); x++) {
					setValue(get<float>(y, x) / sum, y, x);
				}
			}

		}

	
		


		void standardisationNormalise() {
			float sum = 0;
			for (int y = 0; y < getHeight(); y++) {
				for (int x = 0; x < getWidth(); x++) {
					sum += get<float>(y,x);
				}
			}
			float mean = sum / getSize();
			float sqrsum = 0;
			for (int y = 0; y < getHeight(); y++) {
				for (int x = 0; x < getWidth(); x++) {
					sqrsum += std::pow(get<float>(y,x) - mean, 2);
				}
			}

			float stddev = std::sqrt(sqrsum / getSize());

			for (int y = 0; y < getHeight(); y++) {
				for (int x = 0; x < getWidth(); x++) {
					setValue(((get<float>(y, x) - mean) / stddev),y,x);
				}
			}
		
		}


		void print() {

			for (int y = 0; y < getHeight(); y++) {
				for (int x = 0; x < getWidth(); x++) {
					std::cout << get<float>(y, x) << "  ";

				}
				std::cout << "\n";
			}
			std::cout << "\n";
		}
	};
}