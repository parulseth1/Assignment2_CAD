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
#include "Placer.h"

using namespace::std;

int doParse(char* FILENAME, vector<block>* Blocks, Net** Nets, int* numberOfNets);

#endif
