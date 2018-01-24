#include <iostream>
#include <cmath>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#define PI 3.141592654f

float randomNumber0to1(){
    return (float)rand()/RAND_MAX ;
}

float randomNumber0to360(){
    return (float)rand()/RAND_MAX * 360.0f ;
}

cv::Mat getXRotation(float angle){
    cv::Mat R = cv::Mat::zeros(3, 3, CV_32F);
    float angleRad = angle*PI/180.0f;
    R.at<float>(0,0) = 1.0f;
    R.at<float>(1,1) = std::cos(angleRad);
    R.at<float>(1,2) = -std::sin(angleRad);
    R.at<float>(2,1) = std::sin(angleRad);
    R.at<float>(2,2) = std::cos(angleRad);
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

cv::Mat getScale(float sx, float sy, float sz){
    cv::Mat S = cv::Mat::zeros(3,3,CV_32F);
    S.at<float>(0,0) = sx;
    S.at<float>(1,1) = sy;
    S.at<float>(2,2) = sz;
    return S;   
}

cv::Mat getShear(float sxy, float sxz, float syx, float syz, float szx, float szy){
    cv::Mat S = cv::Mat::ones(3,3, CV_32F);
    S.at<float>(0,1) = sxy;
    S.at<float>(0,2) = sxz;
    S.at<float>(1,0) = syx;
    S.at<float>(1,2) = syz;
    S.at<float>(2,0) = szx;
    S.at<float>(2,1) = szy;
    return S;
}

int main(){
    cv::Mat Image = cv::imread("OriginalMarker/001.png");
   
    cv::Mat T = cv::Mat::zeros(3,3, CV_32F); //translate to centre of image
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

    cv::Mat R = getZRotation(randomNumber0to360());
    cv::Mat dst;  

    cv::Mat S = getShear(2.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);

   // for(int i=0; i<5;i++)
    //std::cout<<randomNumber0to360()<<"\n";
    


    cv::warpPerspective(Image, dst, T2*R*T, Image.size(), cv::INTER_CUBIC, cv::BORDER_CONSTANT, cv::Scalar(255, 255, 255));	//set background to white

    namedWindow("myWindow", cv::WINDOW_AUTOSIZE);
    char exit_key_press = 0;
    do {
       imshow("myWindow", dst);
        exit_key_press = cvWaitKey(1);
    }while (exit_key_press != '\x1b');

    return 0;
}