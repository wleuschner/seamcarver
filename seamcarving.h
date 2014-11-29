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

private:
    QImage greyTones(const QImage& image);
    QImage conv(const QImage& image,double* mat);

    int getEnergy(int x, int y);
    void findSeamV();
    void calculateGradients();
    QImage& image;
    QImage Gx;
    QImage Gy;
    std::vector<int> seam;
};

#endif // SEAMCARVING_H
