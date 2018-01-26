/* Include the controller definition */
#include "footbot_aggregation_cNG.h"

/****************************************/
/****************************************/

using namespace std;

CFootBotAggregation_cNG::CFootBotAggregation_cNG() : CFootBotAggregation_NG() {}
   
   

/****************************************/
/****************************************/

void CFootBotAggregation_cNG::Init(TConfigurationNode& t_node) {
    CFootBotAggregation_NG::Init(t_node);
}

void CFootBotAggregation_cNG::Reset() {
    CFootBotAggregation_NG::Reset();
}

/****************************************/
/****************************************/

void CFootBotAggregation_cNG::ChangeState(unsigned short int newState) {
    CFootBotAggregation::ChangeState(newState);
    switch(newState) {
        case STATE_WALK: //this is to ensure that speak is turned off, even if we don't pass by the LEAVE state (rule 2)
             speak(false);
             break;
        case STATE_STAY:
            speak(true);
            break;
        case STATE_LEAVE:
            speak(false);
            break;
    }
}

unsigned int CFootBotAggregation_cNG::CountNeighbours() {
    const CCI_RangeAndBearingSensor::TReadings& tPackets = m_pcRABS->GetReadings();
    unsigned int counter = 1;
    for(size_t i = 0; i < tPackets.size(); ++i) {
        if(tPackets[i].Range < minDist and tPackets[i].Data[0] > 0) {
            if(hear(tPackets[i].Data[0]))
                ++counter;
            else
                counter = 1;
        }
    }
    
    return counter;
}

float CFootBotAggregation_cNG::ComputeProba(unsigned int n) {
    short int w = GetWord();
    a = 0.25*(w%16 + 1);
    b = 0.25*(w/16 + 1);
    //cout << w << " " << a << " " << b << " " << endl;
    return CFootBotAggregation::ComputeProba(n);
}

string CFootBotAggregation_cNG::text() {
    int intA = (int) a;
    int intB = (int) b;
    return to_string(intA)+"."+to_string((int) ((a-intA)*100))+" "+to_string(intB)+"."+to_string((int) ((b-intB)*100));
}

// void CFootBotAggregation_cNG::Stay() {
//     //m_pcRABA->SetData(0, currentWord); //uncomment if bots behave weirdly... but I don't know why it's there...
//     CFootBotAggregation::Stay();
// }

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
REGISTER_CONTROLLER(CFootBotAggregation_cNG, "footbot_aggregation_cNG_controller")
