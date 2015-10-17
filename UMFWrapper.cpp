//
//  UMFWrapper.cpp
//  UMFPackWrapper
//
//  Created by AliakbarEski on 2015-10-16.
//  Copyright (c) 2015 AliakbarEski. All rights reserved.
//

#include "UMFWrapper.h"
#include <iostream>

using namespace::std;

#define DEBUGUMF

int doSolve(int** A, int dim, int** x, int* b){
    //A is a square matrix
    //from A we first have to create the matrixes Ti, Tj and Tx
    //first get the number non zero elements, nz
    
    int nz = 0;
    for (int i = 0; i < dim; i++){
        for (int j = 0; j < dim; j++){
            if (A[i][j] != 0){
                nz++;
            }
        }
    }
    
    int* Ti = new int [nz]; int TiIndex = 0;
    int* Tj = new int [nz]; int TjIndex = 0;
    int* Tx = new int [nz]; int TxIndex = 0;
    
    for (int j = 0; j < dim; j++){
        for (int i = 0; i < dim; i++){
            if (A[i][j] != 0){
                //push this into the Ti, Tj, and Tx vectors
                Tx[TxIndex++] = A[i][j];
                Ti[TiIndex++] = i;
                Tj[TjIndex++] = j;
            }
        }
    }
    
#ifdef DEBUGUMF
    for (int i = 0; i < nz; i++){
        cout<<Ti[i]<<"::"<<Tj[i]<<"::"<<Tx[i]<<endl;
    }
#endif
    
    //having got the Ti, Tj and Tx matrices, we now just have to call the UMFPack Solver
    
    
    delete[] Ti;
    delete[] Tj;
    delete[] Tx;
    
    return 0;
}