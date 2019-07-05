#include "eeaggagent.h"

using namespace std;

EeAggAgent::EeAggAgent(World* w, Vector2D pos, double mutation_rate, bool weak, short int bits) : NgAggAgent(w, pos, 0, 0, weak, bits), m(mutation_rate)
{

}

void EeAggAgent::ComputeN() {
    if(weak) {
        /*random_shuffle ( msgs.begin(), msgs.end() );
        for(unsigned short int i=0; i<msgs.size(); ++i)
            Hear(msgs[i]);*/
        if(msgs.size()>0)
            Hear(msgs[rand()%msgs.size()]);
        n = msgs.size();
    }
    else {
        n = 0;
        for(unsigned short int i=0; i<msgs.size(); ++i)
        {
            if(Hear(msgs[i]))
                ++n;
            else
                n = 0;
        }
    }
    UpdateParameters();
}

void EeAggAgent::UpdateParameters() {
    if(lexicon.size()>0)
    {
        uint16_t newWord = lexicon[rand()%lexicon.size()];
        switch(bits) {
        case 16:
            a = 0.25 + 0.25*((newWord >> 5)&31);
            b = 0.25 + 0.25*(newWord & 31);
            c = 0.125 + 0.125*((newWord >> 10)&7);
            d = 0.125 + 0.125*((newWord >> 13));
            break;
        case 13:
            a = 0.25 + 0.25*((newWord >> 5)&31);
            b = 0.25 + 0.25*(newWord & 31);
            c = 0.125*((newWord >> 10)&7);
            d = 1;
            break;
        case 10:
            a = 0.25 + 0.25*((newWord >> 5)&31);
            b = 0.25 + 0.25*(newWord & 31);
            c = 0.5;
            d = 1;
            break;
        case 8:
            a = 0.25 + 0.25*(newWord >> 4);
            b = 0.25 + 0.25*(newWord & 15);
            c = 0.5;
            d = 1;
            break;
        }
    }
}

bool EeAggAgent::Hear(uint16_t word)
{
    uint16_t newWord = Mutate(word);
    bool success = NgAggAgent::Hear(newWord);
    /*if(success)
    {
        switch(bits) {
        case 16:
            a = 0.25 + 0.25*((newWord >> 5)&31);
            b = 0.25 + 0.25*(newWord & 31);
            c = 0.125 + 0.125*((newWord >> 10)&7);
            d = 0.125 + 0.125*((newWord >> 13));
            break;
        case 13:
            a = 0.25 + 0.25*((newWord >> 5)&31);
            b = 0.25 + 0.25*(newWord & 31);
            c = 0.125*((newWord >> 10)&7);
            d = 1;
            break;
        case 10:
            a = 0.25 + 0.25*((newWord >> 5)&31);
            b = 0.25 + 0.25*(newWord & 31);
            c = 0.5;
            d = 1;
            break;
        case 8:
            a = 0.25 + 0.25*(newWord >> 4);
            b = 0.25 + 0.25*(newWord & 15);
            c = 0.5;
            d = 1;
            break;
        }
    }*/
    return success;
}

uint16_t EeAggAgent::Mutate(uint16_t word)
{
    for(int i=0; i<bits; ++i)
    {
        if((rand()%255)/255.0 < m)
            word = word ^ (uint16_t) pow(2,i);
    }
    return word;
}
