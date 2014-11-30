#ifndef SEAMCARVING_H
#define SEAMCARVING_H

#include <vector>
#include <QImage>
#include "energyfunctioni.h"

class SeamCarving
{
public:
    SeamCarving(QImage& image, EnergyFunctionI* en);
    void removeSeamV();
    void removeSeamH();
    void findSeamV();
    void findSeamH();
    void setMask(QImage* I);
    QImage getImage();

    QImage energyDist;

private:
    void saveEnergyDist(long long* M);
    int getEnergy(int x, int y);
    QImage& image;
    std::vector<int> seam;

    QImage* mask;
    EnergyFunctionI* energy;
    int width;
    int height;
};

#endif // SEAMCARVING_H
