#include "Classifier.h"

Classifier::Classifier(int numofclusters, int n)
{
    srand (time(NULL));
    this->k = numofclusters;
    classNum = n;

    //ctor
}

Classifier::~Classifier()
{

    //dtor
}

// Take as input, a set of BitMapImages
void Classifier::clusterData(BitMapImage* bmp)
{
    init(bmp);
    while(clusterize()){}
    return;
}

void Classifier::init(BitMapImage* bmp)
{
    if(clusterSet.empty())
    {
        point* rnd = randomSelect();
        for(int i = 0; i < k; i++)
        {
            BitMapImage::RBGColour rgb = bmp->getNormalizedValue(rnd[i].x, rnd[i].y); //get random point in image
            clusterSet.push_back(Classifier::cluster()); // Push in a new cluster
            clusterSet[i].clusterNum = i;
            clusterSet[i].clusterAvg = rgb;
        }

    }

    for(int i = 0; i < bmp->getHeight(); i++)
    {
        for(int j = 0; j < bmp->getWidth(); j++)
        {
            int bestFit;
            int distancePrev = INT_MAX;
            BitMapImage::RBGColour rgb = bmp->getNormalizedValue(i, j);

            auto j2 = clusterSet.begin();
            while(j2 != clusterSet.end())
            {
                int distanceSqr = pow(euclidDistance(rgb, j2->clusterAvg), 2);
                if(distancePrev > distanceSqr)
                {
                    bestFit = j2->clusterNum;
                    distancePrev = distanceSqr;
                }
                ++j2;
            }
            clusterSet[bestFit].vals.push_back(rgb);
            clusterSet[bestFit].clusterSize++;
            clusterSet[bestFit].clusterAvg = clustAvg(clusterSet[bestFit]);
            ++i;
        }
    }

    for(auto i2 = clusterSet.begin(); i2 != clusterSet.end(); ++i2)
        i2->clusterAvg = clustAvg(*i2);

    return;
}

bool Classifier::clusterize()
{
    bool change = false;

    auto i = clusterSet.begin();
    while(i != clusterSet.end())
    {
        auto j = i->vals.begin();

        while(j != i->vals.end())
        {
            int bestFit = 0;
            int* distances = new int[k];

            for(int i = 0; i < k; i++)
            {
                distances[i] = pow(euclidDistance(*j, clusterSet[i].clusterAvg), 2);
            }

            int mindist = INT_MAX;
            for(int i = 0; i < k; i++)
            {
                if(mindist > distances[i])
                {
                    mindist = distances[i];
                    bestFit = i;
                }
            }
            if(bestFit != i->clusterNum)
            {
                if(i->clusterSize > 1)
                {
                    change = true;
                    clusterSet[bestFit].clusterSize++;
                    i->clusterSize--;
                    clusterSet[bestFit].vals.push_back(*j);
                    j = i->vals.erase(j);
                    i->clusterAvg = clustAvg(*i);
                    clusterSet[bestFit].clusterAvg = clustAvg(clusterSet[bestFit]);
                }
                else
                {
                    ++j;
                }
            }
            else
            {
                ++j;
            }
        }
        for(auto i2 = clusterSet.begin(); i2 != clusterSet.end(); ++i2)
            i2->clusterAvg = clustAvg(*i2);
        ++i;
    }

    return change;
}

Classifier::point* Classifier::randomSelect()
{
    point* rndList = new point[k];
    int intv = 0;
    int mod = 128 / k;
    for(int i = 0; i < k; i++)
    {
        rndList[i].x = rand() % mod + (intv - 1);
        rndList[i].y = rand() % mod + (intv - 1);
        if(rndList[i].x > 127 || rndList[i].x < 0)
            i--;
        else{
            if(rndList[i].y > 127 || rndList[i].y < 0)
                i--;
            else
                intv = intv + mod;
        }
    }

    return rndList;
}

void Classifier::printCluster()
{
    for(auto i = clusterSet.begin(); i != clusterSet.end(); ++i)
    {
         std::cout << "\nCluster " << i->clusterNum << ": "
         << "Average (R,G,B):"
                  << "( " << i->clusterAvg.r
                  << ", " << i->clusterAvg.g
                  << "," << i->clusterAvg.b
                  << ") " << i->clusterSize
                  << "\n";

        for(auto j = i->vals.begin(); j != i->vals.end(); ++j)
            std::cout << "(" << j->r << ", " << j->g << ", " << j->b << ") \n";

    }
}

BitMapImage::RBGColour Classifier::clustAvg(Classifier::cluster c)
{
    BitMapImage::RBGColour avgRGB = {0, 0, 0};
    int clustsize = c.clusterSize;

    if(clustsize != 0)
    {
        BitMapImage::RBGColour sumRGB = {0, 0, 0};
        for(int i = 0; i < clustsize; i++)
        {
            sumRGB.r += c.vals[i].r;
            sumRGB.b += c.vals[i].b;
            sumRGB.g += c.vals[i].g;
        }
        avgRGB.r = sumRGB.r / clustsize;
        avgRGB.b = sumRGB.b / clustsize;
        avgRGB.g = sumRGB.g / clustsize;
    }

    return avgRGB;
}

int Classifier::getTotal()
{
    int total = 0;
    for(int i = 0; i < k; i++)
        total += clusterSet[i].clusterSize;
    return total;
}

std::vector<Classifier::cluster> Classifier::getClusters()
{
    return clusterSet;
}

double Classifier::euclidDistance(const BitMapImage::RBGColour& lhs, const BitMapImage::RBGColour& rhs)
{
    return sqrt(dotproduct((lhs-rhs), (lhs-rhs)));
}

double Classifier::magnitude(const BitMapImage::RBGColour& lhs)
{
    return sqrt((pow(lhs.b, 2) + pow(lhs.g, 2) + pow(lhs.r, 2)));
}

int Classifier::dotproduct(const BitMapImage::RBGColour& lhs, const BitMapImage::RBGColour& rhs)
{
    return ((lhs.b * rhs.b) + (lhs.g * rhs.g) + (lhs.r * rhs.r));
}



