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
9, 10, 8, 12, 13, 17,     // 0 Phlox -> Redish Pink/Pink
177, 188, 193, 201, 210,    //1 Rose -> Red
151, 164, 166, 179,  //2 Calendula -> Orange
156, 160, 173, 185, 189, 196, 202,  //3 iris -> Purple/Blue & Yellow
152, 178, 191, 200, 205, 208,    //4 leucanthemum maximum -> White
124, 135, 143, 144, 154, 170, 176, 182, 198, 204, //5 bellflower -> Light Purple
138, 161, 184, 187, 197, 199, 206, 209,  //6 viola -> white w/ yellow
180, 190, 192,                             //7 Goldquelle -> Yellow
129, 133, 136, 147, 158, 165, 172, 175, 183, 195, 203,   //8 peony -> White & Purple
175, 187, 195               // 9aquilegia -> Orange
};
std::string classInfo [10] =
{
    "Phlox -> Red/Pink", "Rose -> Red", "Calendula -> Orange", "iris -> Purple/Blue & Yellow",
    "leucanthemum maximum -> White", "bellflower -> Light Purple", "viola -> white w/ yellow",
    "Goldquelle -> Yellow", "peony -> White & Purple", "aquilegia -> Orange"
};

struct est
{
    double distance;
    int pos;
};

bool sortFunc(est i, est j){return i.distance < j.distance;}

void processImage(int i, Histogram* hst, Classifier* cls)
{

    for(int j = 0; j < 15; j++)
    {
        int imgnum = classes[i][j];
        std::string imageNumber = patch::to_string(imgnum);
        BitMapImage* imgs = new BitMapImage(imageNumber);
        if(imgs->processImage())
        {
            hst->writeHistogram(imgs, 5);
        }
        delete imgs;
        imgs = NULL;
    }
    cls->classify(*hst);
}

// s -> start
// e -> end
// c -> class number
double test(int s, int e, int c, std::vector<Classifier*> clss)
{
    Histogram* testHst;
    int corr = 0, incorr = 0;
    float acc = 0.0;
    double size = e - s;
    for(int i2 = s; i2 < e; i2++)
    {
        int imgnum = testSet[i2];
        std::string imageNumber = patch::to_string(imgnum);
       //std::cout << "Testing model on image: " << imageNumber << "\n";
        BitMapImage* testImg = new BitMapImage(imageNumber);
        Histogram* testHst = new Histogram(0);
        Classifier* testCls = new Classifier(25);

        if(testImg->processImage())
            testHst->writeHistogram(testImg);

        testCls->classify(*testHst);
        //std::vector<est> estimations;


         std::vector<double> scores;
        std::cout << "Predictions for image #" << imgnum << "\n";
        for(int j = 0; j < 10; j++)
        {
            scores.push_back(clss[j]->score(*testCls, 64));

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
    std::thread imgThreads[10];
    std::vector<Histogram*> hsts;
    std::vector<Classifier*> clss;

    for(int i = 0; i < 10; i++)
    {
        hsts.push_back(new Histogram(i));
        clss.push_back(new Classifier(25, i));
    }
    std::cout << "Training model...\n";
    for(int i = 0; i < 10; i++)
    {

        imgThreads[i] = std::thread(processImage, i, hsts[i], clss[i]);
    }

    for(int i2 = 0; i2 < 10; i2++)
    {
        imgThreads[i2].join();
    }

    //Testing Model
    std::cout << "Testing Model\n";
    test(0, 5, 0, clss);
    test(6, 11, 1, clss);
    test(11, 15, 2, clss);
    test(15, 23, 3, clss);
    test(23, 29, 4, clss);
    test(29, 39, 5, clss);
    test(39, 47, 6, clss);
    test(47, 50, 7, clss);
    test(50, 61, 8, clss);
    test(61, 64, 9, clss);

 /*   std::cout << "\nImages clustered...\n Testing Model...\n";
    for(int i2 = 0; i2 < 63; i2++)
    {
        int imgnum = testSet[i2];

        std::string imageNumber = patch::to_string(imgnum);
       //std::cout << "Testing model on image: " << imageNumber << "\n";
        BitMapImage* testImg = new BitMapImage(imageNumber);
        Histogram* testHst = new Histogram(0);
        Classifier* testCls = new Classifier(25);

        if(testImg->processImage())
            testHst->writeHistogram(testImg);

        testCls->classify(*testHst);
        //std::vector<est> estimations;


         std::vector<double> scores;
        std::cout << "Predictions for image #" << imgnum << "\n";
        for(int j = 0; j < 10; j++)
        {
            scores.push_back(clss[j]->score(*testCls, 64));

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

        /*for(int j2 = 0; j2 < 10; j2++)
        {
            estimations.push_back(est());
            estimations.back().distance = clss[j2]->compare(*testCls);
            estimations.back().pos = j2;
        }

        std::sort(estimations.begin(), estimations.end(), sortFunc);
        for(int j3 = 0; j3 < 1; j3++)
        {
            if(j3 == 0)
                std::cout << "Guess: " << classInfo[estimations[j3].pos] << ".\n";
            std::cout << "Distance from cluster #" << estimations[j3].pos << ": " << estimations[j3].distance << ".\n";
        }*/
        std::cout << "\n\n";
    /*
        delete testImg;
        delete testHst;
        delete testCls;
        testImg = NULL;
        testHst = NULL;
        testCls = NULL;*/
    //}

    return 0;
}
