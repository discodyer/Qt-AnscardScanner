#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtWidgets/QMainWindow>
#include <QFileDialog>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <vector>
#include <opencv2/core.hpp>
#include <cmath>
#include <QMessageBox>
#include <QTranslator>

using namespace cv;
using namespace std;

#define SET_IF_ARRYLEN 10

bool setIf[SET_IF_ARRYLEN]={
    false,//0 imshow Gray
    false,//1 imshow Gray + Blur
    false,//2 imshow edged
    false,//3 imshow All contours
    false,//4 imshow Out Line contours
    false,//5 imshow four lines
    false,//6 imshow find four points
    false,//7 imshow four point thresh
    false,//8 imshow cnts
    false //9 imshow mask
};

bool tmpSetIf[SET_IF_ARRYLEN]={
    false,//0 imshow Gray
    false,//1 imshow Gray + Blur
    false,//2 imshow edged
    false,//3 imshow All contours
    false,//4 imshow Out Line contours
    false,//5 imshow four lines
    false,//6 imshow find four points
    false,//7 imshow four point thresh
    false,//8 imshow cnts
    false //9 imshow mask
};

bool isCameraUsed=false;

RNG rng(12345);//Random Color

bool TopToBottom ( std::vector<cv::Point> contour1, std::vector<cv::Point> contour2 ) {
    double i = boundingRect(cv::Mat(contour1)).y;
    double j = boundingRect(cv::Mat(contour2)).y;
    return (i<j);
}

bool LeftToRight ( std::vector<cv::Point> contour1, std::vector<cv::Point> contour2 ) {
    double i = boundingRect(cv::Mat(contour1)).x;
    double j = boundingRect(cv::Mat(contour2)).x;
    return (i<j);
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //loadLanguage();

}

MainWindow::~MainWindow()
{
    delete ui;
}

//void MainWindow::loadLanguage(){
//    QTranslator translator;
//    //if (index == 0){
//        translator.load(":/../ReaderUI_zh_CN.qm");
//    //}else if (index == 1){
//    //    translator.load(":/../ReaderUI_zh_CN.qm");
//    //}
//    qApp->installTranslator(&translator);
//    ui->retranslateUi(this);
//}

