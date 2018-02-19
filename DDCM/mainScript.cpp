#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <math.h>
#include <algorithm> 

cv::RNG rng(12345);

int main(){

    float maxContourArea = 15.0f;
    float minContourArea = 1.0f;
    float eMax = 1.0f;
    int contourThreshold = 100;
    int gridThreshold = 16;
    int gridWidth = 32;
    float controlParam = 1.0f;

    cv::Mat imgInput = cv::imread("test.png");
    cv::cvtColor(imgInput, imgInput, CV_BGR2GRAY);
    cv::Mat imgThresh;
    adaptiveThreshold(imgInput, imgThresh, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY,25,5); //these values can be varied

    cv::Mat canny_edges;
    blur( imgThresh, imgThresh, cv::Size(3,3));

    std::vector<std::vector<cv::Point> > contours, contoursThresh,  finalContours;
    std::vector<cv::Vec4i> hierarchy;
    Canny( imgThresh, canny_edges, contourThreshold, contourThreshold*3, 3 , true);
    findContours( imgThresh, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);
     
    for( int i = 0; i< contours.size(); i++ ){
        if((contourArea(contours[i]) < maxContourArea) && (contourArea(contours[i]) > minContourArea)){
            contoursThresh.push_back(contours[i]);
       }
    }

    std::vector<cv::Moments> mu(contoursThresh.size() );
    for( int i = 0; i < contoursThresh.size(); i++ ){ 
        mu[i] = moments( contoursThresh[i], false ); 
    }

    std::vector<cv::Point2f> mc( contoursThresh.size() );
    std::vector< cv::Point2f> geometricDeviation( contoursThresh.size() );
    std::vector<float> eccentricty(contoursThresh.size());
    for( int i = 0; i < contoursThresh.size(); i++ ){ 
        mc[i] = cv::Point2f( mu[i].m10/mu[i].m00 , mu[i].m01/mu[i].m00 ); 
        geometricDeviation[i] =  cv::Point2f(sqrt(mu[i].m20 / mu[i].m00), sqrt(mu[i].m02 / mu[i].m00));
        eccentricty[i] = ((mu[i].m20 - mu[i].m02)*(mu[i].m20 - mu[i].m02) + 4*mu[i].m11*mu[i].m11)/((mu[i].m20 + mu[i].m02)*(mu[i].m20 + mu[i].m02));
        if(eccentricty[i]<eMax){
            finalContours.push_back(contoursThresh[i]);
        }
    }

    // for(int i = 0; i<imgInput.rows; i+=gridWidth){
    //     for(int j = 0; j < imgInput.cols; j+=gridWidth){
    //         int keyPoints = 0;
    //         for(int k = 0; k < contoursThresh.size(); k++ ){
    //             if((mc[k].x < j + gridWidth) &&(mc[k].x >= j)&&(mc[k].y < i + gridWidth) &&(mc[k].y >= i)){
    //                 keyPoints++;
    //             }
    //            // std::cout<<"keyPoints "<<keyPoints<<"\n";
    //         }
    //     }
    // }

    int cluster [contoursThresh.size()];
    cluster[0] = 0;
    int counter = 0;
    float dist, pMax;


    for(int i = 1 ; i < contoursThresh.size(); ++i){
        pMax = std::max(geometricDeviation[i].x, geometricDeviation[i].y);
        bool clusterFound = false;
        for(int j = i - 1; j > -1; --j){
            if(!clusterFound){
               dist = sqrt((mc[i].x - mc[j].x)*(mc[i].x - mc[j].x) - (mc[i].y - mc[j].y)*(mc[i].y - mc[j].y));
               std::cout<<"dist "<<dist<<" pMax "<<pMax<<"\n";
               if(dist < pMax*controlParam){
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
    std::cout<<"counter "<<counter<<"\n";
    cv::Scalar colour ;

    cv::Mat drawing2 = cv::Mat( imgInput.size(), CV_8UC3 );
    drawing2.setTo(cv::Scalar(255,255,255));
    for( int i = 0; i< finalContours.size(); i++ ){
        if(cluster[i] % 3 == 0){
            colour = cv::Scalar(255,0,0);
        }
        if(cluster[i] % 3 == 1){
            colour = cv::Scalar(0,255,0);
        }
        if(cluster[i] % 3 == 2){
            colour = cv::Scalar(0,0,255);
        }
        cv::Scalar color = cv::Scalar( 255, 0, 0);
       drawContours( drawing2, finalContours, i, colour, 1, 8, hierarchy, 0, cv::Point() );
       // drawContours( drawing2, finalContours, i, cv::Scalar(0,0,0), 1, 8, hierarchy, 0, cv::Point() );
 
    }


    namedWindow("myWindow", cv::WINDOW_AUTOSIZE);
    char exit_key_press = 0;
    do {
        cv::imshow("myWindow", drawing2);
        exit_key_press = cvWaitKey(1);
    }while (exit_key_press != '\x1b');

    return 0;
}