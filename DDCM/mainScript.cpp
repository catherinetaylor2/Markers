#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <math.h>
#include <algorithm> 
#include <bits/stdc++.h>
#include <time.h> // for random

cv::RNG rng(12345);

static void draw_delaunay( cv::Mat& img, cv::Subdiv2D& subdiv, cv::Scalar delaunay_color, std::vector<cv::Vec6f> *T )
{
    std::vector<cv::Vec6f> triangleList;
    subdiv.getTriangleList(triangleList);
    std::vector<cv::Point> pt(3);
    cv::Size size = img.size();
    cv::Rect rect(0,0, size.width, size.height);
 
    for( size_t i = 0; i < triangleList.size(); i++ )
    {
        cv::Vec6f t = triangleList[i];
        pt[0] = cv::Point(cvRound(t[0]), cvRound(t[1]));
        pt[1] = cv::Point(cvRound(t[2]), cvRound(t[3]));
        pt[2] = cv::Point(cvRound(t[4]), cvRound(t[5]));
         
        // Draw rectangles completely inside the image.
        if ( rect.contains(pt[0]) && rect.contains(pt[1]) && rect.contains(pt[2]))
        {
            line(img, pt[0], pt[1], delaunay_color, 1, CV_AA, 0);
            line(img, pt[1], pt[2], delaunay_color, 1, CV_AA, 0);
            line(img, pt[2], pt[0], delaunay_color, 1, CV_AA, 0);
            (*T).push_back(triangleList[i]);
        }
    }
}

int calculateKey(std::vector<int> numberDots){
    int i = 0;
    for(int j = 0; j < numberDots.size(); ++j){
        i += (numberDots[j] - 1);
    }
    return i;
}

int findIndex(std::vector<cv::Point2f> centres, cv::Point2f P){
  int index = -1;
  for(int i = 0; i < centres.size(); ++i){
    if(centres [i] == P){
      index = i;
      return index;
    }
  }
  return index;
}

