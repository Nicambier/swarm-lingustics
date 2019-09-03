#ifndef EROSION_H
#define EROSION_H

#include "world.h"

class Agent;
struct Vector2D;

class Erosion : public World
{
public:
    Erosion(int x, int y, int pop, AgentFactory* factory, unsigned int seed=0);
    virtual bool MoveAgentTo(Agent* a, Vector2D pos);
    virtual void Run();

    virtual bool isFinished() {return pop==0;}

protected:
    std::list<Agent*> toDelete;
};

#endif // EROSION_H
