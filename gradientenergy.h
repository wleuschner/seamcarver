#ifndef GRADIENTENERGY_H
#define GRADIENTENERGY_H
#include "energyfunctioni.h"
#include <QImage>
#include <vector>

enum kernel{FAST, PREWITT, SOBEL, LAPLACE};

class GradientEnergy : public EnergyFunctionI
{
public:
    GradientEnergy(QImage& I);
    virtual int calculateEnergy(int x, int y);
    virtual void updateV(std::vector<int> seam);
    virtual void updateH(std::vector<int> seam);
    void greyTones();
    void setKernel(kernel k);
    QImage getGX();
    QImage getGY();
    QImage getEnergyPlot();
    void calculateGradients();

    ~GradientEnergy();

private:
    QImage conv(double* mat, bool sum);

private:
    int height;
    int width;
    short changes;
    QImage& image;
    QImage grey;
    QImage Gx;
    QImage Gy;
    kernel mode;
    bool useBlur;
    unsigned int blurfactor;
};

#endif // GRADIENTENERGY_H
