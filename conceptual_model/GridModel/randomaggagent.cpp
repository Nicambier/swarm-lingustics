#include "randomaggagent.h"

RandomAggAgent::RandomAggAgent(World* w, Vector2D pos) : AggregationAgent(w,pos,0,0)
{
    uint16_t param = rand()%65536;
    a = 0.04*(param >> 8);
    b = 0.04*(param & 255);
    /*a = 0.25 + 0.25*(param >> 4);
    b = 0.25 + 0.25*(param & 15);*/
}
