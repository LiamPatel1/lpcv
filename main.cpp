#include<QApplication>
#include<expected>
#include"lpcv/vec.h"
#include"lpcv.h"
#include"lpcv/imagereader.h"
#include"lpcv/image.h"
#include"lpcv/gaussian.h"
#include"lpcv/viewer.h"

#define CHECK(val) if (!val) return val.error()

int main() {
	int argc = 0;
	QApplication a(argc, {});
	auto image = loadPNG("C:\\Users\\liamp\\Desktop\\example.png");
	CHECK(image);
	auto image2 = lpcv::gaussian(*image);
	CHECK(image2);
	

	new lpcv::Viewer(*image);
	new lpcv::Viewer(*image2);


	
	const unsigned char* data = (unsigned char*)("dxkej687bjnd");






	a.exec();
	return 0;
	
}