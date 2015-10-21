//
//  UMFWrapper.cpp
//  UMFPackWrapper
//
//  Created by AliakbarEski on 2015-10-16.
//  Copyright (c) 2015 AliakbarEski. All rights reserved.
//

#include "UMFWrapper.h"
#include <iostream>
extern "C" {
#include "SuiteSparse/UMFPACK/Include/umfpack.h"
}

using namespace::std;

#define DEBUGUMF

int doSolve(int** A, int dim, double** x, double* b){
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
    double* Tx = new double [nz]; int TxIndex = 0;
    
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
    int* Ap = new int[dim + 1];
    int* Ai = new int[nz];
    double* Ax = new double[nz];
    int status = umfpack_di_triplet_to_col(dim, dim, nz, Ti, Tj, Tx,
                    Ap, Ai, Ax, NULL);
    
    

    if (status != UMFPACK_OK) {
        cout<<"Error"<<endl;
        exit(1);
    }
    
    double *null = (double *) NULL;
    int i;
    void *Symbolic, *Numeric;

    (void) umfpack_di_symbolic(dim, dim, Ap, Ai, Ax, &Symbolic, null, null);
    (void) umfpack_di_numeric(Ap, Ai, Ax, Symbolic, &Numeric, null, null);
    umfpack_di_free_symbolic(&Symbolic);
    (void) umfpack_di_solve(UMFPACK_A, Ap, Ai, Ax, *x, b, Numeric, null, null);
    umfpack_di_free_numeric(&Numeric);
    for (i = 0; i < dim; i++) printf("x [%d] = %g\n", i, *x [i]);
    
    
    delete[] Ti;
    delete[] Tj;
    delete[] Tx;
    
    return 0;
}