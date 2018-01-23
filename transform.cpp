#include <iostream>
#include <cmath>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#define PI 3.141592654f

cv::Mat getXRotation(float angle){
    cv::Mat R = cv::Mat::zeros(3, 3, CV_32F);
    float angleRad = angle*PI/180.0f;
   
    R.at<float>(0,0) = 1.0f;
    R.at<float>(1,1) = std::cos(angleRad);
    R.at<float>(1,2) = -sin(angleRad);
    R.at<float>(2,1) = sin(angleRad);
    R.at<float>(2,2) = cos(angleRad);
    return R;
}

cv::Mat getZRotation(float angle){
    cv::Mat R = cv::Mat::zeros(3, 3, CV_32F);
    float angleRad = angle*PI/180.0f;
    R.at<float>(2,2) = 1.0f;
    R.at<float>(0,0) = std::cos(angleRad);
    R.at<float>(0,1) = -std::sin(angleRad);
    R.at<float>(1,0) = std::sin(angleRad);
    R.at<float>(1,1) = std::cos(angleRad);
    return R;
}

cv::Mat getYRotation(float angle){
    cv::Mat R = cv::Mat::zeros(3, 3, CV_32F);
    float angleRad = angle*PI/180.0f;
    R.at<float>(1,1) = 1.0f;
    R.at<float>(0,0) = std::cos(angleRad);
    R.at<float>(2,0) = -std::sin(angleRad);
    R.at<float>(0,2) = std::sin(angleRad);
    R.at<float>(2,2) = std::cos(angleRad);
    return R;
}

int main(){
    cv::Mat Image = cv::imread("OriginalMarker/001.png");
    cv::Mat R = getXRotation(1.0f);
    cv::Mat dst;  

    cv::Mat T = cv::Mat::zeros(3,3, CV_32F);
    T.at<float>(0,0) = 1.0f;
    T.at<float>(1,1) = 1.0f;
    T.at<float>(2,2) = 1.0f;
    T.at<float>(0,2) = -Image.cols/2.0f;
    T.at<float>(1,2) = -Image.rows/2.0f;

    cv::Mat T2 =  cv::Mat::zeros(3,3, CV_32F);
    T2.at<float>(0,0) = 1.0f;
    T2.at<float>(1,1) = 1.0f;
    T2.at<float>(2,2) = 1.0f;
    T2.at<float>(0,2) = Image.cols/2.0f;
    T2.at<float>(1,2) = Image.rows/2.0f;

    cv::warpPerspective(Image, dst, T2*R*T, Image.size(), cv::INTER_CUBIC, cv::BORDER_CONSTANT);	


    namedWindow("myWindow", cv::WINDOW_AUTOSIZE);
    char exit_key_press = 0;
    do {
       imshow("myWindow", dst);
        exit_key_press = cvWaitKey(1);
    }while (exit_key_press != '\x1b');

    return 0;
}