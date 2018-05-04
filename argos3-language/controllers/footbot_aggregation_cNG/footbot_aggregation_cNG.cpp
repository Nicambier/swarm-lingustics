/* Include the controller definition */
#include "footbot_aggregation_cNG.h"

/****************************************/
/****************************************/

//using namespace std;

CFootBotAggregation_cNG::CFootBotAggregation_cNG() : CFootBotAggregation_NG() {}
   
   

/****************************************/
/****************************************/

void CFootBotAggregation_cNG::Init(TConfigurationNode& t_node) {
    GetNodeAttributeOrDefault(t_node, "noise", noise, noise);
    GetNodeAttributeOrDefault(t_node, "ntype", ntype, ntype);
    GetNodeAttributeOrDefault(t_node, "fitness", fitness, fitness);
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
    unsigned short int w;
    for(size_t i = 0; i < tPackets.size(); ++i) {
        w = Noise(tPackets[i].Data[0]);
        if(tPackets[i].Range < minDist and w > 0) {
            if(hear(w))
                ++counter;
            else
                counter = 1;
        }
    }
    
    return counter;
}

float CFootBotAggregation_cNG::ComputeProba(unsigned int n) {
    short int w = GetWord();
    //a = 0.02*w;
    if(fitness)
    {
        if(w==0) {
            a = 0;
            b = 0;
        } else {
            a = 1.25+0.25*(w%16);
            b = 1.25+0.25*(w/16);
        }
    }
    //cout << w << " " << a << " " << b << " " << endl;
    return CFootBotAggregation::ComputeProba(n);
}

unsigned short int CFootBotAggregation_cNG::Noise(unsigned short int w) {
    if(ntype=="shannon"){
        unsigned short int n = 0;
        for(int bit=0; bit<8; ++bit)
            if(m_pcRNG->Uniform(CRange<Real>(0.0f,1.0f)) < (noise/8))
                n+=pow(2,bit);
        return n ^ w;
    }
    else if(ntype=="gauss")
    {
        unsigned short int a=w%16,b=w/16;
        if(w!=0)
        {
            double aProb = m_pcRNG->Uniform(CRange<Real>(0.0f,1.0f));
            double bProb = m_pcRNG->Uniform(CRange<Real>(0.0f,1.0f));
            if(aProb < noise/2)
            {
                if(aProb<noise/4)
                    a-=1;
                else
                    a=(a+1)%16;
            }
            if(bProb < noise/2)
            {
                if(bProb<noise/4)
                    b-=1;
                else
                    b=(b+1)%16;
            }
        }
        return b*16 + a;
    }
    else if(ntype=="eliseo")
    {
        unsigned short int a=w%16,b=w/16;
        if(m_pcRNG->Uniform(CRange<Real>(0.0f,1.0f)) < (noise/2))
            a = m_pcRNG->Uniform(CRange<UInt32>(0,16));
        if(m_pcRNG->Uniform(CRange<Real>(0.0f,1.0f)) < (noise/2))
            b = m_pcRNG->Uniform(CRange<UInt32>(0,16));
        return b*16 + a;
        
    }
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
