#include <iostream>
#include <cmath>
#include <string>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "affineTransform.hpp"

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

    cv::Mat Rx, Ry, Rz, Scale, Shear, R, dst;

    for(int i = 0; i < 5; ++i){

        Rx = getXRotation(randomNumber0to1());
        Ry = getYRotation(randomNumber0to1());
        Rz = getZRotation(randomNumber0tox(360));
        Scale = getScale(0, randomNumberatob(-5,5), randomNumberatob(-5,5));
        Shear = getShear(randomNumberatob(-5,5), 0, randomNumberatob(-5,5), 0, 0, 0);

        R = Shear*Scale*Rx*Ry*Rz;
         
        cv::warpPerspective(Image, dst, T2*R*T, Image.size(), cv::INTER_CUBIC, cv::BORDER_CONSTANT, cv::Scalar(255, 255, 255));	//set background to white
        std::string filename = "TransformedMarker/" + std::to_string(i) + ".jpg";
        cv::imwrite(filename, dst);

        namedWindow("myWindow", cv::WINDOW_AUTOSIZE);
        char exit_key_press = 0;
        do {
            imshow("myWindow", dst);
            exit_key_press = cvWaitKey(1);
        }while (exit_key_press != '\x1b');
    }
    return 0;
}