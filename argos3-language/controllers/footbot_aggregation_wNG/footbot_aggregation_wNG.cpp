/* Include the controller definition */
#include "footbot_aggregation_wNG.h"

/****************************************/
/****************************************/

using namespace std;

CFootBotAggregation_wNG::CFootBotAggregation_wNG() : CFootBotAggregation_NG() {}
   
   

/****************************************/
/****************************************/

void CFootBotAggregation_wNG::Init(TConfigurationNode& t_node) {
    CFootBotAggregation_NG::Init(t_node);
}

void CFootBotAggregation_wNG::Reset() {
    CFootBotAggregation_NG::Reset();
}

/****************************************/
/****************************************/

void CFootBotAggregation_wNG::ChangeState(unsigned short int newState) {
    CFootBotAggregation::ChangeState(newState);
    switch(newState) {
        case STATE_WALK:
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

unsigned int CFootBotAggregation_wNG::CountNeighbours() {
    const CCI_RangeAndBearingSensor::TReadings& tPackets = m_pcRABS->GetReadings();
    unsigned int counter = 1;
    for(size_t i = 0; i < tPackets.size(); ++i) {
        if(tPackets[i].Range < minDist and tPackets[i].Data[0] != STATE_WALK) {
            hear(tPackets[i].Data[0]);
            ++counter;
        }
    }
    
    return counter;
}

// void CFootBotAggregation_wNG::Stay() {
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
REGISTER_CONTROLLER(CFootBotAggregation_wNG, "footbot_aggregation_wNG_controller")
