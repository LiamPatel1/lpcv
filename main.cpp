#include<QApplication>
#include<expected>
#include"lpcv.h"
#include"lpcv/imagereader.h"
#include"lpcv/image.h"
#include"lpcv/gaussian.h"
#include"lpcv/viewer.h"

#define CHECK(val) if (!val) return val.error()

int main() {
	int argc = 0;
	QApplication a(argc, {});


	auto image = loadPNG("C:\\Users\\liamp\\Desktop\\untitled3.png");
	CHECK(image);

	new lpcv::Viewer(*image);
	auto ret = lpcv::gaussian(*image, { 3, 3 });
	CHECK(ret);
	new lpcv::Viewer(*image);


	a.exec();
	return 0;
	
}