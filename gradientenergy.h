#ifndef GRADIENTENERGY_H
#define GRADIENTENERGY_H
#include "energyfunctioni.h";
#include <QImage>
class GradientEnergy : public EnergyFunctionI
{
public:
    GradientEnergy(QImage& I);
    int calculateEnergy(int x, int y);
    void update();
    QImage getGX();
    QImage getGY();
    QImage getEnergyPlot();
    ~GradientEnergy();

private:
    void greyTones(const QImage& image);
    QImage conv(const QImage& image,double* mat);
    void calculateGradients();

private:
    QImage& image;
    unsigned char* grey;
    QImage Gx;
    QImage Gy;
};

#endif // GRADIENTENERGY_H
