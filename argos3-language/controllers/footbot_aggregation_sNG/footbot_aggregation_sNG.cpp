/* Include the controller definition */
#include "footbot_aggregation_sNG.h"

/****************************************/
/****************************************/

using namespace std;

CFootBotAggregation_sNG::CFootBotAggregation_sNG() : CFootBotAggregation_NG() {}
   
   

/****************************************/
/****************************************/

void CFootBotAggregation_sNG::Init(TConfigurationNode& t_node) {
    CFootBotAggregation_NG::Init(t_node);
}

void CFootBotAggregation_sNG::Reset() {
    CFootBotAggregation_NG::Reset();
}

/****************************************/
/****************************************/

void CFootBotAggregation_sNG::ChangeState(unsigned short int newState) {
    CFootBotAggregation::ChangeState(newState);
    switch(newState) {
        case STATE_WALK: 
            speak(false); //this is to ensure that speak is turned off, even if we don't pass by the LEAVE state (rule 2)
            break;
        case STATE_STAY:
            speak(true);
            break;
        case STATE_LEAVE:
            speak(false);
            break;
    }
}

unsigned int CFootBotAggregation_sNG::CountNeighbours() {
    const CCI_RangeAndBearingSensor::TReadings& tPackets = m_pcRABS->GetReadings();
    unsigned int counter = 0;
    for(size_t i = 0; i < tPackets.size(); ++i) {
        if(tPackets[i].Range < minDist and tPackets[i].Data[0] > 0) {
            if(hear(tPackets[i].Data[0]))
                ++counter;
            else
                counter = 0;
        }
    }
    
    return counter;
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
REGISTER_CONTROLLER(CFootBotAggregation_sNG, "footbot_aggregation_sNG_controller")
