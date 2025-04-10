#include<QApplication>
#include<expected>

#include"lpcv.h"


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


	//lpcv::Image image({ 
	//	0,0,0, 0,0,0, 0,0,0,
	//	0,0,0, 0,0,0, 0,0,0,
	//	0,0,0, 0,0,0, 0,0,0}, 8, 3, 3, lpcv::RGB);

	//image.setValue(255, 1, 1, 1);
	//
	//new lpcv::Viewer(image);

	

	a.exec();
	return 0;
	
}