#include "ngaggagent.h"

using namespace std;

NgAggAgent::NgAggAgent(World* w, Vector2D pos, double aParam, double bParam, double cParam, bool weak, short int bits) : AggregationAgent(w,pos,aParam,bParam,cParam), weak(weak), bits(bits)
{
    //lexicon.push_back(255);//world->random(pow(2,bits)));
    lexicon.push_back(world->random(pow(2,bits)));
    state=STATE_STAY;
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
    /*if(lexicon.size()==0)
        lexicon.push_back(world->random(pow(2,bits)));*/
    Broadcast(lexicon[world->random(lexicon.size())]);
}

bool NgAggAgent::Hear(uint32_t word)
{
    bool found = false;
    //if(world->GetTime()%NG_TURNS==0) {
        for(vector<uint32_t>::iterator it=lexicon.begin(); it!=lexicon.end() && !found; ++it)
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
      for(vector<uint32_t>::const_iterator it=lexicon.begin(); it!=lexicon.end(); ++it)
          if(it!=lexicon.begin())
            str+=","+to_string(*it);
    }
    /*string str;
    if(lexicon.size()==1)
       str = "a: "+to_string(a)+" b:"+to_string(b)+" c:"+to_string(c);*/
    return str;
}
