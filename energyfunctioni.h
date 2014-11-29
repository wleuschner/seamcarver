#ifndef ENERGYFUNCTIONI_H
#define ENERGYFUNCTIONI_H

class EnergyFunctionI
{
public:
    EnergyFunctionI(QImage *image);
    virtual int calculateEnergy(int x, int y);
    virtual void update();
};

#endif // ENERGYFUNCTIONI_H
