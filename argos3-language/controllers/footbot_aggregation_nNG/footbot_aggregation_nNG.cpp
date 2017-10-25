/* Include the controller definition */
#include "footbot_aggregation_nNG.h"

/****************************************/
/****************************************/

using namespace std;

CFootBotAggregation_nNG::CFootBotAggregation_nNG() : CFootBotAggregation_NG() {}
   
   

/****************************************/
/****************************************/

void CFootBotAggregation_nNG::Init(TConfigurationNode& t_node) {
    CFootBotAggregation_NG::Init(t_node);
}

void CFootBotAggregation_nNG::Reset() {
    CFootBotAggregation_NG::Reset();
}

/****************************************/
/****************************************/

void CFootBotAggregation_nNG::ControlStep() {
    speak(true, 1);
    CFootBotAggregation_NG::ControlStep();
    
    const CCI_RangeAndBearingSensor::TReadings& tPackets = m_pcRABS->GetReadings();
    for(size_t i = 0; i < tPackets.size(); ++i) {
        if(tPackets[i].Range < minDist) {
            hear(tPackets[i].Data[1]);
        }
    }
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
REGISTER_CONTROLLER(CFootBotAggregation_nNG, "footbot_aggregation_nNG_controller")
