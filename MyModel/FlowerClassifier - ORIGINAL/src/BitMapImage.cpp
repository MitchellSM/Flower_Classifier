#include "BitMapImage.h"

BitMapImage::BitMapImage(std::string imgNum)
{
    filename = filepath + imgNum + fileext;
}

BitMapImage::BitMapImage(std::string imgNum, int classNum)
{
    filename = filepath + imgNum + fileext;
    classification = classNum;
}

BitMapImage::~BitMapImage()
{
    //dtor
    for(int i = 0; i < width; i++)
        delete[] rbgcolours[i];
    delete[] rbgcolours;
}

/// Process image head and RBG values
bool BitMapImage::processImage(){
    return setImgHead() && setRBGColours();
}


/// Extracts header information from BMP image
bool BitMapImage::setImgHead(){
    std::ifstream fIn;
    char * header;

    fIn.open(filename, std::ios::in | std::ios::binary);
    if(fIn.is_open()){
        header = new char[54];
        fIn.read(header, 54);

        height = *(int*)&header[18];
        width = *(int*)&header[22];
        bitdepth = *(int*)&header[28];

        fIn.close();
        delete header;
    }
    else{
        return false;
    }

    return true;
}


/// Extracts RBG colour information from BMP image
bool BitMapImage::setRBGColours(){

    std::ifstream fIn;
    int pos = 0;
    int excessPitch = 0;
    int pitch = 3 * width;
    unsigned char* dataStore = new unsigned char[pitch * height];

    rbgcolours = new RBGColour*[height];
    for(int i = 0; i < height; i++)
        rbgcolours[i] = new RBGColour[width];

    while(double(pitch / 4) != double(pitch) / 4.0)
    {
        // Calculate offset
        pitch++;
        excessPitch++;
    }

    fIn.open(filename, std::ios::in | std::ios::binary);
    if(fIn.is_open())
    {
        fIn.seekg(54, std::ios::beg); // Move past header
        fIn.read((char*) dataStore, pitch*height); // Read in image file

        for(int i = 0; i < height; i++)
        {
            for(int j = 0; j < width; j++)
            {
                RBGColour rbg;
                // Obtain RBG values of current pixel
                rbg.b = dataStore[pos++];
                rbg.g = dataStore[pos++];
                rbg.r = dataStore[pos++];

                // Store RBG content for image
                rbgcolours[i][j] = rbg;
            }
            pos += excessPitch;
        }

        fIn.close();
        delete dataStore;
    }
    else{
        return false;
    }

    return true;
}

/// Method for printing RBG Values to console
std::string BitMapImage::getImageValue(int i, int j)
{
    std::string out = "";
    if(0 <= i && i <= height && 0 <= j && j <= width)
    {
        out = "B: " + patch::to_string(rbgcolours[i][j].b) +
              " G: " + patch::to_string(rbgcolours[i][j].g) +
              " R: " + patch::to_string(rbgcolours[i][j].r) +
              "\n";
        return out;
    }
    else
        return "No information to extract from image.\n";
}

BitMapImage::RBGColour BitMapImage::getNormalizedValue(int i, int j)
{
    return rbgcolours[i][j];
}

std::string BitMapImage::getNormalizedValueStr(int i, int j)
{
    std::string out = "";
    if(0 <= i && i <= height && 0 <= j && j <= width)
    {
        out = "B: " + patch::to_string(rbgcolours[i][j].b) +
              " G: " + patch::to_string(rbgcolours[i][j].g) +
              " R: " + patch::to_string(rbgcolours[i][j].r) +
              "\n";
        return out;
    }
    else
        return "No information to extract from image.\n";
}

/// Returns the Height of the BMP image
int BitMapImage::getHeight()
{
    return height;
}

/// Returns the Width of the BMP image
int BitMapImage::getWidth()
{
    return width;
}





