#include<expected>

#include"lpcv.h"



#define CHECK(val) if (!val) return val.error()

int main() {

	auto image = loadPNG("C:\\Users\\liamp\\Desktop\\example.png");
	
	image = lpcv::gaussian(image);

	image = lpcv::sobel(image);
	lpcv::display(image);




	return 0;
	
}