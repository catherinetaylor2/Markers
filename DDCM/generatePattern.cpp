#include <iostream>
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <fstream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

int randomNum(int max){
    return std::rand()%max+1;
}
int getDifferentNumber(std::vector<int> currentValues, int max){
    bool unique = false;
    int N;
    while (!unique){
        N = randomNum(max);
        if(std::find(currentValues.begin(), currentValues.end(), N) != currentValues.end()) {
            unique = false;
        }
        else{
            unique = true;
        }
    }
    return N;
}

bool testUnique(std::vector<std::vector<int> > previous, std::vector<int> current){


    for(int i = 0; i < previous.size(); ++i){
        if(current == previous[i]){
            return false;
        }
    }

    return true;
}
void writeToFile(const char * filename, std::vector<std::vector<int> > data){
    std::ofstream myfile;
    myfile.open(filename);
    for(int i = 0; i < data.size(); ++i){
        for(int j = 0; j<data[i].size(); ++j){
           myfile<<data[i][j]<<" ";
        }
        myfile<<"\n";
    }
   myfile.close();
}

void writePattern(const char * filename, int * pattern, int wdth, int hght){
    std::ofstream myfile;
    myfile.open(filename);
    myfile<<"W "<<wdth<<" H "<<hght<<"\n";
    for(int j = 0; j < hght; ++j){
        for(int i = 0; i < wdth; ++i){
            if(j%2 == 0){
                myfile<<pattern[i + wdth*j] <<" ";
            }
            else{
                myfile<<pattern[i + wdth*j] <<" ";
            }
        }
        myfile<<"\n";
    }
    myfile.close();
}
void drawMarker(cv::Mat img, int dotNumber, cv::Point centre, float radius){
    if(dotNumber == 1){
        cv::circle(img, centre, 2*radius, cv::Scalar(0,0,0), -1, 8, 0);
    }
    else if(dotNumber == 2){
        cv::circle(img, cv::Point(centre.x - 3*radius, centre.y), 2*radius, cv::Scalar(0,0,0), -1, CV_AA, 0);
        cv::circle(img, cv::Point(centre.x + 3*radius, centre.y), 2*radius, cv::Scalar(0,0,0), -1, CV_AA, 0);
    }
    else if(dotNumber == 3){
        cv::circle(img, cv::Point(centre.x - 6*radius, centre.y), 2*radius, cv::Scalar(0,0,0), -1, CV_AA, 0);
        cv::circle(img, centre, 2*radius, cv::Scalar(0,0,0), -1, CV_AA, 0);
        cv::circle(img, cv::Point(centre.x + 6*radius, centre.y), 2*radius, cv::Scalar(0,0,0), -1, CV_AA, 0);
    }
    else if(dotNumber == 4){
        cv::circle(img, centre, 2*radius, cv::Scalar(0,0,0), -1, CV_AA, 0);
        cv::circle(img, cv::Point(centre.x - 2*radius, centre.y - 6*radius), 2*radius, cv::Scalar(0,0,0), -1, CV_AA, 0);
        cv::circle(img, cv::Point(centre.x + 6*radius, centre.y), 2*radius, cv::Scalar(0,0,0), -1, CV_AA, 0);
        cv::circle(img, cv::Point(centre.x - 2*radius, centre.y + 6*radius), 2*radius, cv::Scalar(0,0,0), -1, CV_AA, 0);
    }
    else if(dotNumber == 5){
        cv::circle(img, cv::Point(centre.x - 6*radius, centre.y), 2*radius, cv::Scalar(0,0,0), -1, CV_AA, 0);
        cv::circle(img, centre, 2*radius, cv::Scalar(0,0,0), -1, CV_AA, 0);
        cv::circle(img, cv::Point(centre.x + 6*radius, centre.y), 2*radius, cv::Scalar(0,0,0), -1, CV_AA, 0);
        cv::circle(img, cv::Point(centre.x, centre.y + 6*radius), 2*radius, cv::Scalar(0,0,0), -1, CV_AA, 0);
        cv::circle(img, cv::Point(centre.x, centre.y - 6*radius), 2*radius, cv::Scalar(0,0,0), -1, CV_AA, 0);
    }
    else if(dotNumber == 6){
        cv::circle(img, centre, 2*radius, cv::Scalar(0,0,0), -1, CV_AA, 0);
        cv::circle(img, cv::Point(centre.x - 2*radius, centre.y - 6*radius), 2*radius, cv::Scalar(0,0,0), -1, CV_AA, 0);
        cv::circle(img, cv::Point(centre.x + 5*radius, centre.y - 5*radius), 2*radius, cv::Scalar(0,0,0), -1, CV_AA, 0);
        cv::circle(img, cv::Point(centre.x + 6*radius, centre.y + 2*radius ), 2*radius, cv::Scalar(0,0,0), -1, CV_AA, 0);
        cv::circle(img, cv::Point(centre.x - radius, centre.y + 6*radius), 2*radius, cv::Scalar(0,0,0), -1, CV_AA, 0);
         cv::circle(img, cv::Point(centre.x - 6*radius, centre.y), 2*radius, cv::Scalar(0,0,0), -1, CV_AA, 0);
    }
    else if(dotNumber == 7){
        cv::circle(img, cv::Point(centre.x - 6*radius, centre.y), 2*radius, cv::Scalar(0,0,0), -1, CV_AA, 0);
        cv::circle(img, centre, 2*radius, cv::Scalar(0,0,0), -1, CV_AA, 0);
        cv::circle(img, cv::Point(centre.x + 6*radius, centre.y), 2*radius, cv::Scalar(0,0,0), -1, CV_AA, 0);
        cv::circle(img, cv::Point(centre.x + 3*radius, centre.y + 6*radius), 2*radius, cv::Scalar(0,0,0), -1, CV_AA, 0);
        cv::circle(img, cv::Point(centre.x + 3*radius, centre.y - 6*radius), 2*radius, cv::Scalar(0,0,0), -1, CV_AA, 0);
        cv::circle(img, cv::Point(centre.x - 3*radius, centre.y + 6*radius), 2*radius, cv::Scalar(0,0,0), -1, CV_AA, 0);
        cv::circle(img, cv::Point(centre.x - 3*radius, centre.y - 6*radius), 2*radius, cv::Scalar(0,0,0), -1, CV_AA, 0);
    }
    else{
        std::cerr<<"Error: no such pattern. Choose input between 1 and 7 \n";
    }
}

