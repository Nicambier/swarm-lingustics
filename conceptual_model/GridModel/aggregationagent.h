#ifndef AGGREGATIONAGENT_H
#define AGGREGATIONAGENT_H

#include "agent.h"
#include <algorithm>

#define STATE_STAY 0
#define STATE_WALK 1


class AggregationAgent : public Agent
{
protected:
    int n;
    double a;
    double b;
    double c;
    double d;

    unsigned char state;

public:
    AggregationAgent(World* w, Vector2D pos, double aParam, double bParam, double cParam);

    void Loop();

    virtual void ComputeN();
    virtual double LeaveProba(int n);
    virtual double JoinProba(int n);

    virtual void Signal();

    virtual uint32_t GetColour() const {
        switch (state) {
            case STATE_STAY:
                return 255<<8;
                break;
            case STATE_WALK:
                return 0;
                break;
            }
        return 0;
    }
};

#endif // AGGREGATIONAGENT_H
