#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <iostream>
#include <string>
#include <stdlib.h>
//#include <stdio.h>

//#include <fstream>
#include "BitMapImage.h"
#include <Classifier.h>
#include <algorithm>
#include <vector>

class Histogram
{
    public:
        Histogram(int, int, std::vector<Classifier::cluster> cls);
        virtual ~Histogram();

         struct histInfo{
            int occ;
            BitMapImage::RBGColour rgb;
        };

        void printHistogram();
        int classNum;

        Histogram::histInfo* getHist();
        double score(const Histogram& test, const int bins)const;

    private:
        std::vector <histInfo> histData;
        int*** hist;
        std::vector<int> hst;

        void writeHistogramData(std::vector<Classifier::cluster>);
        void buildHistogram(int);
        int bins;



};
#endif // HISTOGRAM_H

