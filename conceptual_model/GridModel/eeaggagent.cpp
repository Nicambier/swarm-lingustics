#include "eeaggagent.h"

using namespace std;

EeAggAgent::EeAggAgent(World* w, Vector2D pos, double mutation_rate, bool weak) : NgAggAgent(w, pos, 0, 0, weak), m(mutation_rate)
{

}

bool EeAggAgent::Hear(uint16_t word)
{
    uint16_t newWord = Mutate(word);
    bool success = NgAggAgent::Hear(newWord);
    if(success)
    {
        /*a = 0.04*(newWord >> 8);
        b = 0.04*(newWord & 255);*/
        a = 0.25 + 0.25*(newWord >> 4);
        b = 0.25 + 0.25*(newWord & 15);
    }
    return success;
}

uint16_t EeAggAgent::Mutate(uint16_t word)
{
    for(int i=0; i<8; ++i)
    {
        if((rand()%255)/255.0 < m)
            word = word ^ (uint16_t) pow(2,i);
    }
    return word;
}
