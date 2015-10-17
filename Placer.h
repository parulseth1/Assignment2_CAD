/* 
 * File:   Placer.h
 * Author: eski
 *
 * Created on October 17, 2015, 6:30 PM
 */

#ifndef PLACER_H
#define	PLACER_H

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
        return x;
    }
    void sety(int _y){
        fixed = true;
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
    vector<int> BlockNums;
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
    vector<int>* getBlockNums(){
        return &BlockNums;
    }
    
};

#endif	/* PLACER_H */

