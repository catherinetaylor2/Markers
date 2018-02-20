#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <math.h>
#include <algorithm> 

cv::RNG rng(12345);

int main(){

    float maxContourArea = 60.0f;
    float minContourArea = 1.0f;
    float eMax = 1.0f;
    int contourThreshold = 100;
    int gridThreshold = 16;
    int gridWidth = 32;
    float controlParam = 1.0f;

    cv::Mat imgInput = cv::imread("test3.png");
    cv::cvtColor(imgInput, imgInput, CV_BGR2GRAY);
    cv::Mat imgThresh;
    adaptiveThreshold(imgInput, imgThresh, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY,25,5); //these values can be varied

    cv::Mat canny_edges;
    blur( imgThresh, imgThresh, cv::Size(3,3));

    std::vector<std::vector<cv::Point> > contours, contoursThresh,  finalContours;
    std::vector<cv::Vec4i> hierarchy;
    Canny( imgThresh, canny_edges, contourThreshold, contourThreshold*3, 7 , true);
    findContours( imgThresh, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);
     
    std::vector<float> radius;
    for( int i = 0; i< contours.size(); i++ ){
        if((contourArea(contours[i]) < maxContourArea) && (contourArea(contours[i]) > minContourArea)){
            contoursThresh.push_back(contours[i]);
            radius.push_back(sqrt(contourArea(contours[i])/3.1415926f));
       }
    }

    std::vector<cv::Moments> mu(contoursThresh.size() );
    for( int i = 0; i < contoursThresh.size(); i++ ){ 
        mu[i] = moments( contoursThresh[i], true ); 
    }

    std::vector<cv::Point2f> mc( contoursThresh.size() );
    std::vector< cv::Point2f> geometricDeviation( contoursThresh.size() );
    std::vector<float> eccentricty(contoursThresh.size());
    for( int i = 0; i < contoursThresh.size(); i++ ){ 
        mc[i] = cv::Point2f( mu[i].m10/mu[i].m00 , mu[i].m01/mu[i].m00 ); 
        geometricDeviation[i] =  cv::Point2f( sqrt(mu[i].m20 / mu[i].m00), sqrt(mu[i].m02 / mu[i].m00));
        eccentricty[i] = ((mu[i].m20 - mu[i].m02)*(mu[i].m20 - mu[i].m02) + 4*mu[i].m11*mu[i].m11)/((mu[i].m20 + mu[i].m02)*(mu[i].m20 + mu[i].m02));
        if(eccentricty[i]<eMax){
            finalContours.push_back(contoursThresh[i]);
        }
    }

    int cluster [contoursThresh.size()];
    cluster[0] = 0;
    int counter = 0;    
    float dist, pMax;

    for(int i = 1 ; i < finalContours.size(); ++i){
        pMax = radius[i];
        bool clusterFound = false;
        for(int j = i - 1; j > -1; --j){
            if(!clusterFound){
               dist = sqrt((mc[i].x - mc[j].x)*(mc[i].x - mc[j].x) + (mc[i].y - mc[j].y)*(mc[i].y - mc[j].y));
               if(dist < pMax*8){
                    cluster[i] = cluster[j];
                   clusterFound = true;;
               }
            }
        }
        if(!clusterFound){
            counter++;
            cluster[i] = counter;
        }
    }

    cv::Scalar colour ;

    cv::Mat drawing2 = cv::Mat( imgInput.size(), CV_8UC3 );
    drawing2.setTo(cv::Scalar(255,255,255));
    for( int i = 0; i< finalContours.size(); i++ ){
        if(cluster[i] % 5 == 0){
            colour = cv::Scalar(255,0,0);
        }
        if(cluster[i] % 5 == 1){
            colour = cv::Scalar(0,255,0);
        }
        if(cluster[i] % 5 == 2){
            colour = cv::Scalar(0,0,255);
        }
        if(cluster[i] % 5 == 3){
            colour = cv::Scalar(255,255,0);
        }
        if(cluster[i] % 5 == 4){
            colour = cv::Scalar(0,0,0);
        }
        cv::Scalar color = cv::Scalar( 255, 0, 0);
        drawContours( drawing2, finalContours, i, colour, 1, 8, hierarchy, 0, cv::Point());
 
    }
    
    namedWindow("myWindow", cv::WINDOW_AUTOSIZE);
    char exit_key_press = 0;
    do {
        cv::imshow("myWindow", drawing2);
        exit_key_press = cvWaitKey(1);
    }while (exit_key_press != '\x1b');

    return 0;
}