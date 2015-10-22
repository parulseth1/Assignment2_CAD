//
//  APlacer.cpp
//  APlacer
// Parul Seth
//
#include "APlacer.h"
#include <stdio.h>
#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include "Placer.h"
#define INTMAX 12345
#define INTMIN -1

// have to get the weights put in for each block using the nets it is connected to.
double setTotalWeight(block Block1, Net** nets){ // some issue here. got to fix it
    vector<int>* net = Block1.getNetNum();
    double weight = 0;
    //int Netweight
    for(int a =0; a< net->size(); a++){  // dont know if we have to use dot or arrow.
        int numpins = (*nets)[((*net)[a]-1)].getNumPins();
        double Netweight = (*nets)[((*net)[a]-1)].getPinWeight() * (numpins-1);
        weight = weight + Netweight; 
        
    }
    cout<< weight<<endl;
   return weight; 
    
}

vector<double> getCorrespondingWeights(block Block1, Net** nets, int blocknumber, int TotalNumOfPin ){
    vector<int>* net = Block1.getNetNum();
    vector<double> weights(TotalNumOfPin);
    //vector<WeightandPin> w1;
    vector<WeightandPin> w1(TotalNumOfPin);
    WeightandPin w;
    double total = Block1.getTotalWeight();
    cout<<"total weight:: "<<total<<endl;
    for(int a = 0; a< net->size(); a++){  // arrow or dot

        vector<int>* blocknums = (*nets)[((*net)[a]-1)].getBlockNums();
        for(int b = 0; b<blocknums->size(); b++){ //arrow or dot
            w.blocknum = blocknums->at(b);
            w.weight = ((*nets)[((*net)[a]-1)].getPinWeight());
            w.weight = -w.weight;
            int count = 0;
            for(int c = 0; c< w1.size(); c++){
                if(w.blocknum == w1[c].blocknum){
                    if(w.blocknum == blocknumber){
                        w1[c].weight = Block1.getTotalWeight();
                        //cout<<"total weight"<<endl;
                        count++;
                    }
                    else{
                        w1[c].weight = (w1[c].weight + w.weight);
                        count = count+1;
                    }
                }
                    
                    
            }
            if(count == 0){
                w1.push_back(w); // dot or arrow
            }
        } 
        
    }
    //cout<<"we got the corresponding weights"<<endl;
    for(int e =0; e< TotalNumOfPin; e++){
        int counter =0;
        for(int f = 0; f< w1.size(); f++){
            if((e+1 )== w1[f].blocknum){
                counter = counter +1;
                weights[(w1[f].blocknum)-1] = w1[f].weight;
            }
       
        }
        if (counter == 0){
            weights[e] = 0;
        }
    }
    return weights;
}


void GetLeftMatrix(vector<vector<double>> weights, vector<block> Blocks, vector<vector<double>>* LeftMatrix, int numOfBlocks){
    vector<double> weight;
    vector<double> row(numOfBlocks);
    for(int a =0; a< numOfBlocks; a++){
        //int counter = 0;
        if(Blocks[a].getFixed() != true){
            weight = weights[a];
            for(int b=0; b< numOfBlocks; b++){
                if(Blocks[b].getFixed() != true){
                    row.push_back(weight[b]);
                    //cout<<weight[b]<<endl;
                    //counter++;
                }
            }
            //cout<<endl;
            //cout<<"RowSizefrom get left matrix"<<row.size()<<endl;
            LeftMatrix->push_back(row);
        }
        
        row.clear();
    }
    
}

void ForXGetRightMatrix(vector<vector<double>> weights, vector<block> Blocks, vector<double>* RightMatrix, int numOfBlocks){
   vector<double> weight;
    //int row =0;
    for(int a =0; a< numOfBlocks; a++){
        if(Blocks[a].getFixed() != true){
            weight = weights[a];
            double row = 0;
            for(int b=0; b< numOfBlocks; b++){
                if(Blocks[b].getFixed() == true){
                    double x = Blocks[b].getx();
                    row = row - (weight[b]*x);
                }
            }
            RightMatrix->push_back(row);
        }
        
    }
     
}


