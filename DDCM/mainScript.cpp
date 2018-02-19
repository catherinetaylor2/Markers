#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <math.h>   

int main(){

    float maxContourArea = 15.0f;
    float minContourArea = 1.0f;
    int contourThreshold = 100;

    cv::Mat imgInput = cv::imread("test.png");
    cv::cvtColor(imgInput, imgInput, CV_BGR2GRAY);
    cv::Mat imgThresh;
    adaptiveThreshold(imgInput, imgThresh, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY,11,15); //these values can be varied

    cv::Mat canny_edges;
    blur( imgThresh, imgThresh, cv::Size(3,3));

    std::vector<std::vector<cv::Point> > contours;
    std::vector<cv::Vec4i> hierarchy;
    Canny( imgThresh, canny_edges, contourThreshold, contourThreshold*3, 3 , true);
    findContours( imgThresh, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);
     
    cv::Mat drawing2 = 255*cv::Mat::ones( imgThresh.size(), CV_8UC1 );
    for( int i = 0; i< contours.size(); i++ ){
        if((contourArea(contours[i]) < maxContourArea) && (contourArea(contours[i]) > minContourArea)){
            drawContours( drawing2, contours, i, cv::Scalar(0,0,0), 1, 8, hierarchy, 0, cv::Point() );
       }
    }


    namedWindow("myWindow", cv::WINDOW_AUTOSIZE);
    char exit_key_press = 0;
    do {
        cv::imshow("myWindow", drawing2);
        exit_key_press = cvWaitKey(1);
    }while (exit_key_press != '\x1b');

    return 0;
}