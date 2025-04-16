#include<expected>

#include"lpcv.h"



#define CHECK(val) if (!val) return val.error()

int main() {

	auto image = loadPNG("C:\\Users\\liamp\\Desktop\\examplea.png");
	CHECK(image);
	image = lpcv::gaussian(*image);

	lpcv::display(*image);




	return 0;
	
}