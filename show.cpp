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


lpcv::ImageViewer::ImageViewer(lpcv::image image) {
	QImage::Format format = QImage::Format_Invalid;
	if (image.getColourDepth() == 8 && image.getColourSpace() == lpcv::RGBA) format = QImage::Format_RGBA8888;
	if (image.getColourDepth() == 8 && image.getColourSpace() == lpcv::RGB) format = QImage::Format_RGB888;
	if (image.getColourDepth() == 8 && image.getColourSpace() == lpcv::G) format = QImage::Format_Grayscale8;
	if (image.getColourDepth() == 8 && image.getColourSpace() == lpcv::RGBA) format = QImage::Format_Grayscale16;
	if (image.getColourDepth() == 16 && image.getColourSpace() == lpcv::RGBA) format = QImage::Format_RGBA64;
	
	QImage qimage(image.getWidth(), image.getHeight(), format);

	//char data[] = {255,255,255 ,255,000,000,
	//				 255,000,000, 255,255,255};

	
	//QImage qimage(2, 2, QImage::Format_RGB888);

	for (int y = 0; y < qimage.height(); y++) {
		memcpy(qimage.scanLine(y), &image.getUChar()[y * qimage.width() * (qimage.depth()/8)], qimage.width() *(qimage.depth())/8);
	}


    m_originalPixmap = QPixmap::fromImage(qimage);
    m_label = new QLabel(this);
    m_label->setAlignment(Qt::AlignCenter);

    m_label->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);

    this->setCentralWidget(m_label);
    updatePixmap();
}


void lpcv::ImageViewer::resizeEvent(QResizeEvent* event) {
    QMainWindow::resizeEvent(event);
    updatePixmap();
}


void lpcv::ImageViewer::updatePixmap() {
    QSize newSize = m_label->size();
    QPixmap scaled = m_originalPixmap.scaled(newSize,
        Qt::KeepAspectRatio /*,Qt::SmoothTransformation*/);
    m_label->setPixmap(scaled);
}

lpcv::Status lpcv::show(lpcv::image image) {


	int argc = 0;
	QApplication a(argc, {});

	lpcv::ImageViewer view(image);
	view.show();

	a.exec();
	return lpcv::Status::SUCCESS;
}