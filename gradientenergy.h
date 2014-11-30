#ifndef GRADIENTENERGY_H
#define GRADIENTENERGY_H
#include "energyfunctioni.h"
#include <QImage>
#include <vector>

class GradientEnergy : public EnergyFunctionI
{
public:
    GradientEnergy(QImage& I);
    virtual int calculateEnergy(int x, int y);
    virtual void updateV(std::vector<int> seam);
    virtual void updateH(std::vector<int> seam);
    void greyTones();
    QImage getGX();
    QImage getGY();
    QImage getEnergyPlot();
    ~GradientEnergy();

private:
    QImage conv(double* mat);
    void calculateGradients();

private:
    int height;
    int width;
    short changes;
    QImage& image;
    QImage grey;
    QImage Gx;
    QImage Gy;
};

#endif // GRADIENTENERGY_H
