#ifndef ENERGYFUNCTIONI_H
#define ENERGYFUNCTIONI_H

#include <vector>

class EnergyFunctionI
{
public:
    //EnergyFunctionI();
    virtual int calculateEnergy(int x, int y)=0;
    virtual void updateV(std::vector<int> seam )=0;
    virtual void updateH(std::vector<int> seam )=0;
};

#endif // ENERGYFUNCTIONI_H
