#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

int main(){

    cv::Mat imgInput = cv::imread("test.png");
    cv::cvtColor(imgInput, imgInput, CV_BGR2GRAY);
    cv::Mat imgThresh;
    adaptiveThreshold(imgInput, imgThresh, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY,11,15); //these values can be varied
    

    namedWindow("myWindow", cv::WINDOW_AUTOSIZE);
    char exit_key_press = 0;
    do {
        cv::imshow("myWindow", imgThresh);
        exit_key_press = cvWaitKey(1);
    }while (exit_key_press != '\x1b');

    return 0;
}