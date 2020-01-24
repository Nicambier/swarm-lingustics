#ifndef INFECTIOUSAGENT_H
#define INFECTIOUSAGENT_H

#include "eeaggagent.h"

class InfectiousAgent : public EeAggAgent
{
public:
    InfectiousAgent(uint32_t word, World* w, Vector2D pos, double mutation_rate, bool weak, short int bits = 8): EeAggAgent(w,pos,mutation_rate,weak,bits)
    {
        lexicon.clear();
        lexicon.push_back(word);
        state = STATE_STAY;
    }
};

#endif // INFECTIOUSAGENT_H
