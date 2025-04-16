#include<expected>

#include"lpcv.h"



#define CHECK(val) if (!val) return val.error()

int main() {

	auto image = loadPNG("C:\\Users\\liamp\\Desktop\\examplea.png");
	CHECK(image);
	auto image2 = lpcv::greyscale(*image);
	
	lpcv::display(*image);
	lpcv::display(image2);





	return 0;
	
}