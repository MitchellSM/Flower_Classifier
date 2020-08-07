#ifndef CLASSIFIER_H
#define CLASSIFIER_H
#include "BitMapImage.h"
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <iostream>
#include <cmath>

class Classifier
{
     public:
        Classifier(int, int n = -1);
        virtual ~Classifier();

        // Structure to hold cluster information
        struct cluster{
            std::vector<BitMapImage::RBGColour> vals;
            BitMapImage::RBGColour clusterAvg;
            int clusterSize;
            int clusterNum;
        };

        // Class number
        int classNum;

        // Method to build clusters from a given image
        void clusterData(BitMapImage* bmp);

        // Print the cluster set -- for debugging
        void printCluster();
        // Print the total number of elements amongst the cluster set -- for debugging
        int getTotal();
        // Provides access to the cluster set
        std::vector<Classifier::cluster> getClusters();

    private:

        // Structure to store a point
        struct point
        {
            int x;
            int y;
        };

        // Container to hold the clusters
        std::vector<Classifier::cluster> clusterSet;

        // Number of clusters
        int k;



        // Initialize a set of clusters
        void init(BitMapImage* bmp);
        // Iterate over the cluster set until it converges
        bool clusterize();
        // Calculate the average RGB channel values
        BitMapImage::RBGColour clustAvg(Classifier::cluster);

        // Calculate the euclidean distance between two 3D vectors
        double euclidDistance(const BitMapImage::RBGColour& lhs, const BitMapImage::RBGColour& rhs);
        // Calculate the magnitude of a 3D vector
        double magnitude(const BitMapImage::RBGColour& lhs);
        // Calculate the dot product of two 3D vectors
        int dotproduct(const BitMapImage::RBGColour& lhs, const BitMapImage::RBGColour& rhs);
        // Randomly select K points from a grid
        Classifier::point* randomSelect();




};


#endif // CLASSIFIER_H

