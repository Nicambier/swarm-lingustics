#include "randomaggagent.h"

RandomAggAgent::RandomAggAgent(World* w, Vector2D pos) : AggregationAgent(w,pos,0,0,0)
{
    //this is obsolete
    uint16_t param = world->random(65536);
    /*a = 0.04*(param >> 8);
    b = 0.04*(param & 255);*/
    a = 0.25 + 0.25*(param >> 4);
    b = 0.25 + 0.25*(param & 15);
}
