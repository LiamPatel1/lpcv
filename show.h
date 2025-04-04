#include"lpcv.h"
#include<QString>
#include<QImage>
#include<QMainWindow>
#include<QLabel>
#include<expected>

namespace lpcv {

    lpcv::Status show(lpcv::image image);

    class ImageViewer : public QMainWindow {
    public:
        ImageViewer(lpcv::image image);

    protected:
        void resizeEvent(QResizeEvent* event) override;

    private:
        void updatePixmap();

        QPixmap m_originalPixmap;
        QLabel* m_label;
    };

}