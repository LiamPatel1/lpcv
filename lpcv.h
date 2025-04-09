#pragma once

#include<vector>
#include<cstddef>

namespace lpcv {
	typedef unsigned char uchar;
	typedef std::vector<uchar> byteArray;


	template<typename T>
	class Vec2 {
	private:
		size_t rows_;
		size_t cols_;
		T* data;
	public:
		Vec2(size_t rows, size_t cols)
			: rows_(rows), cols_(cols), data(new T[rows_ * cols_]) {
		}

		T& operator()(size_t row, size_t col) {
			return data[row * cols_ + col];
		}
		const T& operator()(size_t row, size_t col) const {
			return data[row * cols_ + col];
		}

		size_t getHeight() const noexcept { return rows_; }
		size_t getWidth() const noexcept { return cols_; }
	};





	enum ColourSpace {
		NONE,
		RGB,
		RGBA,
		G,
		GA
	};

	enum Dimension {
		DIMENSION_X,
		DIMENSION_Y
	};

	enum Status {
		SUCCESS,
		ERROR_OPEN_FILE,
		ERROR_NOT_PNG,
		ERROR_PNG_READ_CREATION,
		ERROR_PNG_INFO_CREATION,
		ERROR_UNSUPPORTED_COLOUR_SPACE,
		ERROR_SHOW_INVALID_FORMAT,
		ERROR_KERNAL_EVEN
	};

	class Size {
		public:
			uint32_t x;
			uint32_t y;
			Size(uint32_t x, uint32_t y);
	};
}