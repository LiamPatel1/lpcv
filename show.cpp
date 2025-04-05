#include"lpcv.h"
#include"show.h"
#include<QImage>
#include<QApplication>
#include<iostream>
#include<QLayout>
#include<QWindow>
#include<QLabel>
#include<QDebug>
#include<QMainWindow>
#include <QPixmap>
#include <QResizeEvent>


lpcv::Viewer::Viewer(lpcv::image image) {
	QImage::Format format = QImage::Format_Invalid;
	if (image.getColourDepth() == 8 && image.getColourSpace() == lpcv::RGBA) format = QImage::Format_RGBA8888;
	if (image.getColourDepth() == 8 && image.getColourSpace() == lpcv::RGB) format = QImage::Format_RGB888;
	if (image.getColourDepth() == 8 && image.getColourSpace() == lpcv::G) format = QImage::Format_Grayscale8;
	if (image.getColourDepth() == 8 && image.getColourSpace() == lpcv::G) format = QImage::Format_Grayscale16;
	if (image.getColourDepth() == 16 && image.getColourSpace() == lpcv::RGBA) format = QImage::Format_RGBA64;

	QImage qimage(image.getWidth(), image.getHeight(), format);
	for (int y = 0; y < qimage.height(); y++)
		memcpy(qimage.scanLine(y), &image.getUChar()[y * qimage.width() * (qimage.depth()/8)], qimage.width() *(qimage.depth())/8);

    originalPixmap = QPixmap::fromImage(qimage);
    label = new QLabel(this); 
    label->setAlignment(Qt::AlignCenter);
    label->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    this->setCentralWidget(label);
	this->show();
}

void lpcv::Viewer::resizeEvent(QResizeEvent* event) {
    QMainWindow::resizeEvent(event);
	label->setPixmap(originalPixmap.scaled(label->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));

}
