#include "Histogram.h"

Histogram::Histogram(int classNum)
{
    classNum = classNum;
    MAX_COLOUR_COUNT = 128*128;

    hist = new Histogram::histInfo[MAX_COLOUR_COUNT];
    for(int i = 0; i < MAX_COLOUR_COUNT; i++)
    {
        hist[i].occ = -1;
        hist[i].rgb = {-1, -1, -1};
    }
}

Histogram::~Histogram()
{

}

void Histogram::writeHistogram(BitMapImage* bmp, int threshold)
{
    int temp_pos = 0;
    int width = bmp->getWidth();
    int height = bmp->getHeight();

    for(int x = 0; x < height; x++)
    { // Get x position from image
        for(int y = 0; y < width; y++)
        { // Get y position from image
            for(int i = 0; i < MAX_COLOUR_COUNT; i++)
            { // Find Colour in histogram
                BitMapImage::RBGColour rgb = bmp->getNormalizedValue(x, y);

                if(hist[i].rgb == temp_colour)
                { // No matching colour was found, so add to histogram
                    hist[i].rgb = rgb;
                    hist[i].occ = 1;
                    break;
                }

                if(approx(hist[i].rgb, rgb, threshold))
                { // Found a match, increase occ count
                    hist[i].occ++;
                    break;
                }
            }
        }
    }
}

int Histogram::getHistSize()
{
    int size = 0;
    int i = 0;
    while(hist[i].rgb != temp_colour)
    {
        size++;
        i++;
    }
    return size;
}

void Histogram::printHistogram()
{
    for(int i = 0; i < MAX_COLOUR_COUNT; i++)
    {
        if(hist[i].rgb != temp_colour)
        {
            if(hist[i].occ > 1)
                std::cout << "(b, g, r) B: " << hist[i].rgb.b
                    << " G: " << hist[i].rgb.g << " R: " << hist[i].rgb.r << " \t"
                    << " O: " << hist[i].occ << "\n";
        }
    }
}

Histogram::histInfo* Histogram::getHist()
{
    return hist;
}

bool approx(const BitMapImage::RBGColour& lhs, const BitMapImage::RBGColour& rhs, int threshold)
{
    if( (abs(lhs.b - rhs.b) <= threshold) &&
        (abs(lhs.g - rhs.g) <= threshold) &&
        (abs(lhs.r - rhs.r) <= threshold))
            return true;
    return false;

}

bool operator==(const BitMapImage::RBGColour& lhs, const BitMapImage::RBGColour& rhs)
{
    return ((lhs.b == rhs.b) && (lhs.g == rhs.g) && (lhs.r == rhs.r));
}

bool operator!=(const BitMapImage::RBGColour& lhs, const BitMapImage::RBGColour& rhs)
{
    return ((lhs.b != rhs.b) || (lhs.g != rhs.g) || (lhs.r != rhs.r));
}


