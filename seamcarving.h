#ifndef SEAMCARVING_H
#define SEAMCARVING_H

#include <vector>
#include <QImage>


class SeamCarving
{
public:
    SeamCarving(QImage& image);
    void removeSeamV();

    QImage energyDist;
    void findSeamV();
    QImage getImage();

private:
    void saveEnergyDist(long long* M);
    void findSeamV();
    int getEnergy(int x, int y);

    QImage& image;
    std::vector<int> seam;

    int width;
    int height;
};

#endif // SEAMCARVING_H
