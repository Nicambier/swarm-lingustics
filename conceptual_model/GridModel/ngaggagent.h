#ifndef NGAGGAGENT_H
#define NGAGGAGENT_H

#include <vector>
#include <algorithm>
#include "aggregationagent.h"

//#define NG_TURNS 0

class NgAggAgent : public AggregationAgent
{
protected:
    std::vector<uint16_t> lexicon;
    bool weak;
    short int bits;
public:
    NgAggAgent(World* w, Vector2D pos, double aParam, double bParam, double cParam, bool weak=false, short int bits = 8);

    virtual void ComputeN();
    virtual void Signal();

    virtual void Speak();
    virtual bool Hear(uint16_t word);

    virtual uint16_t GetColour() {
        switch (state) {
        case STATE_STAY:
            if(lexicon.size()>=1) {
                if(bits>8)
                    return lexicon.front();
                else if(bits==8) {
                    uint16_t wo=lexicon.front();
                    uint16_t col=((wo<<8)&32768) + ((wo<<7)&8192) + ((wo<<6)&2048) + ((wo<<5)&512) + ((wo<<4)&128) + ((wo<<3)&32) + ((wo<<2)&8) + ((wo<<1)&2);
                    col += col>>1;
                    return col;
                }
            } else
                return 0;//31<<5;
            break;
        case STATE_WALK:
            return 0;
            break;
        }
    }

    virtual std::string toString() const;
};

#endif // NGAGGAGENT_H
