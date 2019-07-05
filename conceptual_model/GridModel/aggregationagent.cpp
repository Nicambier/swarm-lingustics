#include "aggregationagent.h"

using namespace std;

AggregationAgent::AggregationAgent(World* w, Vector2D pos, double aParam, double bParam, double cParam) : Agent(w,pos), n(0), a(aParam), b(bParam), c(cParam), d(1), state(STATE_WALK)
{

}

void AggregationAgent::Loop()
{
    ComputeN();
    //Signal();
    double r = (rand()%256)/256.0;
    switch (state) {
    case STATE_WALK:
        if(r<JoinProba(n))
            state = STATE_STAY;
        else
            RandomWalk();
        break;
    case STATE_STAY:
        Signal(); //you need to signal before transition or this would never happen since LeaveProba=1 when n=0
        if(r<LeaveProba(n))
            state = STATE_WALK;
        //else do nothing
        break;
    }
}

void AggregationAgent::Signal()
{
    Broadcast(100);
}

void AggregationAgent::ComputeN()
{
    n = msgs.size();
}

double AggregationAgent::LeaveProba(int n)
{
    return exp(-b*n);
}

double AggregationAgent::JoinProba(int n)
{
    //return 0.03+c*(1-exp(-a*n));
    return 0.125 + c*(1-exp(-a*n));
}
