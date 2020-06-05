#ifndef INFECTIOUSAGENT_H
#define INFECTIOUSAGENT_H

#include "eeaggagent.h"

class InfectiousAgent : public EeAggAgent
{
public:
    InfectiousAgent(World* w, Vector2D pos, double mutation_rate, bool weak, short int bits = 8): EeAggAgent(w,pos,mutation_rate,weak,bits)
    {
        state = STATE_STAY;
    }

    void InfectLexiconWith(uint32_t word) {
         lexicon.clear();
         lexicon.push_back(word);
    }
};

#endif // INFECTIOUSAGENT_H
