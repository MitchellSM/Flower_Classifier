#include "Classifier.h"

Classifier::Classifier(int numofclusters, int n)
{
    srand (time(NULL));
    k = numofclusters;
    classNum = n;

    //ctor
}

Classifier::~Classifier()
{

    //dtor
}

void Classifier::classify(Histogram& hst, bool print)
{
    initClusters(hst);
    clusterize(hst, hst.getHistSize());
    while(recluster()) {}
    writeHistogramData(clusterSet);
    buildHistogram(4);
    if(false)
    {
        std::cout << "Top Values for class #" << classNum << ":\n\n";
        int* topAtt = getTop(k);
        for(int i = 0; i < k; i++)
        {
            std::cout << i << ": Cluster #" << topAtt[i] << ": (R, G, B): ("
            << clusterSet[topAtt[i]].clusterAvg.r << ", "
            << clusterSet[topAtt[i]].clusterAvg.g << ", "
            << clusterSet[topAtt[i]].clusterAvg.b << ") "
            << clusterSet[topAtt[i]].clusterSize  << "\n";
        }
    }
}

/* Score function taken from:
http://www.gregbugaj.com/?p=730
*/
double Classifier::score(const Classifier& test, const int bins)const
{
    double d = 0, s1 = 0, s2= 0;

    for (int i = 0; i < 64; ++i)
    {
        d += histogram[i] + test.histogram[i] - std::abs(histogram[i] - test.histogram[i]);
        s1 += histogram[i];
        s2 += test.histogram[i];
    }

    return (0.5 * d) / std::fmax(s1, s2);
}

double Classifier::compare(Classifier& testCls)
{
    int minVal = INT_MAX;
    double clusterDiff = 0;
    auto testI = testCls.clusterSet.begin();
    while(testI != testCls.clusterSet.end())
    {
        std::vector<double> distances;
        minVal = INT_MAX;
        for(auto j = clusterSet.begin(); j != clusterSet.end(); ++j)
        {
            distances.push_back(pow(euclidDistance(testI->clusterAvg, j->clusterAvg), 2));
        }
        for(auto j2 = distances.begin(); j2 != distances.end(); ++j2)
        {
            minVal = (minVal > *j2)? *j2 : minVal;
        }
        //std::cout << "Minimum distance: " << minVal << "\n";
        clusterDiff += minVal;
        ++testI;
    }
    clusterDiff /= k;
    return clusterDiff;
}

bool Classifier::histogramKmeans(Histogram& hst)
{

    initClusters(hst);
    clusterize(hst, hst.getHistSize());
    int moves = recluster();
    while(moves > 0)
    {
        moves = recluster();
    }

    printCluster();

    std::cout << "Top Values:\n\n";
    int* topAtt = getTop(15);
    for(int i = 0; i < 15; i++)
    {
        std::cout << i << ": Cluster #" << topAtt[i] << ": (R, G, B): ("
        << clusterSet[topAtt[i]].clusterAvg.r << ", "
        << clusterSet[topAtt[i]].clusterAvg.g << ", "
        << clusterSet[topAtt[i]].clusterAvg.b << ")\n";
    }


    return true;
}

int* Classifier::highOccList(Histogram hst)
{
    Histogram::histInfo* data = hst.getHist();
    int* highestOcc = new int[k];
    int maxVal = INT_MIN;
    bool found = false;

    for(int i = 0; i < k; i++)
    {
        maxVal = INT_MIN;
        for(int j = 0; j < hst.getHistSize(); j++)
        {
            if(data[j].occ > maxVal)
            {
                for(int i2 = 0; i2 < k; i2++)
                {
                    if(highestOcc[i2] == j)
                        found = true;
                }
                if(!found)
                {
                    maxVal = data[j].occ;
                    highestOcc[i] = j;
                }
                else
                {
                    found = false;
                }
            }
        }
        found = false;
    }
    return highestOcc;
}

int* Classifier::getTop(int size)
{
    int* topPos = new int[size];
    int maxVal = INT_MIN;
    bool found = false;

    for(int i = 0; i < size; i++)
    {
        maxVal = INT_MIN;
        for(auto j = clusterSet.begin(); j != clusterSet.end(); ++j)
        {
            if(j->clusterSize > maxVal)
            {
                for(int i2 = 0; i2 < size; i2++)
                {
                    if(topPos[i2] == j->clusterNum)
                        found = true;
                }
                if(!found)
                {
                    maxVal = j->clusterSize;
                    topPos[i] = j->clusterNum;
                }
                else
                {
                    found = false;
                }
            }
        }
        found = false;
    }

    return topPos;
}

bool Classifier::recluster()
{
    bool change = false;

        //Grab cluster
        auto i = clusterSet.begin();
        while(i != clusterSet.end())
        {
            //Grab cluster values
            auto j = i->vals.begin();

            while(j != i->vals.end())
            {
                int bestFit = 0;

                int* distances = new int[k];

                for(int i = 0; i < k; i++)
                {
                    distances[i] = pow(euclidDistance(j->rgb, clusterSet[i].clusterAvg), 2);
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
                        j++;
                    }

                }
                else
                {
                    j++;
                }
            }
            for(auto i2 = clusterSet.begin(); i2 != clusterSet.end(); ++i2)
                i2->clusterAvg = clustAvg(*i2);
            ++i;
        }

    return change;
}

