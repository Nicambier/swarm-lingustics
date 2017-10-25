/* Include the controller definition */
#include "footbot_aggregation_NG.h"

/****************************************/
/****************************************/

using namespace std;

CFootBotAggregation_NG::CFootBotAggregation_NG() : CFootBotAggregation() {}
   
   

/****************************************/
/****************************************/

void CFootBotAggregation_NG::Init(TConfigurationNode& t_node) {
    CFootBotAggregation::Init(t_node);
}

void CFootBotAggregation_NG::Reset() {
    lexicon.clear();
    CFootBotAggregation::Reset();
}

/****************************************/
/****************************************/

void CFootBotAggregation_NG::speak(bool activate, int channel) {
    if(activate) {
        if(lexicon.size()==0)
            currentWord = m_pcRNG->Uniform(CRange<UInt32>(1,256));
        else
            currentWord = lexicon[m_pcRNG->Uniform(CRange<UInt32>(0,lexicon.size()))];
        m_pcRABA->SetData(channel, currentWord);
    } else
        m_pcRABA->SetData(channel, 0);
}

bool CFootBotAggregation_NG::hear(unsigned short int w) {
    bool inside = false;

    if(w > 0) {
        for (vector<unsigned short int>::iterator it = lexicon.begin() ; it != lexicon.end() && !inside; ++it) {
            if(w == *it) {
                inside = true;
            }
        }
                
        if(inside) {
            currentWord = w;
            if(lexicon.size() > 1) {
                lexicon.clear();
                lexicon.push_back(w);
            }
        } else if(not inside)
            lexicon.push_back(w);        
    }
    
    return inside;
}

unsigned short int CFootBotAggregation_NG::GetWord() {
    unsigned short int w=0;//means no convergence
    
    if(lexicon.size()==1)
        w = lexicon.front();
    
    return w;
}

vector<unsigned short int> CFootBotAggregation_NG::GetLexicon() {
    return lexicon;
}

/****************************************/
/****************************************/

/*
 * This statement notifies ARGoS of the existence of the controller.
 * It binds the class passed as first argument to the string passed as
 * second argument.
 * The string is then usable in the configuration file to refer to this
 * controller.
 * When ARGoS reads that string in the configuration file, it knows which
 * controller class to instantiate.
 * See also the configuration files for an example of how this is used.
 */
REGISTER_CONTROLLER(CFootBotAggregation_NG, "footbot_aggregation_NG_controller")
