#ifndef CLASSIFIER_H
#define CLASSIFIER_H
#include "Histogram.h"
#include "BitMapImage.h"
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <iostream>

class Classifier
{
     public:
        Classifier(int, int n = -1);
        virtual ~Classifier();

        int classNum;

        struct cluster{
            std::vector<Histogram::histInfo> vals;
            BitMapImage::RBGColour clusterAvg;
            int clusterSize;
            int clusterNum;
        };


        // Perform a K-Means clustering on a given histogram
        bool histogramKmeans(Histogram&);
        void classify(Histogram& hst, bool print = false);
        int* getTop(int size = 5);
        double compare(Classifier&);

          double score(const Classifier& test, const int bins)const;


         void buildHistogram(int bins);
         void writeHistogramData(std::vector<Classifier::cluster> cls);

    private:
        //Classifier::cluster* clusterSet;
        std::vector<Classifier::cluster> clusterSet;

        int k; // Number of clusters
        BitMapImage::RBGColour temp_colour = {-1, -1, -1};

        // Sum the values in a given channel
        BitMapImage::RBGColour sumChannels(Histogram::histInfo*, int);

        double euclidDistance(const BitMapImage::RBGColour& lhs, const BitMapImage::RBGColour& rhs);
        double magnitude(const BitMapImage::RBGColour& lhs);
        int dotproduct(const BitMapImage::RBGColour& lhs, const BitMapImage::RBGColour& rhs);
        int* randomSelect(int);
        int* highOccList(Histogram hst);

        void initClusters(Histogram);
        void clusterize(Histogram, int);
        bool recluster();

        // Calculate the average RGB channel values
        BitMapImage::RBGColour clustAvg(Classifier::cluster);

        void printCluster();

        int getTotal();



        int*** hst;
        std::vector<int> histogram;
        std::vector <Histogram::histInfo> histData;

};

//bool operator==(const BitMapImage::RBGColour& lhs, const BitMapImage::RBGColour& rhs);
//bool operator!=(const BitMapImage::RBGColour& lhs, const BitMapImage::RBGColour& rhs);
BitMapImage::RBGColour operator-(const BitMapImage::RBGColour& lhs, const BitMapImage::RBGColour& rhs);


#endif // CLASSIFIER_H
