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
#define FILENAME "/home/eski/Parul/Assignment2/cct2.txt"

using namespace::std;

int main(int argc, const char * argv[]) {
    
    
     if (argc < 2) {
        cout << "Usage : ./AnalyticalPlacer <File name>" << endl;
        return 1;
    }
    int caseDirection;
    if (strcmp(argv[1], "-u") == 0) {
        //unidirectional case
        caseDirection = 1;
    } else if (strcmp(argv[1], "-b") == 0) {
        //bidirectional case
        caseDirection = 2;
    } else {
        cout << "wrong argument" << endl;
        return 1;
    }
    
    
    vector<block> Blocks;
    vector<block> Blocks1;
    Net* Nets = NULL;
    int numNets = 0;
    doParse(FILENAME, &Blocks, &Nets, &numNets);
    
    cout<<"Parsing Complete"<<endl;
    cout<<"number of nets: "<<numNets<<endl;

    for (int i = 0; i < numNets; i++){

        cout<<i<<"::"<<Nets[i].getNumPins()<<"::"<<Nets[i].getPinWeight()<<endl;
    }

    vector<vector<double>> AllWeights; /// gave some error here when i had not put space between the
    for(int a = 0; a< Blocks.size(); a++){   /// check if dot or arrow
        double tot_weight = setTotalWeight(Blocks[a],&Nets);
        Blocks[a].AddTotalWeight(tot_weight);
    }
    cout<<"weights set"<<endl;
    // now to to make the matrix.
    int numOfFixed =0;
    int numOfBlocks = Blocks.size(); // not getting set..
    for(int h = 0; h< Blocks.size(); h++){
        if(Blocks[h].getFixed()!= true){
            numOfFixed++;
        }
    }
    for (int a =0; a<numOfBlocks;a++){
        cout<<a<<endl;
        vector<double> Weights = getCorrespondingWeights(Blocks[a], &Nets, a+1, numOfBlocks);
        AllWeights.push_back(Weights);
        cout<<"w:"<<Weights[0]<<"::"<<Weights[1]<<endl;
        

    }
    vector<vector<double>> LeftMatrix;
    vector<double> RightMatrix_X;
    vector<double> RightMatrix_Y;
    GetLeftMatrix(AllWeights, Blocks, &LeftMatrix, numOfBlocks);
    ForXGetRightMatrix(AllWeights, Blocks, &RightMatrix_X, numOfBlocks);
    ForYGetRightMatrix(AllWeights, Blocks, &RightMatrix_Y, numOfBlocks);

    //UMFPACK STUFF
    double* x = NULL;
    int dim = numOfFixed;
    double* bx = new double[dim];
    double* by = new double[dim];
    double** A = NULL;
    A = new double* [dim];
    for (int i =0; i < dim; i++){
        A[i] = new double[dim];
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
    
    
    double* y = NULL;
    doSolve(A, dim, &x, bx);
    doSolve(A, dim, &y, by);
    
    
    for(int i =0; i<dim; i++){
        cout<<"x"<<i<<":"<<x[i]<<":y: "<<y[i]<<endl;
    }
    

   

    int weight_quad;
    int j =0;
    for(int h = 0; h< Blocks.size(); h++){
        if(Blocks[h].getFixed()!= true){
            Blocks[h].setx(x[j]);
            Blocks[h].sety(y[j]);
            j++;
        }
    }
    double HPWL = CalculateHPWL(&Nets, Blocks, numNets);
    j =0;
    double* X = new double[numOfBlocks];
    double* Y = new double[numOfBlocks];
    for(int h = 0; h< Blocks.size(); h++){
        X[h]=Blocks[h].getx();
        Y[h]=Blocks[h].gety();
    }
    
    point centroid = getCentroid(Blocks, numOfBlocks);
    cout<<"centroid:"<<centroid.x<<"::"<<centroid.y<<endl;
    quadrant* quad = new quadrant[4];
    MakeQuads(Blocks, 100,1.5, &quad);
    cout<<"got the quadrants"<<endl;
    for(int i =0; i<numOfBlocks; i++){
        if(Blocks[i].getFixed()!= true){
            cout<<i<<endl;
            int quad_num = PutBoxInQuads(Blocks[i], centroid);
            Blocks[i].AddTotalWeight(quad[quad_num].weight);
            RightMatrix_X[j] = RightMatrix_X[j]+(quad[quad_num].weight * quad[quad_num].dummy.x);
            RightMatrix_Y[j] = RightMatrix_Y[j]+(quad[quad_num].weight * quad[quad_num].dummy.y);
            cout<<"rightmatrix steps"<<RightMatrix_X[j]<<"::"<<RightMatrix_Y[j]<<"::"<<quad_num<<endl;
            j++;
            quad[quad_num].blocknums.push_back(i+1); /// here we get the seg fault
            } 
    }


    AllWeights.clear();
    for (int a =0; a<numOfBlocks;a++){
        vector<double> Weights = getCorrespondingWeights(Blocks[a], &Nets, a+1, numOfBlocks);
        AllWeights.push_back(Weights);        
    }
    // umfpack stuff
    double* x_after = NULL;
    double* y_after = NULL;
    int dim_after = numOfFixed;
    double* bx_after = new double[dim_after];
    double* by_after = new double[dim_after];
    double** A_after = NULL;
    A_after = new double* [dim_after];
    for (int i =0; i < dim_after; i++){
        A_after[i] = new double[dim_after];
    }
    
    for (int i = 0; i < dim_after; i++){
        for (int j = 0; j < dim_after; j++){
            A_after[i][j] = (LeftMatrix[i][j]);
        }
    }
    for(int i= 0; i<dim_after; i++){
        bx_after[i] = (RightMatrix_X[i]);
        by_after[i] = (RightMatrix_Y[i]);
    }
    j =0;
    doSolve(A_after, dim_after, &x_after, bx_after);
    doSolve(A_after, dim_after, &y_after, by_after);
    
    
    Blocks1=Blocks;
    for(int h = 0; h< Blocks1.size(); h++){
        if(Blocks1[h].getFixed()!= true){
            Blocks1[h].setx(x[j]);
            Blocks1[h].sety(y[j]);
            j++;
        }
    }
    
    double* X_after = new double[numOfBlocks];
    double* Y_after = new double[numOfBlocks];
    for(int h = 0; h< Blocks1.size(); h++){
        X_after[h]=Blocks1[h].getx();
        Y_after[h]=Blocks1[h].gety();
    }
    
    
    double HPWL_after = CalculateHPWL(&Nets, Blocks1, numNets);
    
    cout<<"hpwl before: "<<HPWL<<" hpwl after:"<<HPWL_after<<endl;
    
    LoadBlocks(X,Y,numOfBlocks,10);
    LoadRatsNest(Nets, numNets);
    DrawOnScreen();
    
    
    if (Nets){
        delete[] Nets;
    }
    
    cout<<"Done"<<endl;
    
    return 0;
}