#pragma once

#include<iostream>

namespace lpcv {
	class Kernel : public Image {


	public:
		Kernel(std::vector<std::vector<float>> data)
		: Image(kernelDatatoBytes(data), data.size(), data[0].size(), ColourType::GF) {}

		Kernel(uint32_t length)
			: Image(kernelDatatoBytes(std::vector<std::vector<float>>(length, std::vector<float>(length))), length, length, ColourType::GF) {
		}

		void l1Normalise() {
			float sum = 0;
			for (int y = 0; y < getHeight(); y++) {
				for (int x = 0; x < getWidth(); x++) {
					sum += abs(get(y,x));
				}
			}
			for (int y = 0; y < getHeight(); y++) {
				for (int x = 0; x < getWidth(); x++) {
					write(get(y, x) / sum, y, x);
				}
			}
		}

	
		void standardisationNormalise() {
			float sum = 0;
			for (int y = 0; y < getHeight(); y++) {
				for (int x = 0; x < getWidth(); x++) {
					sum += get(y,x);
				}
			}
			float mean = sum / getData().size();
			float sqrsum = 0;
			for (int y = 0; y < getHeight(); y++) {
				for (int x = 0; x < getWidth(); x++) {
					sqrsum += std::pow(get(y,x) - mean, 2);
				}
			}

			float stddev = std::sqrt(sqrsum / getData().size());

			for (int y = 0; y < getHeight(); y++) {
				for (int x = 0; x < getWidth(); x++) {
					write(((get(y, x) - mean) / stddev), y,x);
				}
			}
		
		}


		void print() {

			for (int y = 0; y < getHeight(); y++) {
				for (int x = 0; x < getWidth(); x++) {
					std::cout << get(y, x) << "  ";

				}
				std::cout << "\n";
			}
			std::cout << "\n";
		}

		static std::vector<unsigned char> kernelDatatoBytes(std::vector<std::vector<float>> data) {

			std::vector<unsigned char> newData;
			for (int y = 0; y < data.size(); y++) {
				for (int x = 0; x < data[y].size(); x++) {
					unsigned char newValue[sizeof(float)];
					memcpy(&newValue, &data[y][x], sizeof(float));
					newData.insert(newData.end(), newValue, newValue + sizeof(float));
				}

			}
			return newData;
		}


	};
}