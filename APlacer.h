/* 
 * File:   Aplacer.h
 * Author: parul
 *
 * Created on October 17, 2015, 1:00 PM
 */

#ifndef APLACER_H
#define	APLACER_H

#include "Parser.h"
#include <vector>
#include "Placer.h"

double setTotalWeight(block, Net**);
vector<double> getCorrespondingWeights(block, Net**, int,int);
void GetLeftMatrix(vector<vector<double>>, vector<block>,vector<vector<double>>*,int);
void ForXGetRightMatrix(vector<vector<double>> , vector<block>, vector<double>* , int);
void ForYGetRightMatrix(vector<vector<double>> , vector<block>, vector<double>* , int);
double CalculateHPWL(Net**, vector<block>, int);
point getCentroid(vector<block>, int);
int PutBoxInQuads(block, point);
void MakeQuads(vector<block>, int, double, quadrant**);


#endif	/* APLACER_H */

