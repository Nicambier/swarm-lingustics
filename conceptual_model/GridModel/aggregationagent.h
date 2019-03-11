#ifndef AGGREGATIONAGENT_H
#define AGGREGATIONAGENT_H

#include "agent.h"

#define STATE_STAY 0
#define STATE_WALK 1


class AggregationAgent : public Agent
{
protected:
    int n;
    double a;
    double b;

    unsigned char state;

public:
    AggregationAgent(World* w, Vector2D pos, double aParam, double bParam);

    void Loop();

    virtual void ComputeN();
    virtual double LeaveProba(int n);
    virtual double JoinProba(int n);

    virtual void Signal();

    virtual uint16_t GetColour() {
        switch (state) {
            case STATE_STAY:
                return 7<<2;
                break;
            case STATE_WALK:
                return 0;
                break;
            }
    }
};

#endif // AGGREGATIONAGENT_H
