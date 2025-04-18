#include<expected>

#include"lpcv.h"



#define CHECK(val) if (!val) return val.error()

int main() {

	auto image = loadPNG("C:\\Users\\liamp\\Desktop\\example.png");
	
	image = lpcv::gaussian(image);

	image = lpcv::canny(image);
	lpcv::display(image);




	return 0;
	
}