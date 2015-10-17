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
#define FILENAME "/home/parul/NetBeansProjects/AnalyticalPlacer/cct1.txt"
using namespace::std;

int main(int argc, const char * argv[]) {
    cout<<"Parsing"<<endl;
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
    
    cout<<"Done";
    return 0;
}
