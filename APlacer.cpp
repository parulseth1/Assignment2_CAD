//
//  APlacer.cpp
//  APlacer
// Parul Seth
//
#include "APlacer.h"
#include <stdio.h>
#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include "Placer.h"

// have to get the weights put in for each block using the nets it is connected to.
void setTotalWeight(block Block1, Net** nets){
    vector<int>* net = Block1.getNetNum();
    int weight = 0;
    for(int a =0; a< net->size(); a++){  // dont know if we have to use dot or arrow.
        int numpins = (*nets)[((*net)[a])].getNumPins();
        int Netweight = (*nets)[((*net)[a])].getPinWeight() * (numpins-1);
        int weight = weight + Netweight;  
    }
   Block1.AddTotalWeight(weight); 
    
}

vector<int> getCorrespondingWeights(block Block1, Net** nets, int blocknumber, int TotalNumOfPin ){
    vector<int>* net = Block1.getNetNum();
    vector<int> weights;
    //vector<WeightandPin> w1;
    vector<WeightandPin> w1;
    WeightandPin w;
    for(int a = 0; a< net->size(); a++){  // arrow or dot
        vector<int>* blocknums = (*nets)[((*net)[a])].getBlockNums();
        for(int b = 0; b<blocknums->size(); b++){ //arrow or dot
            w.blocknum = (*blocknums)[b];
            w.weight = (*nets)[((*net)[a])].getPinWeight();
            int count =0;
            for(int c = 0; c< w1.size(); c++){
                if(w.blocknum == w1[c].blocknum){
                   w1[c].weight = w1[c].weight + w.weight;
                   count = count+1;
                }
                    
            }
            if(count ==0){
                w1.push_back(w); // dot or arrow
            }
        } 
        
    }
    for(int e =0; e< TotalNumOfPin; e++){
        int counter =0;
        for(int f = 0; f< w1.size(); f++){
            if(e == f){
                weights[w1[f].blocknum] = Block1.getTotalWeight();
            }
            if(e == w1[f].blocknum){
                counter = counter +1;
                weights[w1[f].blocknum] = w1[f].weight;
            }
       
        }
        if (counter == 0){
            weights[e] = 0;
        }
    }
    return weights;
}


void GetLeftMatrix(vector<vector<int> > weights, vector<block> Blocks, vector<vector<int> >* LeftMatrix, int numOfBlocks){
    vector<int> weight;
    vector<int> row;
    for(int a =0; a< numOfBlocks; a++){
        int counter = 0;
        if(Blocks[a].getFixed() != true){
            weight = weights[a];
            for(int b=0; b< numOfBlocks; b++){
                if(Blocks[b].getFixed() != true){
                    row[counter] = weight[b];
                    counter++;
                }
            }
        }
        LeftMatrix->push_back(row);
        row.clear();
    }
    
}

void ForXGetRightMatrix(vector<vector<int> > weights, vector<block> Blocks, vector<int>* RightMatrix, int numOfBlocks){
   vector<int> weight;
    int row =0;
    for(int a =0; a< numOfBlocks; a++){
        if(Blocks[a].getFixed() != true){
            weight = weights[a];
            for(int b=0; b< numOfBlocks; b++){
                if(Blocks[b].getFixed() == true){
                    int x = Blocks[b].getx();
                    row = row + (weight[b])*x;
                }
            }
        }
        RightMatrix->push_back(row);
    }
     
}


void ForYGetRightMatrix(vector<vector<int> > weights, vector<block> Blocks, vector<int>* RightMatrix, int numOfBlocks){
   vector<int> weight;
    int row =0;
    for(int a =0; a< numOfBlocks; a++){
        if(Blocks[a].getFixed() != true){
            weight = weights[a];
            for(int b=0; b< numOfBlocks; b++){
                if(Blocks[b].getFixed() == true){
                    int y = Blocks[b].gety();
                    row = row + (weight[b])*y;
                }
            }
        }
        RightMatrix->push_back(row);
    }
     
}




// then use these to find the first locations.
// then, how to calculate Half Perimeter wire length?? finally find the total wire used..
// next would be divide into 4 parts, and get dummy points
// then the dummy points and other not fixed points as 2 pin nets,
// now, add this weight to existing weights and find the new solution.
// now get new HPWL
// thats how we spread
// only for the cct2 do this spread thing until a condition is met...