int main(int argc, char* argv[] ){

    int gridWidth, gridHeight, maxNumberDots, index;
    bool pattern = false;
    std::vector<std::vector <int> > IDS, clusters, sortedMarkers;
    std::vector<int> tempCluster, tempID;

    if(argc>1){
		gridWidth = atoi(argv[1]);
		gridHeight = atoi(argv[2]);
        maxNumberDots = atoi(argv[3]);
	}
	else{
        gridWidth = 4; //note W, H >= 3
        gridHeight = 4;
        maxNumberDots = 7;
	}

    //Find initial configuration of dots where no adjacent vertices are the same -----

    while(!pattern){

        int * outputPattern = new int [gridWidth*gridHeight];
        std::vector<int> adjacentValues;
        for(int j = 0; j <  gridHeight; ++j){
            for(int i = 0; i < gridWidth; ++i){
                adjacentValues.clear();
                if(i + gridWidth*j == 0){
                    outputPattern[i + gridWidth*j] = getDifferentNumber(adjacentValues, maxNumberDots);
                }
                else if ((i != 0) &&(j ==0)){
                    adjacentValues.push_back(outputPattern[i + gridWidth*j -1]);
                    outputPattern[i + gridWidth*j] = getDifferentNumber(adjacentValues, maxNumberDots);
                }
                else if (i + gridWidth*j == gridWidth*gridHeight - 1){
                    adjacentValues.push_back(outputPattern[i + gridWidth*j -1]);
                    adjacentValues.push_back(outputPattern[i + gridWidth*(j-1)]);
                    if(j%2 == 1){
                        adjacentValues.push_back(outputPattern[i + gridWidth*(j-2)]);
                    }
                    outputPattern[i + gridWidth*j] = getDifferentNumber(adjacentValues, maxNumberDots);
                }
                else if (j%2 == 1) {
                    adjacentValues.push_back(outputPattern[i + gridWidth*j -1]);
                    adjacentValues.push_back(outputPattern[i + gridWidth*(j-1)]);
                    adjacentValues.push_back(outputPattern[i + gridWidth*(j-1) + 1]);
                    outputPattern[i + gridWidth*j] = getDifferentNumber(adjacentValues, maxNumberDots);
                }
                else{
                    adjacentValues.push_back(outputPattern[i + gridWidth*j -1]);
                    adjacentValues.push_back(outputPattern[i + gridWidth*(j-1)]);
                    adjacentValues.push_back(outputPattern[i + gridWidth*(j-1) - 1]);
                    if(i == 0){
                        adjacentValues.push_back(outputPattern[i + gridWidth*(j-2)]);
                    }
                    outputPattern[i + gridWidth*j] = getDifferentNumber(adjacentValues, maxNumberDots);
                }
                
            }
        }

        //NOW TEST FOR UNIQUE TRIANGLES  ----------------------------------------

        IDS.clear();
        clusters.clear();
        sortedMarkers.clear();
        bool unique = true;

        for(int j = 0; j < gridHeight - 2; ++j){
            for(int i = 0; i < gridWidth - 2; ++i){
                tempID.clear();
                tempCluster.clear();
                for(int k = 0; k < 3; ++k){
                    index = i + k +j*gridWidth;
                    tempID.push_back(index + 1);
                    tempCluster.push_back(outputPattern[index]);
                }
                for(int k = 0; k < 2; ++k){
                    index = i + k + gridWidth +j*gridWidth + (j%2);
                    tempID.push_back(index + 1);
                    tempCluster.push_back(outputPattern[index]);
                }
                index = i + 1 + 2*gridWidth +j*gridWidth;
                tempID.push_back(index +1);
                tempCluster.push_back(outputPattern[index]);

                IDS.push_back(tempID);
                clusters.push_back(tempCluster);
                std::sort(tempCluster.begin(), tempCluster.end());
                unique = testUnique(sortedMarkers, tempCluster);
                sortedMarkers.push_back(tempCluster);
            }
        }

        for(int j = gridHeight - 1; j > 1; --j){
            for(int i = 0; i < gridWidth - 2; ++i){
                tempID.clear();
                tempCluster.clear();
                for(int k = 0; k < 3; ++k){
                    index = i + k +j*gridWidth;
                    tempID.push_back(index + 1);
                    tempCluster.push_back(outputPattern[index]);
                }
                for(int k = 0; k < 2; ++k){
                    index = i + k - gridWidth + j*gridWidth + (j%2);
                    tempID.push_back(index + 1);
                    tempCluster.push_back(outputPattern[index]);
                }
                index = i + 1 - 2*gridWidth +j*gridWidth;
                tempID.push_back(index +1);
                tempCluster.push_back(outputPattern[index]);

                IDS.push_back(tempID);
                clusters.push_back(tempCluster);
                std::sort(tempCluster.begin(), tempCluster.end());
                unique = testUnique(sortedMarkers, tempCluster);
                sortedMarkers.push_back(tempCluster);
            }
        }

        if(unique){
            pattern = true;
            writePattern("pattern.text", outputPattern, gridWidth, gridHeight);
            int patWidth = 1000, patHeight = 1000;
            cv::Mat Pattern = cv::Mat(patWidth, patHeight, CV_8UC3 );
            Pattern.setTo(cv::Scalar(255,255,255));
            int wdthDivide, hghtDivide;
            if(gridWidth%2 == 1){
                wdthDivide = patWidth/((float)gridWidth + 1.0f);
            }
            else{
                wdthDivide = patWidth/((float)gridWidth + 1.5f );
            }
            if(gridHeight%2 == 1){
                hghtDivide = patHeight/((float) gridHeight + 1.0f);
            }
            else{
            hghtDivide = patHeight/((float) gridHeight + 1.5f); 
            }
            cv::Point centre;
            int indexI, indexJ;
            for(int i = wdthDivide; i < patWidth - wdthDivide; i+= wdthDivide){
                indexI = (i-1)/wdthDivide;
                for(int j = hghtDivide; j< patHeight - hghtDivide; j+= hghtDivide){
                    indexJ = (j-1)/hghtDivide;
                    if(indexJ%2 == 1){
                            centre =  cv::Point(i - hghtDivide/4.0f, j);
                            drawMarker(Pattern, outputPattern[indexI + gridWidth*indexJ], centre, 2);
                    }
                    else{
                            centre = cv::Point(i+hghtDivide/4.0f, j);
                            drawMarker(Pattern, outputPattern[indexI + gridWidth*indexJ], centre, 2);
                    }
                }
            }
            cv::imwrite("pattern.jpg", Pattern);
        }
        delete [] outputPattern;
    }

    writeToFile("markerIDs.txt", IDS);
    writeToFile("markerClusters.txt", clusters);

    



    return 0;
}