#pragma once

#include"lpcv.h"
#include"lpcv/image.h"
#include<QString>
#include<QImage>
#include<QMainWindow>
#include<QLabel>
#include<expected>

namespace lpcv {

    class Viewer : public QMainWindow {
    public:
        Viewer(lpcv::Image image);

    protected:
        void resizeEvent(QResizeEvent* event) override;

    private:
        QPixmap originalPixmap;
        QLabel* label;
    };

}