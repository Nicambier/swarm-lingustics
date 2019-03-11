#include "ngaggagent.h"

using namespace std;

NgAggAgent::NgAggAgent(World* w, Vector2D pos, double aParam, double bParam, bool weak) : AggregationAgent(w,pos,aParam,bParam), weak(weak)
{

}

void NgAggAgent::ComputeN()
{
    n = 0;
    for(list<uint16_t>::iterator it=msgs.begin(); it!=msgs.end(); ++it)
    {
        if(Hear(*it))
            ++n;
        else
            n = 0;
    }
    if(weak)
        n = msgs.size();
}

void NgAggAgent::Signal()
{
    Speak();
}

void NgAggAgent::Speak()
{
    if(lexicon.size()==0)
        lexicon.push_back(rand()%256);
    if(lexicon.size()==1)
        Broadcast(lexicon.front());
}

bool NgAggAgent::Hear(uint16_t word)
{
    bool found = false;
    for(list<uint16_t>::iterator it=lexicon.begin(); it!=lexicon.end() && !found; ++it)
        if(*it==word)
            found = true;

    if(found)
        lexicon.clear();
    lexicon.push_back(word);
    return found;
}

string NgAggAgent::toString() const {
  string str = to_string(pos.x)+","+to_string(pos.y)+"?"+to_string(lexicon.front());
  for(list<uint16_t>::const_iterator it=lexicon.begin(); it!=lexicon.end(); ++it)
      if(it!=lexicon.begin())
        str+=","+to_string(*it);
  return str;
}
