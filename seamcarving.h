#ifndef SEAMCARVING_H
#define SEAMCARVING_H

#include <vector>
#include <QImage>


class SeamCarving
{
public:
    SeamCarving(QImage& image);
    void removeSeamV();
    QImage getGX();
    QImage getGY();
    QImage energyDist;
    void findSeamV();
    QImage getImage();
private:
    QImage greyTones(const QImage& image);
    QImage conv(const QImage& image,double* mat);
    void saveEnergyDist(long long* M);

    int getEnergy(int x, int y);
    void calculateGradients();
    QImage& image;
    QImage Gx;
    QImage Gy;
    std::vector<int> seam;

    int width;
    int height;
};

#endif // SEAMCARVING_H
