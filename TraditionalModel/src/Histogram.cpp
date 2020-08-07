#include "Histogram.h"

Histogram::Histogram(int classNum, int bins, std::vector<Classifier::cluster> cls)
{
    classNum = classNum;
    bins = bins;
    writeHistogramData(cls);
    buildHistogram(bins);
}

Histogram::~Histogram()
{

}

/* Score function taken from:
http://www.gregbugaj.com/?p=730
*/
double Histogram::score(const Histogram& test, const int bins)const
{
    double d = 0, s1 = 0, s2= 0;

    for (int i = 0; i < bins; ++i)
    {
        d += hst[i] + test.hst[i] - std::abs(hst[i] - test.hst[i]);
        s1 += hst[i];
        s2 += test.hst[i];
    }

    return (0.5 * d) / std::fmax(s1, s2);
}

void Histogram::buildHistogram(int bins)
{
    // First we need to determine our bin sizes
    int binrange = 256 / bins;
    hist = new int**[bins];
    for(int i = 0; i < bins; i++)
    {
        hist[i] = new int*[bins];
        for(int j = 0; j < bins; j++)
        {
            hist[i][j] = new int[bins];
            for(int k = 0; k < bins; k++)
                hist[i][j][k] = 0;
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
        hist[p1][p2][p3]++;
        ++it;
    }
        for(int i = 0; i < bins; i++)
        {
            for(int j = 0; j < bins; j++)
            {
                for(int k = 0; k < bins; k++)
                {
                    hst.push_back(hist[i][j][k]);
                }
            }

        }

    return;
}

void Histogram::writeHistogramData(std::vector<Classifier::cluster> cls)
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

void Histogram::printHistogram()
{

    auto it = histData.begin();
    while(it != histData.end())
    {
        std::cout << "(" << it->rgb.r << ", " << it->rgb.g << ", " << it->rgb.b << "): "
        << it->occ << "\n";
        ++it;
    }

    for(int i = 0; i < 64; i++)
        std::cout << hst[i] << "\n";
}
