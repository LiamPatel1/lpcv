#include"lpcv.h"
#include<QString>
#include<QImage>
#include<QMainWindow>
#include<QLabel>
#include<expected>

namespace lpcv {

    class Viewer : public QMainWindow {
    public:
        Viewer(lpcv::image image);

    protected:
        void resizeEvent(QResizeEvent* event) override;

    private:
        QPixmap originalPixmap;
        QLabel* label;
    };

}