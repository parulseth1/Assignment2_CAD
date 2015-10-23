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
    double x, y;
    bool fixed;
    double totalWeight=0;
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
    void setx(double _x){
        x = _x;
    }
    double getx(){
        return x;
    }
    void sety(double _y){
        y = _y;
    }
    double gety(){
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
    
    void setFixed(bool _fixed){
        fixed = _fixed;
    }
    void AddTotalWeight(double weight){
        totalWeight = totalWeight + weight;
    }
    
    double getTotalWeight(){ 
        return totalWeight;
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
            weight = 2.0/numPins;
            
        }
    }
    void setNumPins(int _numPins){
        numPins = _numPins;
    }
    int getNumPins(){
        return numPins;
    }
    double getPinWeight(){
        return weight;
    }
    vector<int>* getBlockNums(){
        return &BlockNums;
    }
    
};

struct WeightandPin{
    int blocknum;
    double weight;
};

struct point{
    double x;
    double y;
};

struct quadrant{
    point dummy;
    int quad_num;
    vector<int> blocknums;
    int size;
    double weight;
};

#endif	/* PLACER_H */

