#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include "BitMapImage.h"
#include <cmath>
#include <vector>

class Histogram
{
    public:
        Histogram(int);
        virtual ~Histogram();

         struct histInfo{
            int occ;
            BitMapImage::RBGColour rgb;
        };

        void writeHistogram(BitMapImage*, int threshold = 5);
        void printHistogram();
        int classNum;
        int getHistSize();
        Histogram::histInfo* getHist();


    private:
        Histogram::histInfo* hist;

        int MAX_COLOUR_COUNT;
        BitMapImage::RBGColour temp_colour = {-1, -1, -1};




};
bool operator==(const BitMapImage::RBGColour& lhs, const BitMapImage::RBGColour& rhs);
bool operator!=(const BitMapImage::RBGColour& lhs, const BitMapImage::RBGColour& rhs);
bool approx(const BitMapImage::RBGColour& lhs, const BitMapImage::RBGColour& rhs, int);

#endif // HISTOGRAM_H

