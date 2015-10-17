//
//  Parser.h
//  Parser
//
//  Created by AliakbarEski on 2015-10-13.
//  Copyright (c) 2015 AliakbarEski. All rights reserved.
//

#ifndef Parser_Parser_h
#define Parser_Parser_h

#include <iostream>
#include <vector>
using namespace::std;

class block{
private:
    int blockNum;
    vector<int> netnum;
    int x, y;
    bool fixed;
public:
    void setBlockNum(int _blockNum){
        blockNum = _blockNum;
    }
    int getBlockNum(){
        return blockNum;
    }
    vector<int>* getNetNum(){
        return &netnum;
    }
    void setx(int _x){
        fixed = true;
        x = _x;
    }
    int getx(){
        fixed = true;
        return x;
    }
    void sety(int _y){
        y = _y;
    }
    int gety(){
        return y;
    }
    
    block(){
        x = 0;
        y = 0;
        fixed = false;
    }
    
    bool getFixed(){
        return fixed;
    }
};

class Net{
private:
    int numPins;
    double weight;
public:
    Net(){
        numPins = 0;
    }
    void incrementNumPins(){
        numPins++;
        if (numPins != 0){
            weight = 2/numPins;
        }
    }
    void setNumPins(int _numPins){
        numPins = _numPins;
    }
    int getNumPins(){
        return numPins;
    }
    int getPinWeight(){
        return weight;
    }
    
};

int doParse(char* FILENAME, vector<block>* Blocks, Net** Nets, int* numberOfNets);

#endif
