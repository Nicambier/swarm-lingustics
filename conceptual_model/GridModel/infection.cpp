#include "infection.h"
#include "infectiousagent.h"

using namespace std;

Infection::Infection(int x, int y, int pop, AgentFactory* factory, double rate, uint8_t iType, unsigned int seed): Erosion(x,y,pop,factory,seed), rate(rate), infectType(iType) {
    /*switch(pop) {
        case 25:
            initWord = 16681317;
            break;
        case 50:
            initWord = 7981047;
            break;
        case 100:
            initWord = 7822581;
            break;
        case 200:
            initWord = 1589208;
            break;
    }*/
    initWord = 16681317;
    for (list<Agent*>::iterator it=agents.begin(); it!=agents.end(); ++it) {
        InfectiousAgent* temp = dynamic_cast<InfectiousAgent*>(*it);
        temp->InfectLexiconWith(initWord);

    }
    if(infectType==REGULAR)
        rate = 1-rate;
}

void Infection::Run()
{
    if(t==0 && rate>0 && infectType!=REGULAR) {
        uint32_t infect;
        if(infectType==HOMOGENEOUS)
            infect = (248<<16) + (50<<8) + 50;
        else if(infectType==HOMOGENEOUS_RANDOM)
            infect = random(pow(2,24));
        vector<int> selected;
        while(((float) selected.size())/agents.size() < rate) {
            int temp = random(agents.size());
            if((std::find(selected.begin(), selected.end(), temp) == selected.end())) {
                selected.push_back(temp);
            }
        }
        sort(selected.begin(), selected.end());
        int i=0, j=0;
        for (list<Agent*>::iterator it=agents.begin(); it!=agents.end(); ++it) {
            if(i==selected[j]) {
                uint32_t inf = ((uint32_t) pow(2,random(25))) | ((uint32_t) pow(2,random(25)));
                if(infectType==HETEROGENEOUS)
                    infect = dynamic_cast<InfectiousAgent*>(*it)->GetLexicon()[0] ^ inf; //random(pow(2,24));
                if(infectType!=REGULAR)
                    dynamic_cast<InfectiousAgent*>(*it)->InfectLexiconWith(infect);
                else
                    dynamic_cast<InfectiousAgent*>(*it)->SetNoise(0);
                ++j;
            }
            ++i;
        }
    }
    else if(t==500 && infectType==REGULAR) {
        for (list<Agent*>::iterator it=agents.begin(); it!=agents.end(); ++it) {
                dynamic_cast<InfectiousAgent*>(*it)->SetNoise(0);
        }
    }
    World::Run();
}

bool Infection::isFinished() {
    bool converged = true;
    if(t>500) {
        uint32_t word = dynamic_cast<InfectiousAgent*>(agents.front())->GetLexicon().front();
        for (list<Agent*>::iterator it=agents.begin(); it!=agents.end() && converged; ++it) {
            if(dynamic_cast<NgAggAgent*>(*it)->GetLexicon().size() > 1)
                converged = false;
            else if(dynamic_cast<NgAggAgent*>(*it)->GetLexicon().front() != word)
                converged = false;
        }
        return converged;
    } else
        return false;
}

bool Infection::asInit() {
    if(isFinished()) {
        uint32_t word = dynamic_cast<InfectiousAgent*>(agents.front())->GetLexicon().front();
        return word==initWord;
    } else
        return false;
}