void ForYGetRightMatrix(vector<vector<double>> weights, vector<block> Blocks, vector<double>* RightMatrix, int numOfBlocks){
   vector<double> weight;
    //int row =0;
    for(int a =0; a< numOfBlocks; a++){
        if(Blocks[a].getFixed() != true){
            weight = weights[a];
            double row = 0;
            cout<<Blocks[a].getBlockNum()<<endl;
            for(int b=0; b< numOfBlocks; b++){
                if(Blocks[b].getFixed() == true){
                    double x = Blocks[b].gety();
                    row = row - (weight[b]*x);
                    //cout<<"weight at"<<b+1<<":"<<weight[b]<<endl;
                }
            }
            RightMatrix->push_back(row);
        }
        
    }
     
}
  
    

double CalculateHPWL(Net** net, vector<block> Blocks, int NumOfNets){
    double hpwl =0;
    for (int a = 0; a < NumOfNets;a++){
        vector<int>* blocknums = (*net)[a].getBlockNums();
        double xMax = INTMIN;
        double yMax = INTMIN;
        double xMin = INTMAX;
        double yMin = INTMAX;
        double delx; double dely;
        for(int b = 0; b< blocknums->size(); b++){
            double x = Blocks[(*blocknums)[b]-1].getx();
            double y = Blocks[(*blocknums)[b]-1].gety();
            if(x < xMin){
                xMin = x;
            }
            if(x> xMax){
                xMax = x;
            }
            
            if(y < yMin){
                yMin = y;
            }
            if(y> yMax){
                yMax = y;
            }
            
        }
        delx = xMax - xMin;
        dely = yMax - yMin;
        hpwl = hpwl+ delx+ dely;
    }
    return hpwl;
}

point getCentroid(vector<block> Blocks,int num){
    point centroid;
    point points;
    centroid.x = 0;
    centroid.y = 0;   
 for(int a = 0; a<num; a++){
    points.x = Blocks[a].getx();
    points.y = Blocks[a].gety();
    centroid.x = centroid.x + points.x;
    centroid.y = centroid.y + points.y;
 }  
   centroid.x = centroid.x/num;
   centroid.y = centroid.y/num;
   return centroid;
}


void MakeQuads(vector<block> Block, int size_quad, double weight, quadrant** quads){
    //quadrant quads[4];
    point dummy;
        for(int a=0; a<4; a++){
            (*quads)[a].quad_num = a+1;
            (*quads)[a].size = size_quad/2;
            if(a == 0){
                dummy.x = (size_quad)/2;
                dummy.y = (size_quad/2) + size_quad;   
            }
            if(a == 1){
                dummy.x = (size_quad)/2;
                dummy.y = (size_quad)/2;   
            }
            if(a == 2){
                dummy.y = (size_quad)/2;
                dummy.x = (size_quad/2) + size_quad;   
            }
            if(a== 3){
                dummy.x = (size_quad/2) + size_quad;
                dummy.y = (size_quad/2) + size_quad;   
            }
            (*quads)[a].dummy = dummy;
            (*quads)[a].weight = weight;
            
        }
   
}
int PutBoxInQuads(block Block, point centroid){
   
    double x = Block.getx();
    double y = Block.gety();
    int quad_num;
    if(x< centroid.x){
        if( y < centroid.y){ // quad 3
            quad_num = 2;
        }
        if(y > centroid.y){ // quad 2
            quad_num = 1;
        }
    }
    if(x> centroid.x){

        if( y < centroid.y){ // quad 4
            quad_num =3;
        }
        if(y > centroid.y){ // quad 1
            quad_num =0;
        }
    }    
    return quad_num;   
}








