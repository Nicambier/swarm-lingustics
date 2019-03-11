#ifndef EEAGGAGENT_H
#define EEAGGAGENT_H

#include "ngaggagent.h"

class EeAggAgent : public NgAggAgent
{
protected:
    double m;
public:
    EeAggAgent(World* w, Vector2D pos, double mutation_rate, bool weak);

    virtual bool Hear(uint16_t word);
    uint16_t Mutate(uint16_t word);
};

#endif // EEAGGAGENT_H
