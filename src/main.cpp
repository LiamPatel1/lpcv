#include<expected>

#include"lpcv.h"


int main() {

	auto image = loadPNG("C:\\Users\\liamp\\Desktop\\a.png");
	
	image = lpcv::canny(image);

	return 0;
	
}