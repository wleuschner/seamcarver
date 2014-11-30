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

    QImage energyDist;
    void findSeamV();
    QImage getImage();

private:
    void saveEnergyDist(long long* M);
    int getEnergy(int x, int y);
    QImage getGX();
    QImage getGY();
    void calculateGradients();
    QImage greyTones(const QImage& image);
    QImage conv(const QImage& image,double* mat);
    QImage& image;
    std::vector<int> seam;

    EnergyFunctionI* energy;
    QImage Gx;
    QImage Gy;
    int width;
    int height;
};

#endif // SEAMCARVING_H
