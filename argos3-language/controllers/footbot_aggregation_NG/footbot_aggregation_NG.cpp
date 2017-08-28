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

void CFootBotAggregation_NG::ChangeState(unsigned short int newState) {
    CFootBotAggregation::ChangeState(newState);
    switch(newState) {
        case STATE_WALK:
            m_pcRABA->SetData(0, 0);
            break;
        case STATE_STAY:
            if(lexicon.size()==0)
                currentWord = m_pcRNG->Uniform(CRange<UInt32>(1,256));
            else
                currentWord = lexicon[m_pcRNG->Uniform(CRange<UInt32>(0,lexicon.size()))];
            m_pcRABA->SetData(0, currentWord);
            break;
        case STATE_LEAVE:
            m_pcRABA->SetData(0, 0);
            break;
    }
}

bool CFootBotAggregation_NG::UpdateLexicon(unsigned short int w) {
    bool inside = false;

    if(w > 0) {
        for (vector<unsigned short int>::iterator it = lexicon.begin() ; it != lexicon.end(); ++it) {
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

unsigned int CFootBotAggregation_NG::CountNeighbours() {
    const CCI_RangeAndBearingSensor::TReadings& tPackets = m_pcRABS->GetReadings();
    unsigned int counter = 1;
    for(size_t i = 0; i < tPackets.size(); ++i) {
        if(tPackets[i].Range < 100) {
            if(UpdateLexicon(tPackets[i].Data[0]))
                ++counter;
            else
                counter = 1;
        }
    }
    
    return counter;
}

void CFootBotAggregation_NG::Stay() {
    m_pcRABA->SetData(0, currentWord);
    CFootBotAggregation::Stay();
}

string CFootBotAggregation_NG::GetState() {
    string word = "N/A";
    
    if(lexicon.size()==1) {
        ostringstream temp;
        temp << lexicon.front();
        word = temp.str();
    }
    
    return word;
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
