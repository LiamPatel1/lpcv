#include<QApplication>
#include<expected>

#include"lpcv.h"


#define CHECK(val) if (!val) return val.error()

int main() {
	int argc = 0;
	QApplication a(argc, {});
	auto image = loadPNG("C:\\Users\\liamp\\Desktop\\untitled2.png");

	CHECK(image);

	image = lpcv::gaussian(*image);


	new lpcv::Viewer(*image);

	

	a.exec();



	return 0;
	
}