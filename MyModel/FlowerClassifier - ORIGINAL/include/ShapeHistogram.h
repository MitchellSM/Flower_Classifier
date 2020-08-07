#ifndef SHAPEHISTOGRAM_H
#define SHAPEHISTOGRAM_H
//
//#include <iostream>
//#include <string>
//#include <stdio.h>
//#include <stdlib.h>
//#include <fstream>
//#include "BitMapImage.h"
#include "Histogram.h"

class ShapeHistogram : public Histogram
{
    public:
//        ShapeHistogram();
//        virtual ~ShapeHistogram();

         // Create the colour histograms for the test set
        void createShapeHistograms();

         // Creates the colour histogram for the given image
        void processImage(BitMapImage);

        // Read in a previously created histogram
        void readInHistogram(std::string);

        // Update histogram values for a given class
        void writeOutHistogram(int);
    protected:

    private:
};

#endif // SHAPEHISTOGRAM_H
