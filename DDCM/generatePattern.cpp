#include <iostream>
#include <stdlib.h>
#include <vector>
#include <algorithm>

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

int main(){

    int gridWidth = 5;
    int gridHeight = 5;
    int maxNumberDots = 7;

    //Find initial configuration of dots where no adjacent vertices are the same -----

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
    //----------------------------------------
 
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
    return 0;
}