#ifndef ENERGYFUNCTIONI_H
#define ENERGYFUNCTIONI_H

class EnergyFunctionI
{
public:
    //EnergyFunctionI();
    virtual int calculateEnergy(int x, int y)=0;
    virtual void update()=0;
};

#endif // ENERGYFUNCTIONI_H
