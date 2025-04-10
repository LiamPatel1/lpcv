#include"lpcv.h"
#include<QImage>
#include<QApplication>
#include<iostream>
#include<QLayout>
#include<QWindow>
#include<QLabel>
#include<QDebug>
#include <iterator>
#include <cstring>
#include<cstddef>
#include<QMainWindow>
#include <QPixmap>
#include<algorithm>
#include <QResizeEvent>


lpcv::Viewer::Viewer(lpcv::Image image) {
	QImage::Format format = QImage::Format_Invalid;
	if (image.getBitDepth() == 8 && image.getColourSpace() == lpcv::RGBA) format = QImage::Format_RGBA8888;
	if (image.getBitDepth() == 8 && image.getColourSpace() == lpcv::RGB) format = QImage::Format_RGB888;
	if (image.getBitDepth() == 8 && image.getColourSpace() == lpcv::G) format = QImage::Format_Grayscale8;
	if (image.getBitDepth() == 16 && image.getColourSpace() == lpcv::G) format = QImage::Format_Grayscale16;
	if (image.getBitDepth() == 16 && image.getColourSpace() == lpcv::RGBA) format = QImage::Format_RGBA64;

	QImage qimage(image.getWidth(), image.getHeight(), format);




	for (int y = 0; y < image.getHeight(); y++) {
		std::copy(image.at(y), image.at(y)+image.getBytesPerRow(), qimage.scanLine(y));
	}
    originalPixmap = QPixmap::fromImage(qimage);
    label = new QLabel(this); 
    label->setAlignment(Qt::AlignCenter);
    label->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    this->setCentralWidget(label);
	this->resize(500, 500);
	this->show();
}

void lpcv::Viewer::resizeEvent(QResizeEvent* event) {
    QMainWindow::resizeEvent(event);
	label->setPixmap(originalPixmap.scaled(label->size(), Qt::KeepAspectRatio ,Qt::SmoothTransformation));

}
