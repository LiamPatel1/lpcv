#include<QApplication>
#include<expected>
#include"lpcv.h"
#include"lpcv/imagereader.h"
#include"lpcv/image.h"
#include"lpcv/viewer.h"

#define CHECK(val) if (!val) return val.error()

int main() {
	int argc = 0;
	QApplication a(argc, {});


	auto image1 = loadPNG("C:\\Users\\liamp\\Desktop\\untitled.png");
	auto image2 = loadPNG("C:\\Users\\liamp\\Desktop\\untitled2.png");
	auto image3 = loadPNG("C:\\Users\\liamp\\Desktop\\untitled3.png");

	CHECK(image1);
	CHECK(image2);
	CHECK(image3);

	new lpcv::Viewer(image1.value());
	new lpcv::Viewer(image2.value());
	new lpcv::Viewer(image3.value());

	a.exec();
	return 0;
	
}