void Classifier::clusterize(Histogram hst, int size)
{
    Histogram::histInfo* data = hst.getHist();

    bool found = false;

    for(int i = 0; i < size;)
    {
        int bestFit;
        int distancePrev = INT_MAX;

        auto j = clusterSet.begin();
        while(j != clusterSet.end())
        {
            int distanceSqr = pow(euclidDistance(data[i].rgb, j->clusterAvg), 2);
            if(distancePrev > distanceSqr)
                {
                    bestFit = j->clusterNum;
                    distancePrev = distanceSqr;
                }
            ++j;
        }
        for(auto j2 = clusterSet[bestFit].vals.begin(); j2 != clusterSet[bestFit].vals.end(); ++j2)
        {
            if(j2->rgb == data[i].rgb)
                found = true;
        }

        if(!found)
        {
            clusterSet[bestFit].vals.push_back(data[i]);
            clusterSet[bestFit].clusterSize++;
            clusterSet[bestFit].clusterAvg = clustAvg(clusterSet[bestFit]);
        }
        found = false;
        ++i;

    }

    for(auto i2 = clusterSet.begin(); i2 != clusterSet.end(); ++i2)
        i2->clusterAvg = clustAvg(*i2);

}

void Classifier::initClusters(Histogram hst)
{
    int* rndPos = new int[k];
    rndPos = highOccList(hst); // randomSelect(hst.getHistSize());
    Histogram::histInfo* data = hst.getHist();

    for(int i = 0; i < k; i++)
    {
        clusterSet.push_back(Classifier::cluster()); // Push in a new cluster
        clusterSet[i].vals.push_back( data[rndPos[i]] ); //Push in a random data point
        clusterSet[i].clusterSize = 1;
        clusterSet[i].clusterNum = i;
        clusterSet[i].clusterAvg = clustAvg(clusterSet[i]);
    }
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
            std::cout << "(" << j->rgb.r << ", " << j->rgb.g << ", " << j->rgb.b << ") " << j->occ << " "
            << pow(euclidDistance(j->rgb, i->clusterAvg), 2) << "\n";

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
            sumRGB.r += c.vals[i].rgb.r;
            sumRGB.b += c.vals[i].rgb.b;
            sumRGB.g += c.vals[i].rgb.g;
        }
        avgRGB.r = sumRGB.r / clustsize;
        avgRGB.b = sumRGB.b / clustsize;
        avgRGB.g = sumRGB.g / clustsize;
    }

    return avgRGB;
}

int* Classifier::randomSelect(int max_pos)
{
    int* rndList = new int[k];
    int intv = 0;
    int mod = (max_pos) % k;
    for(int i = 0; i < k; i++)
    {
        rndList[i] = rand() % mod + intv;
        intv += mod;
    }
    return rndList;
}

int Classifier::getTotal()
{
    int total = 0;
    for(int i = 0; i < k; i++)
        total += clusterSet[i].clusterSize;
    return total;
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

BitMapImage::RBGColour operator-(const BitMapImage::RBGColour& lhs, const BitMapImage::RBGColour& rhs)
{
    BitMapImage::RBGColour rgb;
    rgb.r = lhs.r - rhs.r;
    rgb.g = lhs.g - rhs.g;
    rgb.b = lhs.b - rhs.b;
    return rgb;

}




void Classifier::buildHistogram(int bins)
{
    // First we need to determine our bin sizes
    int binrange = 256 / bins;
    hst = new int**[bins];
    for(int i = 0; i < bins; i++)
    {
        hst[i] = new int*[bins];
        for(int j = 0; j < bins; j++)
        {
            hst[i][j] = new int[bins];
            for(int k = 0; k < bins; k++)
                hst[i][j][k] = 0;
        }
    }

    auto it = histData.begin();
    while(it != histData.end())
    {
        int p1, p2, p3;
        int r1, r2, r3;
        bool f1, f2, f3;
        f1 = false; f2 = false; f3 = false;
        r1 = 0; r2 = 0; r3 = 0;
        for(int i = 0; i < bins; i++)
        {
            if(f1 && f2 && f3)
                break;

            if(it->rgb.r < r1+binrange && !f1 && it->rgb.r >= r1)
            {
                    p1 = i;
                    f1 = true;
            }
            else
                r1+= binrange;

            if(it->rgb.g < r2+binrange && !f2 && it->rgb.g >= r2)
            {
                    p2 = i;
                    f2 = true;
            }

            else
                r2+= binrange;

            if(it->rgb.b < r3+binrange && !f3 && it->rgb.b >= r3)
            {
                    p3 = i;
                    f3 = true;
            }
            else
                r3+= binrange;
        }
        hst[p1][p2][p3]++;
        ++it;
    }
        for(int i = 0; i < bins; i++)
        {
            for(int j = 0; j < bins; j++)
            {
                for(int k = 0; k < bins; k++)
                {
                    histogram.push_back(hst[i][j][k]);
                }
            }

        }

    return;
}

void Classifier::writeHistogramData(std::vector<Classifier::cluster> cls)
{
    auto i = cls.begin();
    while(i != cls.end())
    {
        Histogram::histInfo hst;
        hst.occ = i->clusterSize;
        hst.rgb = i->clusterAvg;
        histData.push_back(hst);
        ++i;
    }

}
