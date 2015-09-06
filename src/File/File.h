/**
  * File.h
  * Defines functions for reading and writing cell data files.
  */

#pragma once

#include "lzw.h"
#include "Common.h"
#include "Grid.hpp"
#include "Region.hpp"
#include <fstream>
#include <string>
#include <cassert>

using std::string;
using std::ofstream;
using std::ifstream;

string lzw(Region<unsigned char>* a, int statecount){
    vector<unsigned char> input;
    vector<unsigned short> output;
    string outp;
    outp.clear();
    output.clear();
    input.clear();
    for (int i=0;i<REGION_SIZE;i++)
        for (int j=0;j<REGION_SIZE;j++)
            input.push_back(a->getCell(i, j));
    output = lzw(input,statecount);
    for (int i=0;i<output.size();i++){
        outp.push_back(output[i]/256);
        outp.push_back(output[i]%256);
    }
    return outp;
}


Region<unsigned char>* de_lzw(vector<unsigned short> code, Region<unsigned char>* a, int statecount){
    vector<unsigned char> output;
    output = de_lzw(code, statecount);
    for (int i=0;i<REGION_SIZE;i++)
        for (int j=0;j<REGION_SIZE;j++)
            a->putCell(i, j, output[i*REGION_SIZE+j]);
    return a;
}


string Code(Grid<unsigned char> *grid, string name, int statecount){
    ofstream file;
    file.open(name.c_str());
    // get information from the Grid class
    int minx = grid->minX(), minRX = minx >> REGION_SCALE_FULL;
    int miny = grid->minY(), minRY = miny >> REGION_SCALE_FULL;
    int maxx = grid->maxX(), maxRX = maxx >> REGION_SCALE_FULL;
    int maxy = grid->maxY(), maxRY = maxy >> REGION_SCALE_FULL;
    Region<unsigned char>* ptr;
    string temp;
    // do basic set up coding

    file << maxx - minx + 1 << ' ' << maxy - miny + 1 << ' ' << statecount << '\n';

    for (int i=minRX; i<=maxRX; i++)
        for (int j=minRY; j<=maxRY; j++){
            ptr = grid->findRegion(i,j);
            if (ptr != NULL){
                file << i << ' ' << j;
                temp = lzw(ptr, statecount);
                file << ' ' << temp.size() << '\n' << temp << '\n';
            }
        }
    return name;
}


void decode(Grid<unsigned char> *grid, string name){
    // basic setup steps for the initialization
    ifstream file;
    file.open(name.c_str());
    char ch,nch;
    bool jud = false;
    int statecount, width, height;
    vector<unsigned char> temp;

    file >> width >> height >> statecount;
    grid->setup(width, height);
    // start determining the region's state
    for (int i=0;;i++) {
        int x, y, len;
        // first determine the location for the region, first x
        file >> x >> y >> len;
        file.ignore();
        temp.clear();
        for (int j=0;j<len;j++)
            temp.push_back(file.get());
        //cout << temp << endl;
        //search for such region in the grid and assign its value
        Region<unsigned char>* ptr = grid->getRegion(x, y);
        vector<unsigned short> input;
        for (int i=0;i<len/2;i++){
            unsigned short j = temp[i*2]*256+temp[i*2+1];
            input.push_back(j);
        }
        de_lzw(input, ptr, statecount);
        while (file.peek() == '\n')
            file.ignore();
        // if we have reached the end in the last loop, then break
        if (file.eof())
            break;
    }
}

