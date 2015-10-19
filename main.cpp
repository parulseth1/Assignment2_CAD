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
    
    if (Nets){
        delete[] Nets;
    }
    
    cout<<"Done"<<endl;
    
    
    
    
    
    ///Parul's work starts here
    
    vector<vector<int> > AllWeights; /// gave some error here when i had not put space between the
                                      // >> therefore had to put space. please check
    
    
    // setting total weights for all the blocks.// this is an initial step.// after this
    // we would just add the weights of the dummy nets we create in order to spread.
    for(int a = 0; a< Blocks.size(); a++){   /// check if dot or arrow
        setTotalWeight(Blocks[a],&Nets); 
    }
    cout<<"weights set"<<endl;
    // now to to make the matrix.
    int numOfBlocks = Blocks.size(); // not getting set..
    for (int a =0; a<numOfBlocks;a++){
        vector<int> Weights = getCorrespondingWeights(Blocks[a], &Nets, a, numOfBlocks);
        AllWeights.push_back(Weights);
    }
    vector<vector<int> > LeftMatrix;
    vector<int> RightMatrix_X;
    vector<int> RightMatrix_Y;
    GetLeftMatrix(AllWeights, Blocks, &LeftMatrix, numOfBlocks);
    ForXGetRightMatrix(AllWeights, Blocks, &RightMatrix_X, numOfBlocks);
    ForYGetRightMatrix(AllWeights, Blocks, &RightMatrix_Y, numOfBlocks);
    
    /// parul's work ends here
    
    
    // put this after the umfpack stuff
    // from here.
     // get this from parser because we wont need any other value for this
                        // jus the initial value.
    int HPWL = CalculateHPWL(&Nets, Blocks, numNets);
    
    //to here
    
    
    
    //UMFPACK STUFF
    int* x = NULL;
    int dim = 5;
    int b[] = {0, 0, 0, 0, 0};
    int** A = NULL;
    A = new int* [5];
    for (int i =0; i < 5; i++){
        A[i] = new int[5];
    }
    
    for (int i = 0; i < dim; i++){
        for (int j = 0; j < dim; j++){
            A[i][j] = 0;
        }
    }
    
    A[0][0] = 2;
    A[0][1] = 3;
    A[1][0] = 3;
    A[1][2] = 4;
    A[1][4] = 6;
    A[2][1] = -1;
    A[2][2] = -3;
    A[2][3] = 2;
    A[3][2] = 1;
    A[4][1] = 4;
    A[4][2] = 2;
    A[4][4] = 1;
    
    
    doSolve(A, dim, &x, b);
    return 0;
}