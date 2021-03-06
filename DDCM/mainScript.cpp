#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <math.h>
#include <algorithm>
#include <bits/stdc++.h>
#include <time.h> // for random
#include <fstream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

cv::RNG rng(12345);
#define PI 3.141592654f

class DotCluster
{
public:
	DotCluster(){};
	DotCluster(int _id, cv::Point2f _position, int numberOfClusters){
		dotNumber = _id;
		position = _position;
	}
	int dotNumber;
	cv::Point2f position;
    cv::Point2f dev;
    std::vector<int> votes;
    int id;
    int modeCount;
   
};
class DC
{
public:
    DC(){
        status = "lost";
        position = cv::Point2f(-1.0f, -1.0f);
        velocity = cv::Point2f(-1.0f, -1.0f);
        deviation = cv::Point2f(0.0f, 0.0f);
    }
    int id;
    std::string status;
    cv::Point2f position;
    cv::Point2f velocity;
    cv::Point2f deviation;
};

static void draw_delaunay( cv::Mat& img, cv::Subdiv2D& subdiv, cv::Scalar delaunay_color, std::vector<cv::Vec6f> *T ){
    std::vector<cv::Vec6f> triangleList;
    subdiv.getTriangleList(triangleList);
    std::vector<cv::Point> pt(3);
    cv::Size size = img.size();
    cv::Rect rect(0,0, size.width, size.height);

    for( size_t i = 0; i < triangleList.size(); i++ ){
        cv::Vec6f t = triangleList[i];
        pt[0] = cv::Point(cvRound(t[0]), cvRound(t[1]));
        pt[1] = cv::Point(cvRound(t[2]), cvRound(t[3]));
        pt[2] = cv::Point(cvRound(t[4]), cvRound(t[5]));

        // Draw rectangles completely inside the image.
        if ( rect.contains(pt[0]) && rect.contains(pt[1]) && rect.contains(pt[2])){
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
        i += (numberDots[j] - 1)*pow(3, j-1);
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

int myMode(std::vector<int> input, int* numberOfVotes){
    std::vector<int> votes ;
    for(int i = 0; i <input.size(); ++i){
        for(int j = 0; j < input[i]; ++j){
            votes.push_back(i+1);
        }
    }
    if(votes.size() == 0){
        return -1;
    }
    std::sort(votes.begin(), votes.end());

    int number = votes[0], modeVal = number;
    int count = 1, countMode = 1;
    int prevMax = 0;
    for (int i=1; i<votes.size(); i++){
        if(votes[i] == number) {
            ++count;
        }
        else{
            if (count > countMode){
                countMode = count;
                modeVal = number;
            }
            else if(count == countMode){
                prevMax = count;
            }
            count = 1;
            number = votes[i];
        }
    }
    if (count > countMode){
        modeVal = number;
    }
    else if(count == countMode){
        prevMax = count;
    }
    (*numberOfVotes) = countMode;
    if(prevMax < countMode){
        return modeVal;
    }
    else{
        return -1;
    }
}

std::vector<int> getPermutation(std::vector<int> input, int permutation){
    if(input.size() == 0){
        return input;
    }
    std::vector<int> output;
    if(permutation % 3 == 0){
        return input;
    }
    else if(permutation % 3 == 1){
        for(int i = 2; i<input.size(); ++i){
            output.push_back(input[i]);
        }
        for(int i = 0; i <2; ++i){
            output.push_back(input[i]);
        }
        return output;
    }
    else{
        for(int i = 4; i < input.size(); ++i){
         output.push_back(input[i]);
        }
        for(int i = 0; i <4; ++i){
            output.push_back(input[i]);
        }
        return output;
    }

}

std::vector<int> getTriangleOrder(std::vector<int> input, int triangles[3], std::vector <std::vector <int> > triangleIndices){
    std::vector<int> output;
    int A, B, t, X, triangle;
    std::vector<int> T;

    for(int i = 0; i < 3; ++i){
        A = input[i];
        B = input[(i+1)%3];
        for(int j = 0; j < 3; ++j){
            t = triangles[j];
            T = triangleIndices[t];
            if(((A == T[0])||(A == T[1]) ||(A==T[2]))&&((B == T[0])||(B == T[1])|| (B == T[2]))){
                for(int k = 0; k < 3; ++k){
                    if((T[k]!=A)&&(T[k]!=B)){
                        output.push_back(T[k]);
                    }
                }
                output.push_back(B);
            }
        }
    }
    return output;
}

void removeMultipleVotes(std::vector<DotCluster> * input){
    int SIZE = (*input).size();
    int currentID, currentMax;
    for(int i = 0; i < SIZE; ++i){
        currentID = (*input)[i].id;
        currentMax =(*input)[i].modeCount;
        if (currentID != -1){
            for(int j = i + 1; j <SIZE; ++j){
                if((*input)[j].id == currentID){
                    if((*input)[j].modeCount < currentMax){
                        (*input)[j].id = -1;
                    }
                    else if ((*input)[j].modeCount == currentMax){
                        (*input)[j].id = -1;
                        (*input)[i].id = -1;
                    }
                    else{
                        (*input)[i].id = -1;
                    }
                }
            }
        }
    }
}

int fillHashtable( std::unordered_map <int, std::vector<int> > * hashTable, const char* IdFilename, const char * markersFilename, const char * patFilename){
    std::vector<int> test, IDs ;
    int key, width, height, j = 0, a, b, c;
    std::fstream myfile(IdFilename, std::ios_base::in);
    std::fstream myfile2(markersFilename, std::ios_base::in);
    std::fstream myfile3(patFilename, std::ios_base::in);

    while(myfile3 >> c){
        if(j == 0){
            width = c;
        }
        else if (j ==1 ){
            height = c;
        }
        ++j;
    }

    int i = 0;
    while (myfile >> a){
        myfile2 >> b;
        if(i%6 == 0 ){
            if(test.size()>0){
                for(int j = 0; j < 3; ++j){
                    key = calculateKey(getPermutation(test, j));
                    (*hashTable)[key] = IDs;
                }
                IDs.clear();
                test.clear();
            }
        }
        test.push_back(b);
        IDs.push_back(a);
        ++i;
    }

    return width*height;
}


int main(){

    //INITIALISE THE GRID-----------------------------------------
    //NOTE: ID indexing begins at 1

    std::vector<int> outputTest(6);
    std::unordered_map <int, std::vector<int> > hashTable;
    int numberOfClusters =  fillHashtable(&hashTable, "markerIDs.txt", "markerClusters.txt", "pattern.text");

    std::vector<DC> clusterData(numberOfClusters);
    for(int i = 0; i<numberOfClusters ; ++i){
        clusterData[i].id = i + 1;
    }
   // std::cout<<"lost "<<clusterData[0].status<<"\n";

    // IDs =  {1,2,3,9,15,8}; test = {1,2,3,4,1,7};
    // for(int i = 0; i < 3; ++i){
    //     key = calculateKey(getPermutation(test, i));
    //     hashTable[key] = IDs;
    // }
    // IDs.clear();
    // test.clear();

    // IDs =  {2,3,4,10,16,9}; test = {2,3,7,5,6,4};
    // for(int i = 0; i < 3; ++i){
    //     key = calculateKey(getPermutation(test, i));
    //     hashTable[key] = IDs;
    // }
    // IDs.clear();
    // test.clear();

    // IDs =  {3,4,5,11,17,10}; test = {3,7,4,1,2,5};
    // for(int i = 0; i < 3; ++i){
    //     key = calculateKey(getPermutation(test, i));
    //     hashTable[key] = IDs;
    // }
    // IDs.clear();
    // test.clear();

    // IDs =  {4,5, 6,12,18,11}; test = {7,4,6,3,6,1};
    // for(int i = 0; i < 3; ++i){
    //     key = calculateKey(getPermutation(test, i));
    //     hashTable[key] = IDs;
    // }
    // IDs.clear();
    // test.clear();

    // IDs =  {5,6,7,13,19,12}; test = {4,6,2,4,2,3};
    // for(int i = 0; i < 3; ++i){
    //     key = calculateKey(getPermutation(test, i));
    //     hashTable[key] = IDs;
    // }
    // IDs.clear();
    // test.clear();

    // IDs =  {2,9,16,15,14,18}; test = {2,4,6,1,5,7};
    // for(int i = 0; i < 3; ++i){
    //     key = calculateKey(getPermutation(test, i));
    //     hashTable[key] = IDs;
    // }
    // IDs.clear();
    // test.clear();

    // IDs =  {8,9,10,16,21,15}; test = {7,4,5,6,3,1};
    // for(int i = 0; i < 3; ++i){
    //     key = calculateKey(getPermutation(test, i));
    //     hashTable[key] = IDs;
    // }
    // IDs.clear();
    // test.clear();

    // IDs =  {3,10,17,16,15,9}; test = {3,5,2,6,1,4};
    // for(int i = 0; i < 3; ++i){
    //     key = calculateKey(getPermutation(test, i));
    //     hashTable[key] = IDs;
    // }
    // IDs.clear();
    // test.clear();

    // IDs =  {9,10,11,17,22,16}; test = {4,5,1,2,4,6};
    // for(int i = 0; i < 3; ++i){
    //     key = calculateKey(getPermutation(test, i));
    //     hashTable[key] = IDs;
    // }
    // IDs.clear();
    // test.clear();

    // IDs =  {4,11,18, 17, 16, 10}; test = {7,1,6,2,6,5};
    // for(int i = 0; i < 3; ++i){
    //     key = calculateKey(getPermutation(test, i));
    //     hashTable[key] = IDs;
    // }
    // IDs.clear();
    // test.clear();

    // IDs =  {10,11,12,18,23,17}; test = {3,1,3,6,5,2};
    // for(int i = 0; i < 3; ++i){
    //     key = calculateKey(getPermutation(test, i));
    //     hashTable[key] = IDs;
    // }
    // IDs.clear();
    // test.clear();

    // IDs =  {5,12,19,18,17,11}; test = {4,3,2,6,2,1};
    // for(int i = 0; i < 3; ++i){
    //     key = calculateKey(getPermutation(test, i));
    //     hashTable[key] = IDs;
    // }
    // IDs.clear();
    // test.clear();

    // IDs =  {11,12,13,19,24,18}; test = {1,3,4,2,7,6};
    // for(int i = 0; i < 3; ++i){
    //     key = calculateKey(getPermutation(test, i));
    //     hashTable[key] = IDs;
    // }
    // IDs.clear();
    // test.clear();

    // IDs =  {6,13,25,19,18,12}; test = {6,4,2,2,6,3};
    // for(int i = 0; i < 3; ++i){
    //     key = calculateKey(getPermutation(test, i));
    //     hashTable[key] = IDs;
    // }
    // IDs.clear();
    // test.clear();

    // IDs =  {1,8,9,15,20,14}; test = {1,7,4,1,4,5};
    // for(int i = 0; i < 3; ++i){
    //     key = calculateKey(getPermutation(test, i));
    //     hashTable[key] = IDs;
    // }
    // IDs.clear();
    // test.clear();

    // IDs =  {8,15,21,20,26,14}; test = {7,1,3,4,3,5};
    // for(int i = 0; i < 3; ++i){
    //     key = calculateKey(getPermutation(test, i));
    //     hashTable[key] = IDs;
    // }
    // IDs.clear();
    // test.clear();

    // IDs =  {14,15,16,21,27,20}; test = {5,1,6,3,1,4};
    // for(int i = 0; i < 3; ++i){
    //     key = calculateKey(getPermutation(test, i));
    //     hashTable[key] = IDs;
    // }
    // IDs.clear();
    // test.clear();

    // IDs =  {9,16,22,21,20,15}; test = {4,6,4,3,4,1};
    // for(int i = 0; i < 3; ++i){
    //     key = calculateKey(getPermutation(test, i));
    //     hashTable[key] = IDs;
    // }
    // IDs.clear();
    // test.clear();

    // IDs =  {15,16,17,22,28,21}; test = {1,6,2,4,4,3};
    // for(int i = 0; i < 3; ++i){
    //     key = calculateKey(getPermutation(test, i));
    //     hashTable[key] = IDs;
    // }
    // IDs.clear();
    // test.clear();

    // IDs =  {10,17,23,22,21,16}; test = {5,2,5,4,3,6};
    // for(int i = 0; i < 3; ++i){
    //     key = calculateKey(getPermutation(test, i));
    //     hashTable[key] = IDs;
    // }
    // IDs.clear();
    // test.clear();

    // IDs =  {16,17,18,23,29,22}; test = {6,2,6,5,7,4};
    // for(int i = 0; i < 3; ++i){
    //     key = calculateKey(getPermutation(test, i));
    //     hashTable[key] = IDs;
    // }
    // IDs.clear();
    // test.clear();

    // IDs =  {11,18,24,23,22,17}; test = {1,6,7,5,4,2};
    // for(int i = 0; i < 3; ++i){
    //     key = calculateKey(getPermutation(test, i));
    //     hashTable[key] = IDs;
    // }
    // IDs.clear();
    // test.clear();

    // IDs =  {17,18,19,24,30,23}; test = {2,6,2,7,2,5};
    // for(int i = 0; i < 3; ++i){
    //     key = calculateKey(getPermutation(test, i));
    //     hashTable[key] = IDs;
    // }
    // IDs.clear();
    // test.clear();

    // IDs =  {12,19,25,24,23,18}; test = {3,2,2,7,5,6};
    // for(int i = 0; i < 3; ++i){
    //     key = calculateKey(getPermutation(test, i));
    //     hashTable[key] = IDs;
    // }
    // IDs.clear();
    // test.clear();

    // IDs =  {18,19,13,25,31,24}; test = {6,2,4,2,3,7};
    // for(int i = 0; i < 3; ++i){
    //     key = calculateKey(getPermutation(test, i));
    //     hashTable[key] = IDs;
    // }
    // IDs.clear();
    // test.clear();

    // IDs =  {15,21,28,27,26,20}; test = {1,3,4,1,3,4};
    // for(int i = 0; i < 3; ++i){
    //     key = calculateKey(getPermutation(test, i));
    //     hashTable[key] = IDs;
    // }
    // IDs.clear();
    // test.clear();

    // IDs =  {16,22,29,28,27,21}; test = {6,4,7,4,1,3};
    // for(int i = 0; i < 3; ++i){
    //     key = calculateKey(getPermutation(test, i));
    //     hashTable[key] = IDs;
    // }
    // IDs.clear();
    // test.clear();

    // IDs =  {17,23,30,29,28,22}; test = {2,5,2,7,4,4};
    // for(int i = 0; i < 3; ++i){
    //     key = calculateKey(getPermutation(test, i));
    //     hashTable[key] = IDs;
    // }
    // IDs.clear();
    // test.clear();

    // IDs =  {18,24,31,30,29,23}; test = {6,7,3,2,7,5};
    // for(int i = 0; i < 3; ++i){
    //     key = calculateKey(getPermutation(test, i));
    //     hashTable[key] = IDs;
    // }
    //--------------------------------------------------------


    float maxContourArea = 1000.0f;
    float minContourArea = 10.0f;
    float eMax = 0.1f;
    int contourThreshold = 100;
    int gridThreshold = 32;
    int gridWidth = 32;
    float controlParam = 1.0f;

    cv::Mat imgInput, imgThresh, canny_edges, drawing;
    std::vector<std::vector<cv::Point> > contours, contoursThresh,  finalContours;
    std::vector<cv::Vec4i> hierarchy;

    imgInput = cv::imread("pattern.jpg");
    cv::cvtColor(imgInput, imgInput, CV_BGR2GRAY);
    adaptiveThreshold(imgInput, imgThresh, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY,25,5); //these values can be varied
    blur( imgThresh, imgThresh, cv::Size(3,3));
    Canny( imgThresh, canny_edges, contourThreshold, contourThreshold*3, 7 , true);
    findContours( imgThresh, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);

    drawing = cv::Mat( imgInput.size(), CV_8UC3 );
    drawing.setTo(cv::Scalar(255,255,255));

    std::vector<float> radius;
    int TotalNumberContours = contours.size();
    for( int i = 0; i< TotalNumberContours; i++ ){
        if((contourArea(contours[i]) < maxContourArea) && (contourArea(contours[i]) > minContourArea)){
            contoursThresh.push_back(contours[i]);
        }
    }

    int NumberOfThreshContours = contoursThresh.size();
    std::vector<cv::Moments> mu(NumberOfThreshContours);
    std::vector<cv::Point2f> mc(NumberOfThreshContours);
    for( int i = 0; i < NumberOfThreshContours; i++ ){
        mu[i] = moments( contoursThresh[i], true );
    }
    for( int i = 0; i <  NumberOfThreshContours; i++ ){
        mc[i] = cv::Point2f( mu[i].m10/mu[i].m00 , mu[i].m01/mu[i].m00 );
    }

    std::vector<std::vector <int> > indicesInGrid;
    std::vector<int> tempGridIndices;
    int pointsInGrid;
    for(int i = 0; i < imgInput.cols; i+= gridWidth){
        pointsInGrid = 0;
        tempGridIndices.clear();
        for(int j = 0; j < imgInput.rows; j+= gridWidth){
            for(int k = 0; k< NumberOfThreshContours; ++k){
                if((mc[k].x >= i)&&(mc[k].x < i + gridWidth)&&(mc[k].y >= j)&&(mc[k].y < j + gridWidth)){
                    tempGridIndices.push_back(k);
                    ++pointsInGrid;
                }
            }
        }
        if(pointsInGrid < gridThreshold){
            indicesInGrid.push_back(tempGridIndices);
        }
    }

    std::vector< cv::Point2f> geometricDeviation;//( NumberOfThreshContours);
    std::vector<float> eccentricty;//(NumberOfThreshContours);
    int gridIndex;
    float Eccentricity;
    cv::Point2f GeometricDeviation;
    std::vector<cv::Point2f> final_mc;
    for(int i = 0; i < indicesInGrid.size(); ++i){
        if(indicesInGrid[i].size() > 0){
            for(int j = 0; j < indicesInGrid[i].size(); ++j){
                gridIndex = indicesInGrid[i][j];
                Eccentricity = ((mu[gridIndex].mu20 - mu[gridIndex].mu02)*(mu[gridIndex].mu20 - mu[gridIndex].mu02) + 4*mu[gridIndex].mu11*mu[gridIndex].mu11)/((mu[gridIndex].mu20 + mu[gridIndex].mu02)*(mu[gridIndex].mu20 + mu[gridIndex].mu02));
                if(Eccentricity < eMax){
                    GeometricDeviation = cv::Point2f( sqrt(mu[i].mu20 / mu[i].m00), sqrt(mu[i].mu02 / mu[i].m00));
                   // std::cout<<"X "<<GeometricDeviation.x<<" Y "<<GeometricDeviation.y<<"\n";
                    geometricDeviation.push_back(GeometricDeviation);
                    finalContours.push_back(contoursThresh[gridIndex]);
                    eccentricty.push_back(Eccentricity);
                    radius.push_back(sqrt(contourArea(contoursThresh[gridIndex])/PI));
                    final_mc.push_back(mc[gridIndex]);
                }
            }
        }
    }

    int FinalContourNumber = finalContours.size();
    int cluster [FinalContourNumber];
    cluster[0] = 0;
    int counter = 0;
    float pMax, dist;

    for(int i = 1 ; i < FinalContourNumber; ++i){
        pMax = radius[i];
        bool clusterFound = false;
        for(int j = i - 1; j > -1; --j){
            if(!clusterFound){
                dist = sqrt((final_mc[i].x - final_mc[j].x)*(final_mc[i].x - final_mc[j].x) + (final_mc[i].y - final_mc[j].y)*(final_mc[i].y - final_mc[j].y));
                if(dist < pMax*8){
                    clusterFound = true;
                    cluster[i] = cluster[j];
                }
            }
        }
        if(!clusterFound){
            counter++;
            cluster[i] = counter;
        }
    }

    std::vector<std::vector< int > > markers(counter + 1);
    for(int i = 0; i < FinalContourNumber; ++i){
        markers[cluster[i]].push_back(i); //each item contains vector with indices of dots in cluster
    }

    std::vector<cv::Point2f> markerCentre (counter + 1);
    cv::Point2f currentCentre, currentDev; //contains centres of each cluster.
    int  markersSize;
    std::vector<DotCluster> markerIDs;
    DotCluster dotCluster;
    for(int j = 0; j < numberOfClusters; ++j){
        (dotCluster.votes).push_back(0);
    }
    for(int i = 0; i < counter + 1; ++i){
        currentCentre = cv::Point2f(0.0f, 0.0f);
        currentDev = cv::Point2f(0.0f, 0.0f);
        markersSize = markers[i].size();
        for(int j = 0; j < markersSize; ++j){
            currentCentre += final_mc[(markers[i])[j]];
            currentDev += geometricDeviation[(markers[i])[j]];
        }
        currentCentre.x /= markersSize;
        currentCentre.y /= markersSize;
        markerCentre[i] = currentCentre;

        currentDev.x /= markersSize;
        currentDev.y /= markersSize;

        dotCluster.position = markerCentre[i];
        dotCluster.dotNumber = markersSize;
        dotCluster.dev = currentDev;
        markerIDs.push_back(dotCluster);
    }


  //  Delauny triangulation
    cv::Rect rect(0,0, imgInput.cols, imgInput.rows);
    cv::Subdiv2D subdiv;
    subdiv.initDelaunay(rect);
    subdiv.insert(markerCentre);
    std::vector<cv::Vec6f>  T;

    draw_delaunay(drawing, subdiv, cv::Scalar(0,0,0), &T);

    cv::Point2f indices[3], indices2[3];

    std::vector <std::vector <int> > adjT (T.size());
    std::vector <std::vector <int> > triangleIndices (T.size());

    for(int i = 0; i< T.size(); ++i){

        cv::Vec6f t = T[i];
        indices[0] = cv::Point2f(t[0], t[1]);
        indices[1] = cv::Point2f(t[2], t[3]);
        indices[2] = cv::Point2f(t[4], t[5]);

        for (int ii = 0; ii < 3; ++ii){
            (triangleIndices[i]).push_back((findIndex(markerCentre, indices[ii]))); //for triangle i, vertices index in markerCentre can be found
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
    std::vector <std::vector<int> > indicesT, dotsT;
    std::vector<int> tempVector, tempVector2, tempVector3;
    int adjNo = 0;
    int triInd [3];
    for(int i = 0; i< T.size(); ++i){
        if(adjT[i].size() == 3){
            tempVector.clear();
            tempVector2.clear();
            tempVector3.clear();
            for(int j = 0 ; j < 3; ++j){
                tempVector.push_back((triangleIndices[i])[j]); //first 3 terms are middle of edge vertices
                triInd[j] = adjT[i][j];
            }
            for(int k = 0; k < 3; ++k){
                for(int j = 0 ; j < 3; ++j){
                    int val = (triangleIndices[(adjT[i])[k]])[j];
                    if((val != tempVector[0]) && (val != tempVector[1]) && (val!= tempVector[2])){
                        tempVector.push_back((triangleIndices[(adjT[i])[k]])[j]);
                    }
                }
            }
            tempVector2 = getTriangleOrder(tempVector, triInd, triangleIndices);

            for(int j = 0; j < tempVector2.size(); ++j){
                tempVector3.push_back(markers[tempVector2[j]].size());
            }
            indicesT.push_back(tempVector2);
            dotsT.push_back(tempVector3);
        }
    }


    for(int i = 0 ; i < indicesT.size(); ++i){
        outputTest = hashTable[calculateKey(dotsT[i])];
        for(int j = 0; j<indicesT[i].size(); ++j){
            for (int k = 0; k < outputTest.size(); ++k){
                ++((markerIDs[indicesT[i][j]]).votes)[outputTest[k] - 1];
            }
        }
    }

    int SIZE = markerIDs.size();
    int numberOfVotes;
    for(int i = 0; i<SIZE; ++i){
        markerIDs[i].id = myMode(markerIDs[i].votes, &numberOfVotes); //uses the mode to vote
        markerIDs[i].modeCount = numberOfVotes;
    }
    removeMultipleVotes(&markerIDs);


    int numberOfDetectedMarkers = 0;
    SIZE = markerIDs.size();
    for(int i = 0; i < SIZE; ++i ){
        if(markerIDs[i].id != -1){
            ++numberOfDetectedMarkers;
            if(clusterData[ markerIDs[i].id -1].status == "lost"){
                clusterData[ markerIDs[i].id-1].status = "detected";
                clusterData[ markerIDs[i].id-1].velocity = cv::Point2f(0.0f, 0.0f);
                clusterData[ markerIDs[i].id-1].deviation = markerIDs[i].dev;
            }
        }
        clusterData[ markerIDs[i].id-1].position = markerIDs[i].position;
        clusterData[ markerIDs[i].id-1].deviation = markerIDs[i].dev;
    }

    // for(int i = 0; i < clusterData.size(); ++i ){
    //     std::cout<<"status "<<clusterData[i].deviation.x<<"\n";
    // }

    cv::Scalar colour  = cv::Scalar(0,0,255);
    for( int i = 0; i< finalContours.size(); i++ ){
        drawContours( drawing, finalContours, i, colour, 2, 8, hierarchy, 0, cv::Point());
    }

    for(int i = 0; i <SIZE; ++i){
        circle(drawing, markerIDs[i].position, 30, cv::Scalar(0,0,0), 1, 8, 0 );
    }

    for(int i = 0; i < clusterData.size(); ++i){
        if(clusterData[i].status == "detected"){
            std::string str = std::to_string(clusterData[i].id);
            putText(drawing, str, clusterData[i].position, cv::FONT_HERSHEY_SIMPLEX,1,cv::Scalar(0,0,0),2, cv::LINE_AA);
        }
    }


    namedWindow("myWindow", cv::WINDOW_AUTOSIZE);
    char exit_key_press = 0;
    do {
        cv::imshow("myWindow", drawing);
        exit_key_press = cvWaitKey(1);
    }while (exit_key_press != '\x1b');


    return 0;
}