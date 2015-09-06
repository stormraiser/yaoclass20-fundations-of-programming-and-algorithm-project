/**
  * Region.hpp
  * Defines class template Region. This class template is
  * responsible for delailed control over the evolution process.
  *
  * Author: StormRaiser (Xiang Sitao)
  */

#pragma once

#include "Rule.h"
#include "Common.h"
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <ctime>

using std::cout;
using std::endl;
using std::min;
using std::max;

#define REGION_SIZE_REAL (REGION_SIZE + 2)

namespace CACore{

template <typename ValType> class Grid;

template <typename ValType> class Region{
private:
    int minX, minY, maxX, maxY, area;                                // Maximum and minimum local cell coordinates
    int minCX, minCY, maxCX, maxCY;                                  // Maximum and minimum local chunk coordinates
    int renderCount, ri;                                             // Number of cell blocks to be rendered
    int idleTime;                                                    // How long has this region been silent
    long long currentTime;                                                  // Timestamp
    ValType *data, *dataBuffer, tState;                              // Cell data and buffer
    Region<ValType> *link[3][3], *pt;                                // Pointer to neighbours
    int flag[CHUNK_NUMBER][CHUNK_NUMBER], flagBuffer[CHUNK_NUMBER][CHUNK_NUMBER];               // How long has one cell remain unchanged
    int cMinX[CHUNK_NUMBER], cMaxX[CHUNK_NUMBER], cMinY[CHUNK_NUMBER], cMaxY[CHUNK_NUMBER];     // Maximum and minimum local cell coordinate for chunks
    int renderList[CHUNK_NUMBER * CHUNK_NUMBER][4];                                             // List of cell blocks to  be rendered
    bool cUpdated[CHUNK_NUMBER][CHUNK_NUMBER], cUpdatedBuffer[CHUNK_NUMBER][CHUNK_NUMBER];      // Whether a chunk has been updated
    bool cUpdatedRec[CHUNK_NUMBER][CHUNK_NUMBER];                    // Whether a chunk has been updated since last render
    long long stat[256];                                             // Count of cell in different states
    bool updated;                                                    // Flags
    Rule<ValType> *rule;                                             // Pointer to the evolution rule
    Grid<ValType> *parent;                                           // Pointer to the grid to whom this region belong

    inline ValType *cell(int x, int y)                               // Get a cell within region
        {return data + x * REGION_SIZE_REAL + y;};
    inline ValType *cellBuffer(int x, int y)
        {return dataBuffer + x * REGION_SIZE_REAL + y;};
    ValType findCell(int x, int y);

    void evolveType0();
    void evolveType1();                                              // Perform the evolution process with rule of different types
    void evolveType2();

public:
    int regionX, regionY;                                            // Global region coordinates of this region
    Region(Grid<ValType> *_parent, int _regionX, int _regionY);
    ~Region();
    void setRule(Rule<ValType> *_rule);
    void cleanUp();
    void prepare();
    void evolve();
    void clearFlag();
    void clearRender();
    int randomize(double *pos);
    int putCell(int x, int y, ValType state);
    inline ValType getCell(int x, int y){return *cell(x, y);};
    inline bool isDead(){return idleTime >= IDLE_TIME_LIMIT;};
    void renderInitialize(int x1, int y1, int x2, int y2);
    bool getRenderRegion(int &x1, int &y1, int &x2, int &y2);
    int population(){return area - stat[0];};
    int population(int index){return stat[index];};
    void putCellArray(ValType *p_data);
};

template <typename ValType> ValType Region<ValType>::findCell(int x, int y){
    if (x < minX){
        if (link[0][1] == NULL)
            parent->getRegion(regionX - 1, regionY);
        return link[0][1]->findCell(link[0][1]->maxX + x - minX + 1, y);
    }
    else if (x > maxX){
        if (link[2][1] == NULL)
            parent->getRegion(regionX + 1, regionY);
        return link[2][1]->findCell(link[2][1]->minX + x - maxX - 1, y);
    }
    else if (y < minY){
        if (link[1][0] == NULL)
            parent->getRegion(regionX, regionY - 1);
        return link[1][0]->findCell(x, link[1][0]->maxY + y - minY + 1);
    }
    else if (y > maxY){
        if (link[1][2] == NULL)
            parent->getRegion(regionX, regionY + 1);
        return link[1][2]->findCell(x, link[1][2]->minY + y - maxY - 1);
    }
    else return *cellBuffer(x, y);
}

template <typename ValType> Region<ValType>::Region(Grid<ValType> *_parent, int _regionX, int _regionY){
    int i, j;
    regionX = _regionX;
    regionY = _regionY;
    parent = _parent;
    currentTime = parent->getTime();
    rule = parent->getRule();

    // local coordinate related calculations
    if (regionX << REGION_SCALE_FULL < parent->minX())
        minX = parent->minX() - (regionX << REGION_SCALE_FULL);
    else
        minX = 0;
    minCX = minX >> CHUNK_SCALE;
    if (regionY << REGION_SCALE_FULL < parent->minY())
        minY = parent->minY() - (regionY << REGION_SCALE_FULL);
    else
        minY = 0;
    minCY = minY >> CHUNK_SCALE;
    if (((regionX + 1) << REGION_SCALE_FULL) - 1 > parent->maxX())
        maxX = parent->maxX() - (regionX << REGION_SCALE_FULL);
    else
        maxX = REGION_SIZE - 1;
    maxCX = maxX >> CHUNK_SCALE;
    if (((regionY + 1) << REGION_SCALE_FULL) - 1 > parent->maxY())
        maxY = parent->maxY() - (regionY << REGION_SCALE_FULL);
    else
        maxY = REGION_SIZE - 1;
    maxCY = maxY >> CHUNK_SCALE;

    for (i = minCX; i <= maxCX; i++){
        if (i * CHUNK_SIZE < minX)
            cMinX[i] = minX;
        else
            cMinX[i] = i << CHUNK_SCALE;
        if ((i + 1) * CHUNK_SIZE - 1 > maxX)
            cMaxX[i] = maxX;
        else
            cMaxX[i] = ((i + 1) << CHUNK_SCALE) - 1;
    }
    for (j = minCY; j <= maxCY; j++){
        if (j * CHUNK_SIZE < minY)
            cMinY[j] = minY;
        else
            cMinY[j] = j << CHUNK_SCALE;
        if ((j + 1) * CHUNK_SIZE - 1 > maxY)
            cMaxY[j] = maxY;
        else
            cMaxY[j] = ((j + 1) << CHUNK_SCALE) - 1;
    }

    // establish link to neighbours and back
    for (i = 0; i < 3; i++)
        for (j = 0; j < 3; j++)
            if ((i != 1) || (j != 1)){
                link[i][j] = parent->findRegion(regionX + i - 1, regionY + j - 1);
                if (link[i][j] != NULL)
                    link[i][j]->link[2 - i][2 - j] = this;
            }

    // initialize
    data = new ValType[REGION_SIZE_REAL * REGION_SIZE_REAL];
    data += REGION_SIZE_REAL + 1;
    dataBuffer = new ValType[REGION_SIZE_REAL * REGION_SIZE_REAL];
    dataBuffer += REGION_SIZE_REAL + 1;
    for (i = -1; i < REGION_SIZE_REAL - 1; i++)
        for (j = -1; j < REGION_SIZE_REAL - 1; j++)
            *cell(i, j) = *cellBuffer(i, j) = 0;
    for (i = 0; i < CHUNK_NUMBER; i++)
        for (j = 0; j < CHUNK_NUMBER; j++){
            flag[i][j] = rule->period();
            cUpdated[i][j] = cUpdatedBuffer[i][j] = false;
        }
    if (link[0][1] != NULL){
        for (j = minY; j <= maxY; j++)
            *cell(minX - 1, j) = *(link[0][1]->cell(link[0][1]->maxX, j));
    }
    if (link[2][1] != NULL){
        for (j = minY; j <= maxY; j++)
            *cell(maxX + 1, j) = *(link[2][1]->cell(link[2][1]->minX, j));
    }
    if (link[1][0] != NULL){
        for (i = minX; i <= maxX; i++)
            *cell(i, minY - 1) = *(link[1][0]->cell(i, link[1][0]->maxY));
    }
    if (link[1][2] != NULL){
        for (i = minX; i <= maxX; i++)
            *cell(i, maxY + 1) = *(link[1][2]->cell(i, link[1][2]->minY));
    }
    if (link[0][0] != NULL)
        *cell(minX - 1, minY - 1) = *(link[0][0]->cell(link[0][0]->maxX, link[0][0]->maxY));
    if (link[0][2] != NULL)
        *cell(minX - 1, maxY + 1) = *(link[0][2]->cell(link[0][2]->maxX, link[0][2]->minY));
    if (link[2][0] != NULL)
        *cell(maxX + 1, minY - 1) = *(link[2][0]->cell(link[2][0]->minX, link[2][0]->maxY));
    if (link[2][2] != NULL)
        *cell(maxX + 1, maxY + 1) = *(link[2][2]->cell(link[2][2]->minX, link[2][2]->minY));
    idleTime = 0;
    stat[0] = area = (maxX - minX + 1) * (maxY - minY + 1);
    for (i = 1; i < 256; i++)
        stat[i] = 0;
}

template <typename ValType> Region<ValType>::~Region(){
    int i, j;
    data -= (REGION_SIZE_REAL + 1);
    delete [] data;
    dataBuffer -= (REGION_SIZE_REAL + 1);
    delete [] dataBuffer;
    for (i = 0; i < 3; i++)
        for (j = 0; j < 3; j++)
            if (((i != 1) || (j != 1)) && (link[i][j] != NULL))
                link[i][j]->link[2 - i][2 - j] = NULL;
}

template <typename ValType> void Region<ValType>::clearFlag(){
    int ci, cj;
    for (ci = minCX; ci <= maxCX; ci++)
        for (cj = minCY; cj <= maxCY; cj++){
            cUpdated[ci][cj] = cUpdatedBuffer[ci][cj] = true;
            flag[ci][cj] = flagBuffer[ci][cj] = 0;
        }
}

template <typename ValType> void Region<ValType>::clearRender(){
    int ci, cj;
    for (ci = minCX; ci <= maxCX; ci++)
        for (cj = minCY; cj <= maxCY; cj++)
            cUpdatedRec[ci][cj] = false;
}

// swap buffer, clear flags
// must be explicitly called before evolve()
template <typename ValType> void Region<ValType>::prepare(){
    int ci, cj, i, j;
    if (parent->getTime() <= currentTime)
        return;
    updated = false;
    for (ci = minCX; ci <= maxCX; ci++)
        for (cj = minCY; cj <= maxCY; cj++)
            if (cUpdatedBuffer[ci][cj]){
                flag[ci][cj] = 0;
                updated = true;
            }
            else if (flag[ci][cj] < rule->period())
                flag[ci][cj]++;
    if ((!updated) && (stat[0] == (maxX - minX + 1) * (maxY - minY + 1))){
        if (idleTime < IDLE_TIME_LIMIT)
            idleTime++;
    }
    else
        idleTime = 0;
    ValType *t = data;
    data = dataBuffer;
    dataBuffer = t;
    for (i = minCX; i <= maxCX; i++)
        for (j = minCY; j <= maxCY; j++)
            cUpdated[i][j] = cUpdatedBuffer[i][j] = false;
}

template <typename ValType> void Region<ValType>::evolve(){
    int i;
    if (parent->getTime() <= currentTime)
        return;
    currentTime++;
    switch (rule->ruleType()){
    case 0:
        evolveType0();
        break;
    case 1:
        evolveType1();
        break;
    case 2:
        evolveType2();
    }
}

// evolve functions
// do the calculation

// remember the logic:
// flag == k --> (t-1) == (t-2) == (t-k-1)
// flag < period --> forced check for update
// cUpdated: updated due to itself
// cUpdatedBuffer: updated due to itself or adjacent chunks

template <typename ValType> void Region<ValType>::evolveType0(){
    int ci, cj, i, j;
    RuleType0<ValType> *tRule = (RuleType0<ValType>*)(rule);
    for (ci = minCX; ci <= maxCX; ci++)
        for (cj = minCY; cj <= maxCY; cj++)
            if (flag[ci][cj] == 0){
                for (i = cMinX[ci]; i <= cMaxX[ci]; i++)
                    for (j = cMinY[cj]; j <= cMaxY[cj]; j++){
                        tState = tRule->calc(cellBuffer(i - 1, j - 1), cellBuffer(i, j - 1), cellBuffer(i + 1, j - 1));
                        if (tState != *cellBuffer(i, j)){
                            stat[(unsigned char)(*cellBuffer(i, j))]--;
                            stat[(unsigned char)(tState)]++;
                            cUpdated[ci][cj] = cUpdatedBuffer[ci][cj] = true;
                        }
                        *cell(i, j) = tState;
                    }
                cUpdatedRec[ci][cj] = cUpdatedRec[ci][cj] || cUpdated[ci][cj];
            }
}

template <typename ValType> void Region<ValType>::evolveType1(){
    RuleType1<ValType> *tRule = (RuleType1<ValType>*)(rule);
    int ci, cj, i, j, t = currentTime % rule->period();
    for (ci = minCX; ci <= maxCX; ci++)
        for (cj = minCY; cj <= maxCY; cj++)
            if (flag[ci][cj] < rule->period()){
                for (i = cMinX[ci]; i <= cMaxX[ci]; i++)
                    for (j = cMinY[cj]; j <= cMaxY[cj]; j++){
                        tState = tRule->calc(cellBuffer(i - 1, j - 1), cellBuffer(i, j - 1), cellBuffer(i + 1, j - 1),
                                            (regionX * REGION_SCALE_FULL) + i, (regionY * REGION_SCALE_FULL) + j, t);
                        if (tState != *cellBuffer(i, j)){
                            stat[(unsigned char)(*cell(i, j))]--;
                            stat[(unsigned char)(tState)]++;
                            cUpdated[ci][cj] = cUpdatedBuffer[ci][cj] = true;
                        }
                        *cell(i, j) = tState;
                    }
                cUpdatedRec[ci][cj] = cUpdatedRec[ci][cj] || cUpdated[ci][cj];
            }
}

template <typename ValType> void Region<ValType>::cleanUp(){
    int ci, cj, i, j;

    if (rule->ruleType() == 2)
        return;

    if (rule->period() != 0){
        // check whether a chunk needs to be checked next time step due to its neighbour being updated
        for (ci = minCX; ci <= maxCX; ci++)
            for (cj = minCY; cj <= maxCY; cj++)
                if (cUpdated[ci][cj]){
                    if ((ci != minCX) && (cj != minCY) && !cUpdatedBuffer[ci - 1][cj - 1] &&
                        (*cell(cMinX[ci], cMinY[cj]) != *cellBuffer(cMinX[ci], cMinY[cj])))
                        cUpdatedBuffer[ci - 1][cj - 1] = true;
                    if ((ci != minCX) && (cj != maxCY) && !cUpdatedBuffer[ci - 1][cj + 1] &&
                        (*cell(cMinX[ci], cMaxY[cj]) != *cellBuffer(cMinX[ci], cMaxY[cj])))
                        cUpdatedBuffer[ci - 1][cj + 1] = true;
                    if ((ci != maxCX) && (cj != minCY) && !cUpdatedBuffer[ci + 1][cj - 1] &&
                        (*cell(cMaxX[ci], cMinY[cj]) != *cellBuffer(cMaxX[ci], cMinY[cj])))
                        cUpdatedBuffer[ci + 1][cj - 1] = true;
                    if ((ci != maxCX) && (cj != maxCY) && !cUpdatedBuffer[ci + 1][cj + 1] &&
                        (*cell(cMaxX[ci], cMaxY[cj]) != *cellBuffer(cMaxX[ci], cMaxY[cj])))
                        cUpdatedBuffer[ci + 1][cj + 1] = true;
                    if ((ci != minCX) && !cUpdatedBuffer[ci - 1][cj])
                        for (j = cMinY[cj]; j <= cMaxY[cj]; j++)
                            if (*cell(cMinX[ci], j) != *cellBuffer(cMinX[ci], j)){
                                cUpdatedBuffer[ci - 1][cj] = true;
                                break;
                            }
                    if ((ci != maxCX) && !cUpdatedBuffer[ci + 1][cj])
                        for (j = cMinY[cj]; j <= cMaxY[cj]; j++)
                            if (*cell(cMaxX[ci], j) != *cellBuffer(cMaxX[ci], j)){
                                cUpdatedBuffer[ci + 1][cj] = true;
                                break;
                            }
                    if ((cj != minCY) && !cUpdatedBuffer[ci][cj - 1])
                        for (i = cMinX[ci]; i <= cMaxX[ci]; i++)
                            if (*cell(i, cMinY[cj]) != *cellBuffer(i, cMinY[cj])){
                                cUpdatedBuffer[ci][cj - 1] = true;
                                break;
                            }
                    if ((cj != maxCY) && !cUpdatedBuffer[ci][cj + 1])
                        for (i = cMinX[ci]; i <= cMaxX[ci]; i++)
                            if (*cell(i, cMaxY[cj]) != *cellBuffer(i, cMaxY[cj])){
                                cUpdatedBuffer[ci][cj + 1] = true;
                                break;
                            }
                }

        // propagate update flags to neighbours
        for (cj = minCY; cj <= maxCY; cj++){
            if (cUpdated[minCX][cj]){
                for (j = cMinY[cj]; j <= cMaxY[cj]; j++)
                    if (*cell(minX, j) != *cellBuffer(minX, j)){
                        if (link[0][1] == NULL)
                            parent->getRegion(regionX - 1, regionY);
                        link[0][1]->cUpdatedBuffer[link[0][1]->maxCX][cj] = true;
                        break;
                }
                if (cj != minCY)
                    if (*cell(minX, cMinY[cj]) != *cellBuffer(minX, cMinY[cj])){
                        if (link[0][1] == NULL)
                            parent->getRegion(regionX - 1, regionY);
                        link[0][1]->cUpdatedBuffer[link[0][1]->maxCX][cj - 1] = true;
                    }
                if (cj != maxCY)
                    if (*cell(minX, cMaxY[cj]) != *cellBuffer(minX, cMaxY[cj])){
                        if (link[0][1] == NULL)
                            parent->getRegion(regionX - 1, regionY);
                        link[0][1]->cUpdatedBuffer[link[0][1]->maxCX][cj + 1] = true;
                    }
            }
            if (cUpdated[maxCX][cj]){
                for (j = cMinY[cj]; j <= cMaxY[cj]; j++)
                    if (*cell(maxX, j) != *cellBuffer(maxX, j)){
                        if (link[2][1] == NULL)
                            parent->getRegion(regionX + 1, regionY);
                        link[2][1]->cUpdatedBuffer[link[2][1]->minCX][cj] = true;
                        break;
                    }
                if (cj != minCY)
                    if (*cell(maxX, cMinY[cj]) != *cellBuffer(maxX, cMinY[cj])){
                        if (link[2][1] == NULL)
                            parent->getRegion(regionX + 1, regionY);
                        link[2][1]->cUpdatedBuffer[link[2][1]->minCX][cj - 1] = true;
                    }
                if (cj != maxCY)
                    if (*cell(maxX, cMaxY[cj]) != *cellBuffer(maxX, cMaxY[cj])){
                        if (link[2][1] == NULL)
                            parent->getRegion(regionX + 1, regionY);
                        link[2][1]->cUpdatedBuffer[link[2][1]->minCX][cj + 1] = true;
                    }
            }
        }
        for (ci = minCX; ci <= maxCX; ci++){
            if (cUpdated[ci][minCY]){
                for (i = cMinX[ci]; i <= cMaxX[ci]; i++)
                    if (*cell(i, minY) != *cellBuffer(i, minY)){
                        if (link[1][0] == NULL)
                            parent->getRegion(regionX, regionY - 1);
                        link[1][0]->cUpdatedBuffer[ci][link[1][0]->maxCY] = true;
                        break;
                    }
                if (ci != minCX)
                    if (*cell(cMinX[ci], minY) != *cellBuffer(cMinX[ci], minY)){
                        if (link[1][0] == NULL)
                            parent->getRegion(regionX, regionY - 1);
                        link[1][0]->cUpdatedBuffer[ci - 1][link[1][0]->maxCY] = true;
                    }
                if (ci != maxCX)
                    if (*cell(cMaxX[ci], minY) != *cellBuffer(cMaxX[ci], minY)){
                        if (link[1][0] == NULL)
                            parent->getRegion(regionX, regionY - 1);
                        link[1][0]->cUpdatedBuffer[ci + 1][link[1][0]->maxCY] = true;
                    }
            }
            if (cUpdated[ci][maxCY]){
                for (i = cMinX[ci]; i <= cMaxX[ci]; i++)
                    if (*cell(i, maxY) != *cellBuffer(i, maxY)){
                        if (link[1][2] == NULL)
                            parent->getRegion(regionX, regionY + 1);
                        link[1][2]->cUpdatedBuffer[ci][link[1][2]->minCY] = true;
                        break;
                    }
                if (ci != minCX)
                    if (*cell(cMinX[ci], maxY) != *cellBuffer(cMinX[ci], maxY)){
                        if (link[1][2] == NULL)
                            parent->getRegion(regionX, regionY + 1);
                        link[1][2]->cUpdatedBuffer[ci - 1][link[1][2]->minCY] = true;
                    }
                if (ci != maxCX)
                    if (*cell(cMaxX[ci], maxY) != *cellBuffer(cMaxX[ci], maxY)){
                        if (link[1][2] == NULL)
                            parent->getRegion(regionX, regionY + 1);
                        link[1][2]->cUpdatedBuffer[ci + 1][link[1][2]->minCY] = true;
                    }
            }
        }
        if (cUpdated[minCX][minCY] && (*cell(minX, minY) != *cellBuffer(minX, minY))){
            if (link[0][0] == NULL)
                parent->getRegion(regionX - 1, regionY - 1);
            link[0][0]->cUpdatedBuffer[link[0][0]->maxCX][link[0][0]->maxCY] = true;
        }
        if (cUpdated[minCX][maxCY] && (*cell(minX, maxY) != *cellBuffer(minX, maxY))){
            if (link[0][2] == NULL)
                parent->getRegion(regionX - 1, regionY + 1);
            link[0][2]->cUpdatedBuffer[link[0][2]->maxCX][link[0][2]->minCY] = true;
        }
        if (cUpdated[maxCX][minCY] && (*cell(maxX, minY) != *cellBuffer(maxX, minY))){
            if (link[2][0] == NULL)
                parent->getRegion(regionX + 1, regionY - 1);
            link[2][0]->cUpdatedBuffer[link[2][0]->minCX][link[2][0]->maxCY] = true;
        }
        if (cUpdated[maxCX][maxCY] && (*cell(maxX, maxY) != *cellBuffer(maxX, maxY))){
            if (link[2][2] == NULL)
                parent->getRegion(regionX + 1, regionY + 1);
            link[2][2]->cUpdatedBuffer[link[2][2]->minCX][link[2][2]->minCY] = true;
        }
    }

    // copy updated cells to neighbours
    if (link[0][1] != NULL){
        i = link[0][1]->maxX + 1;
        for (cj = minCY; cj <= maxCY; cj++)
            if (cUpdated[minCX][cj] || (flag[minCX][cj] == 0))
                for (j = cMinY[cj]; j <= cMaxY[cj]; j++)
                    *(link[0][1]->cell(i, j)) = *cell(minX, j);
    }
    if (link[2][1] != NULL){
        i = link[2][1]->minX - 1;
        for (cj = minCY; cj <= maxCY; cj++)
            if (cUpdated[maxCX][cj] || (flag[maxCX][cj] == 0))
                for (j = cMinY[cj]; j <= cMaxY[cj]; j++)
                    *(link[2][1]->cell(i, j)) = *cell(maxX, j);
    }
    if (link[1][0] != NULL){
        j = link[1][0]->maxY + 1;
        for (ci = minCX; ci <= maxCX; ci++)
            if (cUpdated[ci][minCY] || (flag[ci][minCY] == 0))
                for (i = cMinX[ci]; i <= cMaxX[ci]; i++)
                    *(link[1][0]->cell(i, j)) = *cell(i, minY);
    }
    if (link[1][2] != NULL){
        j = link[1][2]->minY - 1;
        for (ci = minCX; ci <= maxCX; ci++)
            if (cUpdated[ci][maxCY] || (flag[ci][maxCY] == 0))
                for (i = cMinX[ci]; i <= cMaxX[ci]; i++)
                    *(link[1][2]->cell(i, j)) = *cell(i, maxY);
    }
    if ((link[0][0] != NULL) && (cUpdated[minCX][minCY] || flag[minCX][minCY] == 0))
        *(link[0][0]->cell(link[0][0]->maxX + 1, link[0][0]->maxY + 1)) = *cell(minX, minY);
    if ((link[0][2] != NULL) && (cUpdated[minCX][maxCY] || flag[minCX][maxCY] == 0))
        *(link[0][2]->cell(link[0][2]->maxX + 1, link[0][2]->minY - 1)) = *cell(minX, maxY);
    if ((link[2][0] != NULL) && (cUpdated[maxCX][minCY] || flag[maxCX][minCY] == 0))
        *(link[2][0]->cell(link[2][0]->minX - 1, link[2][0]->maxY + 1)) = *cell(maxX, minY);
    if ((link[2][2] != NULL) && (cUpdated[maxCX][maxCY] || flag[maxCX][maxCY] == 0))
        *(link[2][2]->cell(link[2][2]->minX - 1, link[2][2]->minY - 1)) = *cell(maxX, maxY);
}

template <typename ValType> void Region<ValType>::evolveType2(){
    int i, j, di, dj;
    RuleType2<ValType> *tRule = (RuleType2<ValType>*)(rule);
    int r = tRule->radius(), d = r * 2 + 1;
    ValType *param = new ValType[d * d];
    for (i = minX; i <= maxX; i++)
        for (j = minY; j <= maxY; j++){
            for (di = -r; di <= r; di++)
                for (dj = -r; dj <= r; dj++)
                    param[(di + r) * d + (dj + r)] = findCell(di, dj);
            *cell(i, j) = tRule->calc(param, (regionX * REGION_SCALE_FULL) + i, (regionY * REGION_SCALE_FULL) + j, currentTime);
        }
    delete [] param;
}

template <typename ValType> int Region<ValType>::putCell(int x, int y, ValType state){
    int cx = x >> CHUNK_SCALE, cy = y >> CHUNK_SCALE, t = 0;
    if (*cell(x, y) != state){
        stat[(unsigned char)(*cell(x, y))]--;
        if (*cell(x, y) != 0)
            t -= 1;
        stat[(unsigned char)(state)]++;
        if (state != 0)
            t += 1;
        *cell(x, y) = state;
        cUpdated[cx][cy] = true;
        cUpdatedBuffer[cx][cy] = true;
    }
    return t;
}

template <typename ValType> void Region<ValType>::renderInitialize(int x1, int y1, int x2, int y2){
    int ci, cj;
    ri = 0;
    renderCount = 0;
    for (ci = minCX; ci <= maxCX; ci++)
        if ((cMinX[ci] <= x2) && (cMaxX[ci] >= x1))
            for (cj = minCY; cj <= maxCY; cj++)
                if ((cMinY[cj] <= y2) && (cMaxY[cj] >= y1))
                    if ((rule->period() == 0) || (rule->needConversion()) || cUpdatedRec[ci][cj]){
                        renderList[renderCount][0] = max(x1, cMinX[ci]);
                        renderList[renderCount][1] = max(y1, cMinY[cj]);
                        renderList[renderCount][2] = min(x2, cMaxX[ci]);
                        renderList[renderCount][3] = min(y2, cMaxY[cj]);
                        renderCount++;
                    }
    clearRender();
}

template <typename ValType> bool Region<ValType>::getRenderRegion(int &x1, int &y1, int &x2, int &y2){
    if (ri == renderCount)
        return false;
    x1 = renderList[ri][0];
    y1 = renderList[ri][1];
    x2 = renderList[ri][2];
    y2 = renderList[ri][3];
    ri++;
    return true;
}

template <typename ValType> void Region<ValType>::setRule(Rule<ValType> *_rule){
    int i, j;
    rule = _rule;
    clearFlag();
    for (i = minX; i <= maxX; i++)
        for (j = minY; j <= maxY; j++)
            if ((unsigned char)(*cell(i, j)) >= rule->stateCount()){
                stat[(unsigned char)(*cell(i, j))]--;
                *cell(i, j) = rule->stateCount() - 1;
                stat[rule->stateCount() - 1]++;
            }
}

template <typename ValType> int Region<ValType>::randomize(double *pos){
    int i, j, k;
    double t;
    clearFlag();
    for (i = 0; i < 256; i++)
        stat[i] = 0;
    for (i = minX; i <= maxX; i++)
        for (j = minY; j <= maxY; j++){
            t = (double(rand()) + 0.5) / (double(RAND_MAX) + 1) * 100;
            if (t <= pos[0])
                k = 0;
            else for (k = 1; k < rule->stateCount() - 1; k++)
                if ((t > pos[k - 1]) && (t <= pos[k]))
                    break;
            *cell(i, j) = k;
            stat[k]++;
        }
    return population();
}

template <typename ValType> void Region<ValType>::putCellArray(ValType *p_data){
    int i, j;
    clearFlag();
    for (i = 0; i < 256; i++)
        stat[i] = 0;
    for (i = minX; i < maxX; i++)
        for (j = minY; j < maxY; j++){
            *cell(i, j) = p_data[(i << REGION_SCALE) | j];
            stat[(unsigned char)(*cell(i, j))]++;
        }
    for (i = 0; i < 3; i++)
        for (j = 0; j < 3; j++)
            if (((i != 1) || (j != 1)) && (link[i][j] == NULL))
                parent->getRegion(regionX + i - 1, regionY + j - 1);
    for (j = minY; j <= maxY; j++){
        *(link[0][1]->cell(link[0][1]->maxX + 1, j)) = *cell(minX, j);
        *(link[2][1]->cell(link[2][1]->minX - 1, j)) = *cell(maxX, j);
    }
    for (i = minX; i <= maxX; i++){
        *(link[1][0]->cell(i, link[1][0]->maxY + 1)) = *cell(i, minY);
        *(link[1][2]->cell(i, link[1][2]->minY - 1)) = *cell(i, maxY);
    }
    *(link[0][0]->cell(link[0][0]->maxX + 1, link[0][0]->maxY + 1)) = *cell(minX, minY);
    *(link[0][2]->cell(link[0][2]->maxX + 1, link[0][2]->minY - 1)) = *cell(minX, maxY);
    *(link[2][0]->cell(link[2][0]->minX - 1, link[2][0]->maxY + 1)) = *cell(maxX, minY);
    *(link[2][2]->cell(link[2][2]->minX - 1, link[2][2]->minY - 1)) = *cell(maxX, maxY);
}

}
