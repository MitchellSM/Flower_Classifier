#ifndef BITMAPIMAGE_H
#define BITMAPIMAGE_H

#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include "patch.h"
#include <float.h>

class BitMapImage
{
    public:
        // Constructor for creating a BMP object with just a file path/name
        BitMapImage(std::string);

        // Constructor for creating a BMP object with a file path and class
        BitMapImage(std::string, int);

        virtual ~BitMapImage();

        struct RBGColour{
            int r;
            int b;
            int g;
        };


        bool processImage();
        std::string getImageValue(int, int);
        std::string getNormalizedValueStr(int, int);
        BitMapImage::RBGColour getNormalizedValue(int, int);
        int getHeight();
        int getWidth();

    private:
        std::string filename;
        std::string filepath = "images\\";
        std::string fileext = ".bmp";
        int classification;
        int bitdepth;
        int height;
        int width;

        bool setImgHead();
        bool setRBGColours();
        float e = FLT_MIN;
        RBGColour** rbgcolours;    // Ptr to colour array

};

#endif // BITMAPIMAGE_H
