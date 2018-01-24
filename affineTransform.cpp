#include <iostream>
#include <cmath>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "affineTransform.hpp"

float degToRad(float angleDegrees){
    return  angleDegrees*PI/180.0f;
}
float randomNumber0to1(){
    return (float)rand()/RAND_MAX ;
}
float randomNumber0tox(float x){
    return randomNumber0to1() * x ;
}
float randomNumberatob(float a, float b){
    return (b-a)*randomNumber0to1() + a ;
}

cv::Mat getXRotation(float angle){
    cv::Mat R = cv::Mat::zeros(3, 3, CV_32F);
    float angleRad = degToRad(angle);
    R.at<float>(0,0) = 1.0f;
    R.at<float>(1,1) = std::cos(angleRad);
    R.at<float>(1,2) = -std::sin(angleRad);
    R.at<float>(2,1) = std::sin(angleRad);
    R.at<float>(2,2) = std::cos(angleRad);
    return R;
}
cv::Mat getZRotation(float angle){
    cv::Mat R = cv::Mat::zeros(3, 3, CV_32F);
    float angleRad = degToRad(angle);
    R.at<float>(2,2) = 1.0f;
    R.at<float>(0,0) = std::cos(angleRad);
    R.at<float>(0,1) = -std::sin(angleRad);
    R.at<float>(1,0) = std::sin(angleRad);
    R.at<float>(1,1) = std::cos(angleRad);
    return R;
}
cv::Mat getYRotation(float angle){
    cv::Mat R = cv::Mat::zeros(3, 3, CV_32F);
    float angleRad = degToRad(angle);
    R.at<float>(1,1) = 1.0f;
    R.at<float>(0,0) = std::cos(angleRad);
    R.at<float>(2,0) = -std::sin(angleRad);
    R.at<float>(0,2) = std::sin(angleRad);
    R.at<float>(2,2) = std::cos(angleRad);
    return R;
}
cv::Mat getScale(float sx, float sy, float sz){
    cv::Mat S = cv::Mat::zeros(3,3,CV_32F);
    if(sx == 0) sx = 1; //prevents blank image
    if(sy == 0) sy = 1;
    if(sz == 0) sz = 1;
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

