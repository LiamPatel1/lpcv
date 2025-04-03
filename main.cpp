#include "lpcv.h"
#include "imagereader.h"
#include "show.h"
#include<iostream>

#define CHECK(val) if (!val) return val.error()

int main() {
	auto image = loadPNG("C:\\Users\\liamp\\Desktop\\untitled.png");
	CHECK(image);
	
	auto ret = lpcv::show(image.value());
	CHECK(ret);
	
}