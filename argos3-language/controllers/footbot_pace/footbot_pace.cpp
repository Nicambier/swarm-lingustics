/* Include the controller definition */
#include "footbot_aggregation_cNG.h"

/****************************************/
/****************************************/
#define TIMESTEP 10
//using namespace std;

CFootBotPace::CFootBotPace() {
    a = 0;
    b = 0;
    c = 0;
    
    t = 0;
    turn = 0;
}
   
   

/****************************************/
/****************************************/

void CFootBotPace::Init(TConfigurationNode& t_node) {
    m_pcWheels    = GetActuator<CCI_DifferentialSteeringActuator>("differential_steering");
    m_pcProximity = GetSensor  <CCI_FootBotProximitySensor      >("footbot_proximity"    );
    m_pcRABA      = GetActuator<CCI_RangeAndBearingActuator     >("range_and_bearing"    );
    m_pcRABS      = GetSensor  <CCI_RangeAndBearingSensor       >("range_and_bearing"    );
    GetNodeAttributeOrDefault(t_node, "noise", noise, noise);
}

void CFootBotPace::Reset() {
    CFootBotAggregation_NG::Reset();
    a = 0;
    b = 0;
    c = 0;
    t = 0;
    lexicon.clear();
}

void CFootBotPace::ControlStep() {
    
    
    
    ++t;
}

bool hear(uint32_t w) {
    
}

void speak() {
    
}

unsigned int CFootBotPace::CountNeighbours() {
    const CCI_RangeAndBearingSensor::TReadings& tPackets = m_pcRABS->GetReadings();
    unsigned int counter = 0;
    uint32_t w;
    for(size_t i = 0; i < tPackets.size(); ++i) {
        if(tPackets[i].Range < minDist and tPackets[i].Data[0] != 0) {
            w = Noise(tPackets[i].Data[0]);
            if(hear(w))
                ++counter;
            else//comment this part for strong link
                ++counter;
        }
    }
    return counter;
}

uint32_t CFootBotPace::Noise(uint32_t w) {
    uint32_t m = 0;
    for(int bit=0; bit<8; ++bit)
        if(m_pcRNG->Uniform(CRange<Real>(0.0f,1.0f)) < (noise)) {
            m+=pow(2,bit);
        }
    uint32_t res = m^w;
    return res;
}

string CFootBotPace::text() {
    return to_string((int) c/0.0039)+" "to_string((int) a/0.02)+" "+to_string((int) b/0.02);
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
REGISTER_CONTROLLER(CFootBotPace, "footbot_pace")
