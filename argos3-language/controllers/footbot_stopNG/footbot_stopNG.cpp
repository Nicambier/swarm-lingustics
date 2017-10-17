/* Include the controller definition */
#include "footbot_stopNG.h"

/****************************************/
/****************************************/

using namespace std;

CFootBotStopNG::CFootBotStopNG() : CFootBotAggregation_NG() {}
   
   

/****************************************/
/****************************************/

void CFootBotStopNG::Init(TConfigurationNode& t_node) {
    CFootBotAggregation_NG::Init(t_node);
}

/****************************************/
/****************************************/
    
void CFootBotStopNG::ChangeState(unsigned short int newState) {
    state = newState;
    switch(newState) {
        case STATE_WALK:
            speak(true);
            Move();
            walkTurns = m_fWalkTurns;
            break;
        case STATE_STAY:
            speak(true);
            m_pcWheels->SetLinearVelocity(0, 0);
            stayTurns = 1;
            break;
        case STATE_LEAVE:
            speak(false);
            Move();
            leaveTurns = m_fLeaveTurns;
            break;
    }
}

unsigned int CFootBotStopNG::CountNeighbours() {
    const CCI_RangeAndBearingSensor::TReadings& tPackets = m_pcRABS->GetReadings();
    bool success = true;
    bool empty = true;
    for(size_t i = 0; i < tPackets.size(); ++i) {
        if(tPackets[i].Range < minDist) {
            empty = false;
            if(!hear(tPackets[i].Data[0]))
                success = false;
        }
    }
    if(success && !empty)
        return 1;
    else
        return 0;
}

float CFootBotStopNG::ComputeProba(unsigned int n) {
    return n;
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
REGISTER_CONTROLLER(CFootBotStopNG, "footbot_stopNG_controller")
