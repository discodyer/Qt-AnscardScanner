#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtWidgets/QMainWindow>
#include <QFileDialog>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <vector>

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
    src=imread(img_name.toUtf8().data());//opencv 读取图像数据


    //toAscii()返回8位描述的string,为QByteArray,data()表示返回QByteArray的指针，QByteArray为字节指针，古老的toascii，我们使用toUtf8。网上有toLatin1，但是好像会出错
    //src = imread("girl.jpg");
//    namedWindow( "src", WINDOW_NORMAL );
//    if(!src.empty()){
//    imshow("src",src);
//    }

//**************预处理*******************
    Mat inputImg;
    cvtColor( src, inputImg, COLOR_BGR2RGB );//https://github.com/IntelRealSense/librealsense/issues/1634
    img = QImage( (const unsigned char*)(inputImg.data), inputImg.cols, inputImg.rows, QImage::Format_RGB888 );
    ui->labRawImage->setPixmap( QPixmap::fromImage(img));//显示原始图像
    //ui->labRawImage->resize( ui->labRawImage->pixmap()->size());

    Mat gray;//灰度处理后的图像
    cvtColor(src,gray,COLOR_BGR2GRAY);

//    //prsdImg = QImage( (const unsigned char*)(gray.data), gray.cols, gray.rows, QImage::Format_RGB888 );
//    prsdImg = QImage((const unsigned char*)(gray.data),gray.cols,gray.rows,gray.step,  QImage::Format_Indexed8);
//    ui->labProcessedImage->setPixmap( QPixmap::fromImage(prsdImg));//显示灰度处理后的图像

    Mat blur;
    cv::GaussianBlur(gray, blur, cv::Size(5,5) , 0 );

//    prsdImg = QImage((const unsigned char*)(blur.data),blur.cols,blur.rows,blur.step,  QImage::Format_Indexed8);
//    ui->labProcessedImage->setPixmap( QPixmap::fromImage(prsdImg));//显示blur处理后的图像

    Mat edged;
    Canny(blur, edged, 75, 200);
    prsdImg = QImage((const unsigned char*)(edged.data),edged.cols,edged.rows,edged.step,  QImage::Format_Indexed8);
    ui->labProcessedImage->setPixmap( QPixmap::fromImage(prsdImg));//显示edged处理后的图像

//************预处理****************

//*************轮廓检测******************
    Mat findCont=edged;
    vector<vector<Point>> contours;     // 每个轮廓由一系列点组成
    vector<Vec4i> hierarchy;

    cv::findContours(findCont, contours, hierarchy, RETR_LIST, CHAIN_APPROX_SIMPLE);
    vector<vector<Point>> cnts;
    cv::sort (contours, cnts, contourArea())



//*************轮廓检测******************






}

void MainWindow::on_actionOpen_a_Image_triggered()
{
    MainWindow::on_actionOpen_triggered();
}


void MainWindow::on_actionScan_triggered()
{

}

