/**
@file plant_classifier.cpp
@brief This program classifies 10 categories of flowers from 24-bit BMP files
@author Mitchell Sulz-Martin
@version v0.1
@date 2019-03-25
**/

#include <iostream>
#include "BitMapImage.h"
#include "Classifier.h"
#include "Histogram.h"
#include <vector>
#include <algorithm>
#include <thread>

int classes [10][15] =
{
    {1, 2, 4, 5, 11, 18, 20, 34, 65, 116, 119, 128, 137, 181, 207}, // 0 Phlox -> Redish Pink/Pink
    {6, 16, 42, 45, 50, 62, 74, 81, 92, 95, 101, 105, 157, 159, 163}, //1 Rose -> Red
    {3, 21, 27, 39, 61, 87, 94, 102, 106, 113, 122, 127, 132, 139, 145}, //2 Calendula -> Orange
    {31, 35, 44, 48, 51, 55, 68, 76, 80, 83, 100, 108, 110, 126, 150}, //3 iris -> Purple/Blue & Yellow
    {22, 24, 46, 52, 44, 82, 88, 90, 103, 112, 123, 134, 140, 142, 148}, //4 leucanthemum maximum -> White
    {25, 28, 33, 37, 57, 66, 75, 86, 93, 96, 98, 104, 111, 115, 121}, //5 bellflower -> Light Purple
    {7, 19, 26, 29, 30, 32, 56, 59, 67, 69, 77, 78, 89, 118, 130}, //6 viola -> white w/ yellow
    {14, 15, 23, 43, 47, 53, 63, 72, 91, 146, 153, 167, 72, 43, 146}, //7 Goldquelle -> Yellow
    {36, 40, 49, 58, 60, 64, 79, 85, 97, 99, 107, 109, 117, 120, 125}, //8 peony -> White & Purple
    {39, 42, 55, 72, 74, 85, 115, 132, 142, 150, 156, 163, 169, 170, 172} // 9aquilegia -> Orange
};

int testSet [63] =  {
9, 10, 8, 12, 13, 17,                                   // 0
177, 188, 193, 201, 210,                                 // 1   6
151, 164, 166, 179,                                      // 2
156, 160, 173, 185, 189, 196, 202,                       // 3
152, 178, 191, 200, 205, 208,                            // 4
124, 135, 143, 144, 154, 170, 176, 182, 198, 204,        // 5
138, 161, 184, 187, 197, 199, 206, 209,                  // 6
180, 190, 192,                                           // 7
129, 133, 136, 147, 158, 165, 172, 175, 183, 195, 203,   // 8
175, 187, 195                                            // 9
};
std::string classInfo [10] =
{
    "Phlox -> Red/Pink", "Rose -> Red", "Calendula -> Orange", "iris -> Purple/Blue & Yellow",
    "leucanthemum maximum -> White", "bellflower -> Light Purple", "viola -> white w/ yellow",
    "Goldquelle -> Yellow", "peony -> White & Purple", "aquilegia -> Orange"
};
// s -> start
// e -> end
// c -> class number
double test(int s, int e, int c, std::vector<Histogram*> hsts)
{
    Histogram* hst;
    int corr = 0, incorr = 0;
    float acc = 0.0;
    double size = e - s;
    for(int i = s; i < e; i++)
    {
        int imgnum = testSet[i];
        std::string imageNumber = patch::to_string(imgnum);
        BitMapImage* imgs = new BitMapImage(imageNumber);
        Classifier* cls = new Classifier(25);
        if(imgs->processImage())
        {
            cls->clusterData(imgs);
            hst = new Histogram(i, 6, cls->getClusters());
        }
        std::vector<double> scores;
        std::cout << "Predictions for image #" << imgnum << "\n";
        for(int j = 0; j < 10; j++)
        {
            scores.push_back(hsts[j]->score(*hst, 216));

        }
        double m = DBL_MIN;
        int p = 0;
        for(int j = 0; j < 10; j++)
        {
            if(scores[j] > m)
            {
                m = scores[j];
                p = j;
            }

        }
        std::cout << "Guess: "<< p << " " << classInfo[p] << " " << m << ".\n";
        if(p == c)
        {
            std::cout << "\tCorrect.\n";
            corr++;
        }
        else
        {
            incorr++;
            std::cout << "\tIncorrect.\n";
        }
        std::cout << "\n\n";
    }
    acc = corr/size;
    double accStr = acc*100;
    std::cout << "RESULTS for species #" << c << "\n" << "# of correct predictions: " << corr
    << "\n# of incorrect predictions: " << incorr << "\nTotal accuracy: " << accStr << "%.\n";
    std::cout << "-----------------------------------------\n\n\n";
    return acc;
}



int main()
{
    std::vector<Classifier*> clss;
    std::vector<Histogram*> hsts;


    for(int i = 0; i < 10; i++)
    {
        clss.push_back(new Classifier(25));
        for(int j = 0; j < 15; j++)
        {
            int imgnum = classes[i][j];
            std::string imageNumber = patch::to_string(imgnum);
            BitMapImage* imgs = new BitMapImage(imageNumber);
            if(imgs->processImage())
                clss[i]->clusterData(imgs);
        }
        hsts.push_back(new Histogram(i, 6, clss[i]->getClusters()));

    }
    test(0, 5, 0, hsts);
    test(6, 11, 1, hsts);
    test(11, 15, 2, hsts);
    test(15, 23, 3, hsts);
    test(23, 29, 4, hsts);
    test(29, 39, 5, hsts);
    test(39, 47, 6, hsts);
    test(47, 50, 7, hsts);
    test(50, 61, 8, hsts);
    test(61, 64, 9, hsts);



    return 0;
}
