#ifndef INFECTION_H
#define INFECTION_H

#include <vector>
#include <list>
#include <algorithm>
#include "erosion.h"

#define HOMOGENEOUS 1
#define HOMOGENEOUS_RANDOM 2
#define HETEROGENEOUS 3
#define REGULAR 4

class Infection : public Erosion
{
public:
    Infection(int x, int y, int pop, AgentFactory* factory, double rate, uint8_t iType, unsigned int seed=0);
    virtual void Run();
    virtual bool isFinished();
    bool asInit();

private:
    uint8_t infectType;
    uint32_t initWord;
    double rate;
};

#endif // INFECTION_H
