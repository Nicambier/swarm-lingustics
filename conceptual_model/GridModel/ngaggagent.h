#ifndef NGAGGAGENT_H
#define NGAGGAGENT_H

#include <list>
#include "aggregationagent.h"

class NgAggAgent : public AggregationAgent
{
protected:
    std::list<uint16_t> lexicon;
    bool weak;
public:
    NgAggAgent(World* w, Vector2D pos, double aParam, double bParam, bool weak=false);

    virtual void ComputeN();
    virtual void Signal();

    virtual void Speak();
    virtual bool Hear(uint16_t word);

    virtual uint16_t GetColour() {
        switch (state) {
        case STATE_STAY:
            if(lexicon.size()==1) {
                return lexicon.front();
            } else
                return 7<<2;
            break;
        case STATE_WALK:
            return 0;
            break;
        }
    }

    virtual std::string toString() const;
};

#endif // NGAGGAGENT_H
