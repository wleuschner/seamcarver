#ifndef GRADIENTENERGY_H
#define GRADIENTENERGY_H
#include "energyfunctioni.h"
#include <QImage>
class GradientEnergy : public EnergyFunctionI
{
public:
    GradientEnergy(QImage& I);
    virtual int calculateEnergy(int x, int y);
    virtual void update();
    QImage getGX();
    QImage getGY();
    QImage getEnergyPlot();
    ~GradientEnergy();

private:
    void greyTones();
    QImage conv(double* mat);
    void calculateGradients();

private:
    QImage& image;
    QImage grey;
    QImage Gx;
    QImage Gy;
};

#endif // GRADIENTENERGY_H
