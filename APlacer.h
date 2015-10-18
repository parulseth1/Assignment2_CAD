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

void setTotalWeight(block, Net**);
vector<int> getCorrespondingWeights(block, Net**, int,int);
void GetLeftMatrix(vector<vector<int> >, vector<block>,vector<vector<int> >*,int);
void ForXGetRightMatrix(vector<vector<int> > , vector<block>, vector<int>* , int);
void ForYGetRightMatrix(vector<vector<int> > , vector<block>, vector<int>* , int);
int CalculateHPWL(Net**, vector<block>, int);
#endif	/* APLACER_H */

