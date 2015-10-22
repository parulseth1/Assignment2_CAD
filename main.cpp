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
#define FILENAME "/home/parul/NetBeansProjects/AnalyticalPlacer/cct2.txt"

using namespace::std;

int main(int argc, const char * argv[]) {
    vector<block> Blocks;
    Net* Nets = NULL;
    int numNets = 0;
    doParse(FILENAME, &Blocks, &Nets, &numNets);
    
    cout<<"Parsing Complete"<<endl;
    cout<<"number of nets: "<<numNets<<endl;

    for (int i = 0; i < numNets; i++){

        cout<<i<<"::"<<Nets[i].getNumPins()<<"::"<<Nets[i].getPinWeight()<<endl;
    }
    
    
    
    
    
    
    
    ///Parul's work starts here
    
    vector<vector<double>> AllWeights; /// gave some error here when i had not put space between the
                                      // >> therefore had to put space. please check
    
    
    // setting total weights for all the blocks.// this is an initial step.// after this
    // we would just add the weights of the dummy nets we create in order to spread.
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
    cout<<"Left Matrix"<<endl;
    for(int a1 = 0; a1 < 10; a1++){
        for (int a2 = 0; a2 < 10; a2++) {
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
        cout<<"x"<<i<<":"<<x[i]<<":y: "<<y[i]<<endl;
    }
    
    double HPWL = CalculateHPWL(&Nets, Blocks, numNets);

    int weight_quad;
    int j =0;
    for(int h = 0; h< Blocks.size(); h++){
        if(Blocks[h].getFixed()!= true){
            Blocks[h].setx(x[j]);
            Blocks[h].sety(y[j]);
            j++;
        }
    }
    j =0;
    point centroid = getCentroid(Blocks, numOfBlocks);
    cout<<"centroid:"<<centroid.x<<"::"<<centroid.y<<endl;
    quadrant* quad = new quadrant[4];
    MakeQuads(Blocks, 100, .08, &quad);
    cout<<"got the quadrants"<<endl;
    for(int i =0; i<numOfBlocks; i++){
        if(Blocks[i].getFixed()!= true){
            cout<<i<<endl;
            int quad_num = PutBoxInQuads(Blocks[i], centroid);
            Blocks[i].AddTotalWeight(quad[quad_num].weight);
            //cout<<"rightmatrix steps"<<RightMatrix_X[j]<<"::"<<RightMatrix_Y[j]<<endl;
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
    //vector<vector<int> > LeftMatrix;
    LeftMatrix.clear();
    
    GetLeftMatrix(AllWeights, Blocks, &LeftMatrix, numOfBlocks);

    cout<<endl<<"for x"<<endl;
    for(int a1 = 0; a1 < RightMatrix_X.size(); a1++){
            cout << RightMatrix_X[a1]<<"::";
    
    }
    cout<<endl<<"for Y"<<endl;
      for(int a1 = 0; a1 < RightMatrix_Y.size(); a1++){
            cout << RightMatrix_Y[a1]<<"::";

    }
    cout<<endl;
    
    double* x_after = NULL;
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
        bx_after[i] = (RightMatrix_X[i])*10000;
        by_after[i] = (RightMatrix_Y[i])*10000;
    }
     
    double* y_after = NULL;
    doSolve(A_after, dim_after, &x_after, bx_after);
    doSolve(A_after, dim_after, &y_after, by_after);
    
    cout<<"after spreading"<<endl;
    for(int i =0; i<dim; i++){
        cout<<"x"<<i<<":"<<x_after[i]<<":y: "<<y_after[i]<<endl;
    }
    
    
    DrawOnScreen();
    
    if (Nets){
        delete[] Nets;
    }
    
    cout<<"Done"<<endl;
    
    return 0;
}