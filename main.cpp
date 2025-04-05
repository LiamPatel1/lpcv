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

	CHECK(image1);
	CHECK(image2);

	new lpcv::Viewer(image1.value());
	new lpcv::Viewer(image2.value());

	a.exec();
	return 0;
	
}