void MainWindow::opencv_scan(cv::Mat * inputp,cv::Mat * outputp){

//**************预处理***************index****
    Mat inputImg=*inputp;

    Mat gray;//灰度处理后的图像
    cv::cvtColor(inputImg,gray,COLOR_BGR2GRAY);
    if(setIf[0]&&!gray.empty()){
        imshow("Gray",gray);
    }else if(gray.empty()){
        cout<<"Error: Step mainwindow.cpp:113 cv::cvtColor 'turn src to Gray' gray.empty()"<<endl;
        QMessageBox::critical(this, tr("Error"),  tr("Error: Step mainwindow.cpp:113 \ncv::cvtColor 'turn src to Gray' gray.empty()"),
                              QMessageBox::Discard,  QMessageBox::Discard);
        return;
    }

    Mat blur;
    cv::GaussianBlur(gray, blur, cv::Size(5,5) , 0 );
    if(setIf[1]&&!blur.empty()){
        imshow("gray + blur",blur);
    }else if(blur.empty()){
        cout<<"Error: mainwindow.cpp:128 Step indexcv::GaussianBlur 'turn gray to blur' blur.empty()"<<endl;
        QMessageBox::critical(this, tr("Error"),  tr("Error: mainwindow.cpp:128 \nStep cv::GaussianBlur 'turn gray to blur' blur.empty()"),
                              QMessageBox::Discard,  QMessageBox::Discard);
        return;
    }


    Mat edged;
    Canny(blur, edged, 75, 200);
    if(setIf[2]&&!edged.empty()){
        imshow("edged",edged);
    }else if(edged.empty()){
        cout<<"Error: mainwindow.cpp:142 Step cv::Canny'turn blur to edged' edged.empty()"<<endl;
        QMessageBox::critical(this, tr("Error"),  tr("Error: mainwindow.cpp:142 \nStep cv::Canny'turn blur to edged' edged.empty()"),
                              QMessageBox::Discard,  QMessageBox::Discard);
        return;
    }


//************预处理****************

//*************轮廓检测******************
    Mat findCont=edged;
    vector<vector<Point>> contours;     // 每个轮廓由一系列点组成
    vector<Vec4i> hierarchy;

    cv::findContours(findCont, contours, hierarchy, RETR_LIST, CHAIN_APPROX_SIMPLE);
    if(contours.size()!=0){
        cout << "contous.size:" << contours.size() << endl;
    }else{
        cout<<"Error: mainwindow.cpp:161 cv::findContours"<<endl;
        QMessageBox::critical(this, tr("Error"),  tr("Error: mainwindow.cpp:161 \ncv::findContours"),
                              QMessageBox::Discard,  QMessageBox::Discard);
        return;
    }

    //轮廓绘制
    int width = inputImg.cols;
    int height = inputImg.rows;
    Mat drawImage = Mat::zeros(inputImg.size(), CV_8UC3);
    cout << contours.size() << endl;
    for (size_t t = 0; t < contours.size(); t++)
    {
        Rect rect = boundingRect(contours[t]);
        if (rect.width > width / 2 && rect.height > height / 2 && rect.width<width-5 && rect.height<height-5)
        {
            drawContours(drawImage, contours, static_cast<int>(t), Scalar(0, 0, 255), 2, 8, hierarchy, 0, Point(0, 0));
        }
    }
    if(setIf[3]&&!drawImage.empty()){
        imshow("All contours", drawImage);//显示找到的轮廓
    }else if(drawImage.empty()){
        cout<<"Error: mainwindow.cpp:174 contours not found drawImage.empty()"<<endl;
        QMessageBox::critical(this, tr("Error"),  tr("Error: mainwindow.cpp:174 \ncontours not found drawImage.empty()"),
                              QMessageBox::Discard,  QMessageBox::Discard);
        return;
    }

    //直线检测
    vector<Vec4i> lines;
    Mat contoursImg;
    int accu = min(width*0.2, height*0.2);
    cvtColor(drawImage, contoursImg, COLOR_BGR2GRAY);
    if(setIf[4]&&!drawImage.empty()){
        imshow("Out Line contours", contoursImg);
    }else if(drawImage.empty()){
        cout<<"Error: mainwindow.cpp:197 contours not found contoursImg.empty()"<<endl;
        QMessageBox::critical(this, tr("Error"),  tr("Error: mainwindow.cpp:197 \ncontours not found contoursImg.empty()"),
                              QMessageBox::Discard,  QMessageBox::Discard);
        return;
    }

    Mat linesImage = Mat::zeros(inputImg.size(), CV_8UC3);
    HoughLinesP(contoursImg, lines, 1, CV_PI/180.0, accu, accu, 0);
    for (size_t t = 0; t < lines.size(); t++)
    {
        Vec4i ln = lines[t];
        line(linesImage, Point(ln[0], ln[1]), Point(ln[2], ln[3]), Scalar(0, 0, 255), 2, 8, 0);//绘制直线
    }
    cout << "number of lines:"<<lines.size() << endl;
    if(setIf[5]&&!linesImage.empty()){
        imshow("four lines", linesImage);
    }else if(linesImage.empty()){
        cout<<"Error: mainwindow.cpp:208 four lines contours not found linesImage.empty()"<<endl;
        QMessageBox::critical(this, tr("Error"),  tr("Error: mainwindow.cpp:208 \nfour lines contours not found linesImage.empty()"),
                              QMessageBox::Discard,  QMessageBox::Discard);
        return;
    }

    //寻找与定位上下 左右 四条直线
    int deltah = 0; //高度差
    int deltaw = 0; //宽度差
    Vec4i topLine, bottomLine; //直线定义
    Vec4i rightLine, leftLine;

    for (int i = 0; i < lines.size(); i++)
    {
        Vec4i ln = lines[i];
        /*
        Opencv中的累计霍夫变换HoughLinesP()，输出的是一个Vector of Vec4i，
        Vector每个元素代表一条直线，是由一个4元浮点数构成，
        前两个一组x_1,y_1，后两个一组x_2,y_2，代表了图像中直线的起始点和结束点。
        */
        deltah = abs(ln[3] - ln[1]); //计算高度差(y2-y1)
        //topLine
        if (ln[3] < height / 2.0 && ln[1] < height / 2.0 && deltah < accu - 1)
        {
            topLine = lines[i];
        }
        //bottomLine
        if (ln[3] > height / 2.0 && ln[1] >height / 2.0 && deltah < accu - 1)
        {
            bottomLine = lines[i];
        }

        deltaw = abs(ln[2] - ln[0]); //计算宽度差(x2-x1)
        //leftLine
        if (ln[0] < height / 2.0 && ln[2] < height / 2.0 && deltaw < accu - 1)
        {
            leftLine = lines[i];
        }
        //rightLine
        if (ln[0] > width / 2.0 && ln[2] >width / 2.0 && deltaw < accu - 1)
        {
            rightLine = lines[i];
        }
    }

    // 打印四条线的坐标
    cout << "topLine : p1(x,y)= " << topLine[0] << "," << topLine[1] << "; p2(x,y)= " << topLine[2] <<  "," << topLine[3] << endl;
    cout << "bottomLine : p1(x,y)= " << bottomLine[0] << "," << bottomLine[1] << "; p2(x,y)= " << bottomLine[2] <<  "," << bottomLine[3] << endl;
    cout << "leftLine : p1(x,y)= " << leftLine[0] << "," << leftLine[1] << "; p2(x,y)= " << leftLine[2] <<  "," << leftLine[3] << endl;
    cout << "rightLine : p1(x,y)= " << rightLine[0] << "," << rightLine[1] << "; p2(x,y)= " << rightLine[2] << "," << rightLine[3] << endl;

    //拟合四条直线
    float k1, k2, k3, k4, c1, c2, c3, c4;
    k1 = float(topLine[3] - topLine[1]) / float(topLine[2] - topLine[0]);
    c1 = topLine[1] - k1*topLine[0];

    k2 = float(bottomLine[3] - bottomLine[1]) / float(bottomLine[2] - bottomLine[0]);
    c2 = bottomLine[1] - k2*bottomLine[0];

    k3 = float(leftLine[3] - leftLine[1]) / float(leftLine[2] - leftLine[0]);
    c3 = leftLine[1] - k3*leftLine[0];

    k4 = float(rightLine[3] - rightLine[1]) / float(rightLine[2] - rightLine[0]);
    c4 = rightLine[1] - k4*rightLine[0];
//*************轮廓检测******************

//*************透视变换******************

    //求四个角点,
    Point p1;//topLine  leftLine 左上角
    p1.x = static_cast<int>(c1 - c3) / k3 - k1;
    p1.y = k1*p1.x + c1;

    Point p2;//topLine  rightLine 右上角
    p2.x = static_cast<int>(c1 - c4) / k4 - k1;
    p2.y = k1*p2.x + c1;

    Point p3;//bottomLine  leftLine 左下角
    p3.x = static_cast<int>(c2 - c3) / k3 - k2;
    p3.y = k2*p3.x + c2;

    Point p4;//bottomLine  rightLine 右下角
    p4.x = static_cast<int>(c2 - c4) / k4 - k2;
    p4.y = k2*p4.x + c2;

    cout << "Point p1: (" << p1.x << "," << p1.y << ")" << endl;
    cout << "Point p2: (" << p2.x << "," << p2.y << ")" << endl;
    cout << "Point p3: (" << p3.x << "," << p3.y << ")" << endl;
    cout << "Point p4: (" << p4.x << "," << p4.y << ")" << endl;


    //显示四个点
    cv::circle(linesImage, p1, 2, Scalar(0, 255, 0), 2);
    cv::circle(linesImage, p2, 2, Scalar(0, 255, 0), 2);
    cv::circle(linesImage, p3, 2, Scalar(0, 255, 0), 2);
    cv::circle(linesImage, p4, 2, Scalar(0, 255, 0), 2);

    if(setIf[6]&&!linesImage.empty()){
        imshow("find four points", linesImage);
    }else if(linesImage.empty()){
        cout<<"Error: mainwindow.cpp:308 find four points not found linesImage.empty()"<<endl;
        QMessageBox::critical(this, tr("Error"),  tr("Error: mainwindow.cpp:308 \nfind four points not found linesImage.empty()"),
                              QMessageBox::Discard,  QMessageBox::Discard);
        return;
    }
    if((p1.x==p2.x)&&(p1.y==p2.y)){
        cout<<"Error: mainwindow.cpp:308 same point found"<<endl;
        QMessageBox::critical(this, tr("Error"),  tr("Error: mainwindow.cpp:308 \nsame point found"),
                              QMessageBox::Discard,  QMessageBox::Discard);
        return;
    }

    //透视变换
    vector<Point2f> src_corners(4);
    src_corners[0] = p1;
    src_corners[1] = p2;
    src_corners[2] = p3;
    src_corners[3] = p4;

    Mat transedImages = Mat::zeros(500, 400, CV_8UC3);
    vector<Point2f> dst_corners(4);
    dst_corners[0] = Point(0, 0);
    dst_corners[1] = Point(transedImages.cols, 0);
    dst_corners[2] = Point(0, transedImages.rows);
    dst_corners[3] = Point(transedImages.cols, transedImages.rows);

    Mat warpmatrix = getPerspectiveTransform(src_corners, dst_corners); //获取透视变换矩阵
    warpPerspective(inputImg, transedImages, warpmatrix, transedImages.size()); //透视变换
    //原文链接：https://blog.csdn.net/plSong_CSDN/article/details/93743821


//*************透视变换******************

//*************对透视变换结果进行处理***************

    Mat thresh;
    Mat transedImage_Copy = transedImages;
    cvtColor(transedImage_Copy, transedImage_Copy, COLOR_BGR2GRAY);
    threshold(transedImage_Copy, thresh, 0, 255, THRESH_BINARY_INV | THRESH_OTSU);
    if(setIf[7]&&!thresh.empty()){
        imshow("four points thresh", thresh);
    }else if(thresh.empty()){
        cout<<"Error: mainwindow.cpp:356 cv::threshold thresh error thresh.empty()"<<endl;
        QMessageBox::critical(this, tr("Error"),  tr("Error: mainwindow.cpp:356 \ncv::threshold thresh error thresh.empty()"),
                              QMessageBox::Discard,  QMessageBox::Discard);
        return;
    }

    Mat threshContours = thresh;
    vector<vector<Point>> cnts;
    findContours(threshContours, cnts, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    drawContours(threshContours, cnts, -1, Scalar(0,0,255), 3);
    if(setIf[8]&&!threshContours.empty()){
        imshow("cnts", threshContours);
    }else if(threshContours.empty()){
        cout<<"Error: mainwindow.cpp:368 cv::findContours threshContours error threshContours.empty()"<<endl;
        QMessageBox::critical(this, tr("Error"),  tr("Error: mainwindow.cpp:368 \ncv::findContours threshContours error threshContours.empty()"),
                              QMessageBox::Discard,  QMessageBox::Discard);
        return;
    }

    vector<vector<Point>> questionCnts;
//    std::vector<std::vector<cv::Point> > questionCnts;
//    vector<Rect> questionCnts;
//    vector<Rect> ploy_rects(cnts.size());
//    vector<Point2f> ccs(cnts.size());
//    vector<float> radius(cnts.size());

//    vector<RotatedRect> minRects(cnts.size());
//    vector<RotatedRect> myellipse(cnts.size());

//    for (size_t i = 0; i < cnts.size(); i++) {
//        approxPolyDP(Mat(contours[i]), questionCnts[i], 3, true);
//        ploy_rects[i] = boundingRect(questionCnts[i]);
//        minEnclosingCircle(questionCnts[i], ccs[i], radius[i]);
//        if (questionCnts[i].size() > 5) {
//            myellipse[i] = fitEllipse(questionCnts[i]);
//            minRects[i] = minAreaRect(questionCnts[i]);
//        }
//    }
    vector<Rect> ploy_rects;
    for (size_t i = 0; i < cnts.size(); i++) {
        cv::Rect exRect = boundingRect(cnts[i]);
        float arc = exRect.width/float(exRect.height);
        if(exRect.width>20 and exRect.height>20 and exRect.width<100 and exRect.height<100 and arc >= 0.9 and arc <= 1.1){
            questionCnts.push_back(cnts[i]);
        }
    }

    std::sort(questionCnts.begin(), questionCnts.end(), TopToBottom);//轮廓排序
    //std::sort(questionCnts.begin(), questionCnts.end(), LeftToRight);
    for (int i=0;i<25 ; i+=5 ) {
        std::sort(questionCnts.begin()+i, questionCnts.begin()+i+5, LeftToRight);
    }

//    Mat drawing = Mat::zeros(transedImages.size(), CV_8UC3);
//    for (int i = 0; i<1;i++){
//        Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255));
//        drawContours(drawing, questionCnts, i, color, 2, 8, hierarchy, 0, Point());
//    }
//    imshow("www",drawing);

//    prsdImg = QImage((const unsigned char*)(drawing.data),drawing.cols,drawing.rows,drawing.step,  QImage::Format_RGB888);
//    ui->labProcessedImage->setPixmap( QPixmap::fromImage(prsdImg));//显示处理后的图像



//*************对透视变换结果进行处理***************

//***************读取答案*******************

    int ANSKEY[]= {2,10,11,19,22};
    int correct = 0;
    for (int i=0;i<25;i++ ) {
        Mat mask = Mat::zeros(transedImages.size(), CV_8UC1);
        Mat masked;
        cv::drawContours(mask, questionCnts, i, Scalar(255,255,255), -1);
        if(setIf[9]&&!mask.empty()){
            imshow("mask",mask);
        }else if(mask.empty()){
            cout<<"Error: mainwindow.cpp:434 cv::drawContours drawContours error mask.empty()"<<endl;
            QMessageBox::critical(this, tr("Error"),  tr("Error: mainwindow.cpp:434 \ncv::drawContours drawContours error mask.empty()"),
                                  QMessageBox::Discard,  QMessageBox::Discard);
            return;
        }
        bitwise_and(threshContours, threshContours, masked, mask);
        int total = countNonZero(masked);
        cout<<i+1<<" "<<total<<endl;
        if ((total>400) and (ANSKEY[int(i/5)]==i+1)){
            correct++;
            drawContours(transedImages, questionCnts, i, Scalar(0,255,0), 2, 8, hierarchy, 0, Point());
            continue;
        }else if((total<400) and ANSKEY[int(i/5)]==i+1){
            drawContours(transedImages, questionCnts, i, Scalar(0,0,255), 2, 8, hierarchy, 0, Point());
            continue;
        }else if(total>400 and (ANSKEY[int(i/5)]!=i+1)){
            drawContours(transedImages, questionCnts, i, Scalar(255,0,0), 2, 8, hierarchy, 0, Point());
            continue;
        }
    }
    std::string text = "Score: " + to_string(float(correct)/float(5)*100);
    cv::Point origin;
    origin.x=10;
    origin.y=50;
    cv::putText(transedImages, text, origin, FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(0, 255, 255), 2, 8, 0);

    *outputp=transedImages;
//***************读取答案*******************

}


void MainWindow::on_actionSettings_triggered()
{
    Settings *v = new Settings();
    v->show();
}



void MainWindow::on_actionOpen_triggered()
{
    img_name = QFileDialog::getOpenFileName( this, tr("Open Image"), ".",tr("Image Files(*.png *.jpg *.jpeg *.bmp)"));
    if(img_name.length()<=0){
        fprintf(stderr, "Can not load image !");
        return;
    }

    //imread的第二个参数：读取4通道的png图像。其中第四个通道的数据类型和其他通道的一样，都是uchar型，完全透明为0，否则为255
    //src=imread(img_name.toUtf8().data());//opencv 读取图像数据
    cv::Mat src = cv::imread(img_name.toUtf8().data());
    cv::resize(src,src,cv::Size(src.cols/4*4,src.rows/4*4));


    //toAscii()返回8位描述的string,为QByteArray,data()表示返回QByteArray的指针，QByteArray为字节指针，古老的toascii，我们使用toUtf8。网上有toLatin1，但是好像会出错
    //src = imread("girl.jpg");
//    namedWindow( "src", WINDOW_NORMAL );
//    if(!src.empty()){
//    imshow("src",src);
//    }

//    Mat inputImg;
//    cvtColor( src, inputImg, COLOR_BGR2RGB );//https://github.com/IntelRealSense/librealsense/issues/1634

    img = QImage( (const unsigned char*)(src.data), src.cols, src.rows, QImage::Format_RGB888 );
    ui->labRawImage->setPixmap( QPixmap::fromImage(img));//显示原始图像
    //ui->labRawImage->resize( ui->labRawImage->pixmap()->size());
    Mat inputImg=src;
    Mat outputImg;


    opencv_scan(&inputImg,&outputImg);

    prsdImg = QImage((const unsigned char*)(outputImg.data),outputImg.cols,outputImg.rows,outputImg.step,  QImage::Format_RGB888);
    ui->labProcessedImage->setPixmap( QPixmap::fromImage(prsdImg));//显示处理后的图像
}

void MainWindow::on_actionOpen_a_Image_triggered()
{
    MainWindow::on_actionOpen_triggered();
}


void MainWindow::on_actionScan_triggered()
{

}



void MainWindow::on_actionCamera_triggered()
{
    isCameraUsed=!isCameraUsed;
    ui->actionCamera->setChecked(isCameraUsed);
}


void MainWindow::on_actionLanguages_triggered()
{
    //loadLanguage();
}

