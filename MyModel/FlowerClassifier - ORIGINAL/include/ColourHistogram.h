#ifndef COLOURHISTOGRAM_H
#define COLOURHISTOGRAM_H
//
//#include <iostream>
//#include <string>
//#include <stdio.h>
//#include <stdlib.h>
//#include <fstream>
//#include "BitMapImage.h"
#include "Histogram.h"

class ColourHistogram : public Histogram
{
    public:
        //ColourHistogram(int, int);
        //virtual ~ColourHistogram();

         // Create the colour histograms for the test set
        void createColourHistograms();

         // Creates the colour histogram for the given image
        void processImage(BitMapImage);

        // Read in a previously created histogram
        void readInHistogram(std::string);

        // Update histogram values for a given class
        void writeOutHistogram(int);


    protected:

    private:
};

#endif // COLOURHISTOGRAM_H
