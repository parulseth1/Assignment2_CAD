//
//  Parser.cpp
//  Parser
//
//  Created by AliakbarEski on 2015-10-13.
//  Copyright (c) 2015 AliakbarEski. All rights reserved.
//

#include <stdio.h>
#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include "Parser.h"
using namespace::std;



#define BLOCK_NET_NUM 1
#define FIXED_POINT 2

int reading;

int doParse(char* FILENAME, vector<block>* Blocks, Net** Nets, int* numberOfNets){
    
    reading = BLOCK_NET_NUM;
    ifstream fin;
    string line;
    fin.open(FILENAME);
    if (!fin.is_open()){
        cout<<"Couldnt open file"<<endl;
        return -1;
    }
    
    
    //start reading the file in
    bool quit = false;
    while (!quit){
        
        block temp;
//        fin.getline(lineIn, 100); //nothing being read? why?
        getline (fin, line);
        if (fin.eof()){
            quit = true;
            break;
        }
        
        if (line == "-1" && reading == BLOCK_NET_NUM){
            //switch reading to FIXED_POINT
            reading = FIXED_POINT;
        }
        else if (line == "-1" && reading == FIXED_POINT){
            quit = true;
            break;
        }
        
        stringstream ss1(line);
        int blocknum;
        int net_num;
        if (reading == BLOCK_NET_NUM){
            //extract the blocknum
            ss1>>blocknum;
            temp.setBlockNum(blocknum);
            
            //extract the netnums
            bool endOfBlock = false;
            while(!endOfBlock){
                ss1>>net_num;
                if (net_num == -1){
                    endOfBlock = true;
                    break;
                }
                else {
                    temp.getNetNum()->push_back(net_num);
                    //while reading from netnum, we can get an idea of how many nets are there.
                    if (*numberOfNets < net_num){
                        *numberOfNets = net_num;
                    }
                }
            }
            Blocks->push_back(temp);
        }
        
        
        
        if (reading == FIXED_POINT){
            //extract the blocknum
            int blocknum;
            ss1>>blocknum;
            int x, y;
            //extract the x, y points
            ss1>>x>>y;
            (*Blocks)[blocknum-1].setx(x);
            (*Blocks)[blocknum-1].sety(y);
        }
    
    }
    
    //now reserve that space in the Nets vector
    *Nets = new Net[*numberOfNets];
    
    //go through the entire array of each block's netnums and calculate the no. of pins that connect to each netnum
    for (int h = 0; h < Blocks->size(); h++){
        vector<int>* NetNumTemp = (*Blocks)[h].getNetNum();
        for (int g = 0; g < NetNumTemp->size(); g++){
            (*Nets)[(*NetNumTemp)[g]-1].incrementNumPins();
        }
    }
    
    
    return 0;
}

