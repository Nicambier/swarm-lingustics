#include "eeaggagent.h"

using namespace std;

EeAggAgent::EeAggAgent(World* w, Vector2D pos, double mutation_rate, bool weak, short int bits) : NgAggAgent(w, pos, 0, 0, 0, weak, bits), m(mutation_rate)
{
    /*float a = 2.38;
    float b = 1.42;
    float c = 0.62;
    uint32_t init_word = (int (round(a*50))<<8) + int (round(b*50)) + (int (round(c*255))<<16);
    lexicon.clear();
    lexicon.push_back(init_word);*/
}

void EeAggAgent::ComputeN() {
    //if(weak) {
        /*random_shuffle ( msgs.begin(), msgs.end() );
        for(unsigned short int i=0; i<msgs.size(); ++i)
            Hear(msgs[i]);*/
        if(msgs.size()>0)
            Hear(msgs[rand()%msgs.size()]);
        n = msgs.size();
    /*}
    else {
        n = 0;
        random_shuffle ( msgs.begin(), msgs.end() );
        for(unsigned short int i=0; i<msgs.size(); ++i)
        {
            if(Hear(msgs[i]))
                ++n;
            else
                n = 0;
        }
    }*/
    UpdateParameters();
}

void EeAggAgent::UpdateParameters() {
    if(lexicon.size()>0)
    {
        uint32_t newWord = lexicon[rand()%lexicon.size()];
        switch(bits) {
        case 24:
            a = 0.02*((newWord >> 8)&255);
            b = 0.02*(newWord & 255);
            c = 0.0039*((newWord >> 16)&255);
            d = 1-c;
            break;
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
            d = 0.125;
            break;
        case 10:
            a = 0.25 + 0.25*((newWord >> 5)&31);
            b = 0.25 + 0.25*(newWord & 31);
            c = 0.5;
            d = 0.03;
            break;
        case 8:
            a = 0.25 + 0.25*(newWord >> 4);
            b = 0.25 + 0.25*(newWord & 15);
            c = 0.5;
            d = 0.03;
            break;
        }
    }
}

bool EeAggAgent::Hear(uint32_t word)
{
    uint32_t newWord = Mutate(word);
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

uint32_t EeAggAgent::Mutate(uint32_t word)
{
    for(int i=0; i<bits; ++i)
    {
        if(world->uniform() < m)
            word = word ^ (uint32_t) pow(2,i);
    }
    return word;
}