int main(){

    std::vector<int> test(6), outputTest(6);
    std::vector <int> IDs (6);
    IDs = {1,2,3,4,5,8};
    test = {4,3,4,2,4,3};

    std::unordered_map <int, std::vector<int> > hashTable;
    hashTable[calculateKey(test)] = IDs;

    std::cout<<"test A0 "<< calculateKey(test)<<"\n";

    IDs.clear();
    test.clear();

    IDs = {2,4,5,7,8,9};
    test = {3,2,4,1,3,1};
    hashTable[calculateKey(test)] = IDs;

    std::cout<<"test B0 "<< calculateKey(test)<<"\n";

    float maxContourArea = 60.0f;
    float minContourArea = 1.0f;
    float eMax = 1.0f;
    int contourThreshold = 100;
    int gridThreshold = 16;
    int gridWidth = 32;
    float controlParam = 1.0f;

    cv::Mat imgInput = cv::imread("test5.png");
    cv::cvtColor(imgInput, imgInput, CV_BGR2GRAY);
    cv::Mat imgThresh;
    adaptiveThreshold(imgInput, imgThresh, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY,25,5); //these values can be varied

    cv::Mat canny_edges;
    blur( imgThresh, imgThresh, cv::Size(3,3));

    std::vector<std::vector<cv::Point> > contours, contoursThresh,  finalContours;
    std::vector<cv::Vec4i> hierarchy;
    Canny( imgThresh, canny_edges, contourThreshold, contourThreshold*3, 7 , true);
    findContours( imgThresh, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);
     


  cv::Mat drawing = cv::Mat( imgInput.size(), CV_8UC3 );
  drawing.setTo(cv::Scalar(255,255,255));



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

    int cluster [ finalContours.size()];
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


    std::vector<std::vector< int > > markers(counter + 1);
    for(int i = 0; i < finalContours.size(); ++i){
        markers[cluster[i]].push_back(i); //each item contains vector with indices of dots in cluster
    }

    std::vector<cv::Point2f> markerCentre (counter + 1);
    cv::Point2f currentCentre; //contains centres of each cluster.
    for(int i = 0; i < counter + 1; ++i){
        currentCentre = cv::Point2f(0.0f, 0.0f);
        for(int j = 0; j < markers[i].size(); ++j){
            currentCentre += mc[(markers[i])[j]];
        }
        currentCentre.x /= markers[i].size();
        currentCentre.y /= markers[i].size();
        markerCentre[i] = currentCentre;
    }


    //Delauny triangulation
    cv::Rect rect(0,0, imgInput.cols, imgInput.rows);
    cv::Subdiv2D subdiv;
    subdiv.initDelaunay(rect); 
    subdiv.insert(markerCentre);
    std::vector<cv::Vec6f>  T;

    draw_delaunay(drawing, subdiv, cv::Scalar(0,0,0), &T);
    
    cv::Point2f indices[3], indices2[3];

    std::vector< cv::Vec4f >edgeList ;
    subdiv.getEdgeList(edgeList);

    std::vector <std::vector <int> > adjT (T.size());
    std::vector <std::vector <int> > triangleIndices (T.size());


    for(int i = 0; i< T.size(); ++i){
      
      cv::Vec6f t = T[i];
      indices[0] = cv::Point2f(t[0], t[1]);
      indices[1] = cv::Point2f(t[2], t[3]);
      indices[2] = cv::Point2f(t[4], t[5]);

      for (int ii = 0; ii < 3; ++ii){
        (triangleIndices[i]).push_back((findIndex(markerCentre, indices[ii])));
      }

      for(int j =0 ; j < T.size(); ++j){
        if(i!=j){
          cv::Vec6f t2 = T[j];
          indices2[0] = cv::Point2f(t2[0], t2[1]);
          indices2[1] = cv::Point2f(t2[2], t2[3]);
          indices2[2] = cv::Point2f(t2[4], t2[5]);
          int matchingIndices = 0;

          for(int k = 0; k < 3; ++k){
            for( int l = 0; l < 3; ++l){
              if(indices[k] == indices2[l]){
                matchingIndices++;
              }
            }
          }
          if(matchingIndices == 2){
            adjT[i].push_back(j);
          }

        }
      }
         
    }
    std::vector <std::vector<int> > indicesT;
    std::list<int> tempList;
    std::vector<int> tempVector;
    int adjNo = 0;
    for(int i = 0; i< T.size(); ++i){
      if(adjT[i].size() == 3){
        tempList.clear();
        tempVector.clear();
        for(int j = 0 ; j < 3; ++j){
          tempList.push_back((triangleIndices[i])[j]);
        }
        for(int k = 0; k < 3; ++k){
          for(int j = 0 ; j < 3; ++j){
            tempList.push_back((triangleIndices[(adjT[i])[k]])[j]);
          }
        }
       
        tempList.sort();
        tempList.unique();

        for (std::list<int>::iterator it=tempList.begin(); it!=tempList.end(); ++it){
          tempVector.push_back(markers[*it].size());
        }
        adjNo++;
        indicesT.push_back(tempVector);
      }
    }
  
    

    for(int i = 0 ; i < indicesT.size(); ++i){
      outputTest = hashTable[calculateKey(indicesT[i])];
    }
  

    cv::Scalar colour ;


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
        drawContours( drawing, finalContours, i, colour, 1, 8, hierarchy, 0, cv::Point());
        
 
    }

    for(int i =0 ; i< T.size(); ++i){
      std::string str = std::to_string(i);
      putText(drawing, str, markerCentre[i], cv::FONT_HERSHEY_SIMPLEX,1,cv::Scalar(0,0,0),2, cv::LINE_AA);
    }

    for(int i = 0; i< counter +1; ++i){
        circle(drawing, markerCentre[i], 30, cv::Scalar(0,0,0), 1, 8, 0 );
    }


    
    namedWindow("myWindow", cv::WINDOW_AUTOSIZE);
    char exit_key_press = 0;
    do {
        cv::imshow("myWindow", drawing);
        exit_key_press = cvWaitKey(1);
    }while (exit_key_press != '\x1b');


    return 0;
}