#ifndef AGENTFACTORY_H
#define AGENTFACTORY_H

#include "eeaggagent.h"
#include "randomaggagent.h"

#define TYPE_AGENT 100
#define TYPE_AGG_AGENT 101
#define TYPE_NG_AGG_AGENT 102
#define TYPE_EE_AGG_AGENT 103
#define TYPE_RANDOM_AGG_AGENT 104

class AgentFactory
{
private:
    int type;
    double p1;
    double p2;

    bool weak;

public:
    AgentFactory(int type, double p1=0, double p2=0);
    Agent* GetNewAgent(World* w, Vector2D pos);

    void SetWeak(bool weak);
};

#endif // AGENTFACTORY_H
