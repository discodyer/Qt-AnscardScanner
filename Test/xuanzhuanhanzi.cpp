#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <vector>
#include <opencv2/core.hpp>
#include <cmath>
using namespace cv;
int main()
{
//****************输入图片**********************
    std::string image_path = samples::findFile("/home/cody/Downloads/QQ_Image_1644599160012_r.jpg");
    cv::Mat img = imread(image_path, IMREAD_COLOR);
    if(img.empty())
    {
        std::cout << "Could not read the image: " << image_path << std::endl;
        return 1;
    }
    imshow("Raw Image", img);
//****************输入图片**********************

//***************图片预处理*******************

    cv::Mat gray;//灰度处理后的图像
    cv::cvtColor(img,gray,COLOR_BGR2GRAY);
    if(!gray.empty()){
        imshow("Gray",gray);
    }else if(gray.empty()){
        std::cout<<"Error: cv::cvtColor 'turn src to Gray' gray.empty()"<<std::endl;
        return 1;
    }
    //显示灰度处理后的图像

    Mat blur;
    cv::GaussianBlur(gray, blur, cv::Size(3,3) , 0 );
    if(!blur.empty()){
        imshow("gray + blur",blur);
    }else if(blur.empty()){
        std::cout<<"Error: 'turn gray to blur' blur.empty()"<<std::endl;
        return 1;
    }
    //显示blur处理后的图像

    // cv::Mat edged;
    // cv::Canny(blur, edged, 75, 200);
    // if(!edged.empty()){
    //     cv::imshow("edged",edged);
    // }else if(edged.empty()){
    //     std::cout<<"Error: Step cv::Canny 'turn blur to edged' edged.empty()"<<std::endl;
    //     return 1;
    // }


    // // convert to greyscale
    // cv::Mat converted, blurred, blackAndWhite;
    // converted = cv::Mat(img.rows, img.cols, CV_8UC1);
    // cv::cvtColor(img, converted, COLOR_BGR2GRAY );

    // // remove noise
    // cv::GaussianBlur(converted, blurred, cv::Size(3,3), 0);
    cv::Mat blackAndWhite=blur;
    Mat kernel
    // adaptive threshold    
    cv::adaptiveThreshold(blackAndWhite, blackAndWhite, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 15 , 9);
    kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3,3));
    morphologyEx(blackAndWhite, blackAndWhite, MORPH_OPEN, kernel, 5);
    imshow("2", blackAndWhite);


//***************图片预处理*******************
    int k = waitKey(0); // Wait for a keystroke in the window
    if(k == 's')
    {
        imwrite("Output.png", img);
    }
    return 0;
}