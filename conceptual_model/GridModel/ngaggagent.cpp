#include "ngaggagent.h"

using namespace std;

NgAggAgent::NgAggAgent(World* w, Vector2D pos, double aParam, double bParam, double cParam, bool weak, short int bits) : AggregationAgent(w,pos,aParam,bParam,cParam), weak(weak), bits(bits)
{

}

void NgAggAgent::ComputeN()
{
    n = 0;
    random_shuffle ( msgs.begin(), msgs.end() );
    for(unsigned short int i=0; i<msgs.size(); ++i)
    {
        if(Hear(msgs[i]))
            ++n;
        else
            n = 0;
    }
    if(weak) {
        n = msgs.size();
    }
}

void NgAggAgent::Signal()
{
    Speak();
}

void NgAggAgent::Speak()
{
    if(lexicon.size()==0)
        lexicon.push_back(rand()%(int) pow(2,bits));
    Broadcast(lexicon[rand()%(int) lexicon.size()]);
}

bool NgAggAgent::Hear(uint16_t word)
{
    bool found = false;
    //if(world->GetTime()%NG_TURNS==0) {
        for(vector<uint16_t>::iterator it=lexicon.begin(); it!=lexicon.end() && !found; ++it)
            if(*it==word)
                found = true;

        if(found)
            lexicon.clear();
        lexicon.push_back(word);
    //}
    return found;
}

string NgAggAgent::toString() const {
  string str = to_string(pos.x)+","+to_string(pos.y)+"?";
  if(lexicon.size()>0) {
      str+=to_string(lexicon.front());
      for(vector<uint16_t>::const_iterator it=lexicon.begin(); it!=lexicon.end(); ++it)
          if(it!=lexicon.begin())
            str+=","+to_string(*it);
  }
  return str;
}
