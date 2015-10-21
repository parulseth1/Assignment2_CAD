//
//  main.cpp
//  Parser
//
//  Created by AliakbarEski on 2015-10-13.
//  Copyright (c) 2015 AliakbarEski. All rights reserved.
//

#include <iostream>
#include <vector>
#include <sstream>
#include "Parser.h"
#include "UMFWrapper.h"
#include "Placer.h"
#include "APlacer.h"
#include "graphics.h"
#include "Drawing.h"
#define FILENAME "/home/parul/NetBeansProjects/AnalyticalPlacer/cct1.txt"
using namespace::std;

int main(int argc, const char * argv[]) {
    vector<block> Blocks;
    Net* Nets = NULL;
    int numNets = 0;
    doParse(FILENAME, &Blocks, &Nets, &numNets);
    
    cout<<"Parsing Complete"<<endl;
    cout<<"number of nets: "<<numNets<<endl;

    for (int i = 0; i < 17; i++){
        cout<<Nets[i].getNumPins()<<"::"<<Nets[i].getPinWeight()<<endl;
    }
    
    
    
    
    
    
    
    ///Parul's work starts here
    
    vector<vector<int> > AllWeights; /// gave some error here when i had not put space between the
                                      // >> therefore had to put space. please check
    
    
    // setting total weights for all the blocks.// this is an initial step.// after this
    // we would just add the weights of the dummy nets we create in order to spread.
    for(int a = 0; a< Blocks.size(); a++){   /// check if dot or arrow
        int tot_weight = setTotalWeight(Blocks[a],&Nets);
        Blocks[a].AddTotalWeight(tot_weight);
    }
    cout<<"weights set"<<endl;
    // now to to make the matrix.
    int numOfBlocks = Blocks.size(); // not getting set..
    for (int a =0; a<numOfBlocks;a++){
        cout<<a<<endl;
        vector<int> Weights = getCorrespondingWeights(Blocks[a], &Nets, a+1, numOfBlocks);
        AllWeights.push_back(Weights);
        for(int b =0; b<numOfBlocks; b++){
            cout<<Weights[b]<<endl;
        }
        cout<<endl;
    }
    vector<vector<int>> LeftMatrix;
    vector<int> RightMatrix_X;
    vector<int> RightMatrix_Y;
    GetLeftMatrix(AllWeights, Blocks, &LeftMatrix, numOfBlocks);
    ForXGetRightMatrix(AllWeights, Blocks, &RightMatrix_X, numOfBlocks);
    ForYGetRightMatrix(AllWeights, Blocks, &RightMatrix_Y, numOfBlocks);
    cout<<"Left Matrix"<<endl;
    for(int a1 = 0; a1 < LeftMatrix.size(); a1++){
        for (int a2 = 0; a2 < LeftMatrix[a1].size(); a2++) {
            cout << LeftMatrix[a1][a2]<<"::";

        }
        cout<<endl;
    }
    cout<<endl<<"for x"<<endl;
    for(int a1 = 0; a1 < RightMatrix_X.size(); a1++){
            cout << RightMatrix_X[a1]<<"::";
    
    }
    cout<<endl<<"for Y"<<endl;
      for(int a1 = 0; a1 < RightMatrix_Y.size(); a1++){
            cout << RightMatrix_Y[a1]<<"::";

    }
    cout<<endl;
    
    
    
    
    
    /// parul's work ends here
    
    
    // put this after the umfpack stuff
    // from here.
     // get this from parser because we wont need any other value for this
                        // jus the initial value.
    
    
    //to here
    
    
    
    //UMFPACK STUFF
    double* x = NULL;
    int dim = 6;
    double bx[] = {0, 0, 0, 0, 0,0};
    double by[] = {0, 0, 0, 0, 0,0};
    int** A = NULL;
    A = new int* [dim];
    for (int i =0; i < dim; i++){
        A[i] = new int[dim];
    }
    
    for (int i = 0; i < dim; i++){
        for (int j = 0; j < dim; j++){
            A[i][j] = LeftMatrix[i][j];
        }
    }
    for(int i= 0; i<dim; i++){
        bx[i] = RightMatrix_X[i];
        by[i] = RightMatrix_Y[i];
    }
    
    
//    A[0][0] = 2;
//    A[0][1] = 3;
//    A[1][0] = 3;
//    A[1][2] = 4;
//    A[1][4] = 6;
//    A[2][1] = -1;
//    A[2][2] = -3;
//    A[2][3] = 2;
//    A[3][2] = 1;
//    A[4][1] = 4;
//    A[4][2] = 2;
//    A[4][4] = 1;
    
    double* y = NULL;
    doSolve(A, dim, &x, bx);
    doSolve(A, dim, &y, by);
    
    for(int i =0; i<dim; i++){
        cout<<"x"<<i<<":"<<x[i]<<endl;
    }
    
    int HPWL = CalculateHPWL(&Nets, Blocks, numNets);

    int weight_quad;
    int j =0;
    for(int h = 0; h< Blocks.size(); h++){
        if(Blocks[h].getFixed()!= true){
            Blocks[h].setx(x[j]);
            Blocks[h].sety(y[j]);
            j++;
        }
    }
    
    point centroid = getCentroid(Blocks, numOfBlocks);
   quadrant* quad = spreading(Blocks, centroid, 100);
    weight_quad =(1+.1) * 1;
    //Blocks.AddTotalWeight(weight_quad);
    
    j = 0;
    
    for (int a =0; a<numOfBlocks;a++){
        if(Blocks[a].getFixed()!= true){
            Blocks[a].AddTotalWeight(weight_quad);
            for(int k =0; k<quad[0].blocknums.size();k++){
                if(Blocks[a].getBlockNum()== quad[0].blocknums[k]){
                RightMatrix_X[j] = weight_quad * quad[0].dummy.x;
                RightMatrix_Y[j] = weight_quad * quad[0].dummy.y;
                }
            } 
            for(int k =0; k<quad[1].blocknums.size();k++){
                if(Blocks[a].getBlockNum()== quad[1].blocknums[k]){
                RightMatrix_X[j] = weight_quad * quad[1].dummy.x;
                RightMatrix_Y[j] = weight_quad * quad[1].dummy.y;
                }
            }   
            for(int k =0; k<quad[2].blocknums.size();k++){
                if(Blocks[a].getBlockNum()== quad[2].blocknums[k]){
                RightMatrix_X[j] = weight_quad * quad[2].dummy.x;
                RightMatrix_Y[j] = weight_quad * quad[2].dummy.y;
                }
            }   
            for(int k =0; k<quad[3].blocknums.size();k++){
                if(Blocks[a].getBlockNum()==quad[3].blocknums[k]){
                RightMatrix_X[j] = weight_quad * quad[3].dummy.x;
                RightMatrix_Y[j] = weight_quad * quad[3].dummy.y;
                }
            }   
        }
    }
    for (int a =0; a<numOfBlocks;a++){
        vector<int> Weights = getCorrespondingWeights(Blocks[a], &Nets, a, numOfBlocks);
        AllWeights.push_back(Weights);
    }
    //vector<vector<int> > LeftMatrix;
    GetLeftMatrix(AllWeights, Blocks, &LeftMatrix, numOfBlocks);
    
    DrawOnScreen();
    
    if (Nets){
        delete[] Nets;
    }
    
    cout<<"Done"<<endl;
    
    return 0;
}