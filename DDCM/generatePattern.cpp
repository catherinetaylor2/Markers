#include <iostream>
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <fstream>

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

int main(int argc, char* argv[] ){

    int gridWidth, gridHeight, maxNumberDots;
    bool pattern = false;
    std::vector<std::vector <int> > IDS;
    std::vector<std::vector <int> > clusters;

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
        std::vector<int> tempID;
        clusters.clear();
        std::vector<int> tempCluster;
        std::vector<std::vector <int> > sortedMarkers;
        bool unique = true;

        int index;
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
        }
    




    //  std::cout<<"sorted ";
    //             for(int k = 0 ; k< tempID.size(); ++k){
    //                 std::cout<<tempID[k]<<" ";
    //             }
    //             std::cout<<"\n";

        std::cout<<unique<<" pattern ";
        for(int j = 0; j < gridHeight; ++j){
            for(int i = 0; i < gridWidth; ++i){
                if(j%2 == 0){
                    std::cout<<outputPattern[i + gridWidth*j] <<" ";
                }
                else{
                    std::cout<<" "<<outputPattern[i + gridWidth*j] ;
                }
            }
            std::cout<<"\n";
        }

        

        delete [] outputPattern;
    }
    writeToFile("markerIDs.txt", IDS);
    // std::ofstream IDs;
    // IDs.open("IDs.txt");
    // for(int i = 0; i < IDS.size(); ++i){
    //     for(int j = 0; j<IDS[i].size(); ++j){
    //        IDs<<IDS[i][j]<<" ";
    //     }
    //     IDs<<"\n";
    // }
    // IDs.close();



    return 0;
}