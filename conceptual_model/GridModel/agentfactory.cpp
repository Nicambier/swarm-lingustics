#include "agentfactory.h"

AgentFactory::AgentFactory(int type, double p1, double p2, double p3) : type(type), p1(p1), p2(p2), p3(p3), weak(false)
{

}

Agent* AgentFactory::GetNewAgent(World* w, Vector2D pos)
{
    switch (type) {
    case TYPE_AGENT:
        return new Agent(w, pos);
        break;
    case TYPE_AGG_AGENT:
        return new AggregationAgent(w, pos, p1, p2, p3);
        break;
    case TYPE_NG_AGG_AGENT:
        return new NgAggAgent(w, pos, p1, p2, p3, weak);
        break;
    case TYPE_EE_AGG_AGENT:
        return new EeAggAgent(w, pos, p1, weak,24);
        break;
    case TYPE_RANDOM_AGG_AGENT:
        return new RandomAggAgent(w, pos);
        break;
    }
    return nullptr;
}

void AgentFactory::SetWeak(bool weak)
{
    this->weak = weak;
}
