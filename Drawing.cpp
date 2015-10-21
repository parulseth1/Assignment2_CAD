#include "Drawing.h"
#include <iostream>

using namespace::std;

void drawscreen();

int* x = NULL;
int* y = NULL;
int gridSize = 10;
int blockCnt = 0;
Net* nets = NULL;
int netCnt = 0;
vector<block> Blocks;

int LoadBlocks(int* _x, int* _y, int _blockCnt, int _gridSize) {

    x = _x;
    y = _y;
    blockCnt = _blockCnt;
    gridSize = _gridSize;
}

int LoadRatsNest(Net* _nets, int _netCnt) {
    nets = _nets;
    netCnt = _netCnt;
}

int DrawOnScreen() {
    init_graphics("Analytical Placer", WHITE);

    set_visible_world(0, 0, 1000, 1000);

    event_loop(NULL, NULL, NULL, drawscreen);

    close_graphics();
    return 0;
}

void drawscreen() {
    //draw the main grid and the wire mesh - gridSize x gridSize grid i.e.
    //draw the main rect first
    float blockDim = 1000 / (gridSize + 2);
    float rectOffset = blockDim;

    setcolor(135, 206, 250); //blue
    fillrect(rectOffset, rectOffset, rectOffset + blockDim*gridSize, rectOffset + blockDim * gridSize);

    //draw the mesh lines
    setcolor(0, 0, 0); //black
    for (int i = 0; i <= gridSize; i++) {
        drawline(rectOffset + blockDim*i, rectOffset, rectOffset + blockDim*i, rectOffset + gridSize * blockDim);
        drawline(rectOffset, rectOffset + blockDim*i, rectOffset + gridSize*blockDim, rectOffset + blockDim * i);
    }

    //draw the blocks
    setcolor(184, 184, 184); //grey
    for (int i = 0; i < blockCnt; i++) {
        fillrect(rectOffset + blockDim * (x[i]), rectOffset + blockDim * (y[i]), rectOffset + blockDim * (x[i] + 1), rectOffset + blockDim * (y[i] + 1));
    }

    //draw the rats nest
    setcolor(255, 0, 0);
    //for each net 
    if (nets != NULL){
        for (int i = 0; i < netCnt; i++){
            vector<int>* blocks = nets[i].getBlockNums();
            for (int j = 0; j < blocks->size(); j++){
                for (int k = j + 1; k < blocks->size(); k++){
                    int x1 = x[(*blocks)[j]-1];
                    int y1 = y[(*blocks)[j]-1];
                    int x2 = x[(*blocks)[k]-1];
                    int y2 = y[(*blocks)[k]-1];
                    drawline(rectOffset + blockDim*x1 + blockDim/2, rectOffset + blockDim*y1 + blockDim/2, rectOffset + blockDim*x2 + blockDim/2, rectOffset + blockDim*y2 + blockDim/2);
                }
            }
        }
    }
    //	
}
