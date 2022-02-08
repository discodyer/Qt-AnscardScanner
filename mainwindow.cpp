#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtWidgets/QMainWindow>
#include <QFileDialog>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>

using namespace cv;
using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionSettings_triggered()
{
    Settings *v = new Settings();
    v->show();
}

void MainWindow::on_actionOpen_triggered()
{
    img_name = QFileDialog::getOpenFileName( this, tr("Open Image"), ".",tr("Image Files(*.png *.jpg *.jpeg *.bmp)"));
    //QTextCodec *code = QTextCodec::codecForName("gb18030");
    //std::string name = code->fromUnicode(img_name).data();
    //QTextCodec *code = QTextCodec::codecForName("gb18030");
    //std::string name = code->fromUnicode(img_name).data();
    if(img_name.length()<=0)return;
    //imread的第二个参数：读取4通道的png图像。其中第四个通道的数据类型和其他通道的一样，都是uchar型，完全透明为0，否则为255
    src=imread(img_name.toUtf8().data());
    //toAscii()返回8位描述的string,为QByteArray,data()表示返回QByteArray的指针，QByteArray为字节指针，古老的toascii，我们使用toUtf8。网上有toLatin1，但是好像会出错
    //src = imread("girl.jpg");
//    namedWindow( "src", WINDOW_NORMAL );
//    if(!src.empty()){
//    imshow("src",src);
//    }
    cvtColor( src, src, COLOR_BGR2RGB );//https://github.com/IntelRealSense/librealsense/issues/1634
    img = QImage( (const unsigned char*)(src.data), src.cols, src.rows, QImage::Format_RGB888 );
    ui->labRawImage->setPixmap( QPixmap::fromImage(img));
    //ui->labRawImage->resize( ui->labRawImage->pixmap()->size());
}

void MainWindow::on_actionOpen_a_Image_triggered()
{
    MainWindow::on_actionOpen_triggered();
}

