#ifndef EEAGGAGENT_H
#define EEAGGAGENT_H

#include "ngaggagent.h"
#include "math.h"
#include "stdio.h"

class EeAggAgent : public NgAggAgent
{
protected:
    double m;
public:
    EeAggAgent(World* w, Vector2D pos, double mutation_rate, bool weak, short int bits = 8);

    virtual bool Hear(uint32_t word);
    virtual void ComputeN();
    virtual void UpdateParameters();

    uint32_t Mutate(uint32_t word);
};

#endif // EEAGGAGENT_H
