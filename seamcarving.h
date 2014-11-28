#ifndef SEAMCARVING_H
#define SEAMCARVING_H

#include <vector>
#include <QImage>

class SeamCarving
{
public:
    SeamCarving(QImage image);
    void removeSeam();

private:
    int getEnergy(int x, int y);
    void findSeam();
    QImage image;
    QImage Gx;
    QImage Gy;
    std::vector<int> seam;
};

#endif // SEAMCARVING_H
