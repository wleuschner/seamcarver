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
    void insertSeam(std::vector<int> xcord, int y);
    QImage getImage();
    std::vector<int> getSeam();

    QImage energyDist;
    int width;
    int height;

private:
    void saveEnergyDist(long long* M);
    int getEnergy(int x, int y);
    QImage& image;
    std::vector<int> seam;

    QImage* mask;
    EnergyFunctionI* energy;

};

#endif // SEAMCARVING_H
