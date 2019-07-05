/* Include the controller definition */
#include "footbot_aggregation.h"
/* Function definitions for XML parsing */
#include <argos3/core/utility/configuration/argos_configuration.h>
/* 2D vector definition */
#include <argos3/core/utility/math/vector2.h>

/****************************************/
/****************************************/

CFootBotAggregation::CFootBotAggregation() :
   m_pcWheels(NULL),
   m_pcProximity(NULL),
   m_cAlpha(90.0f),
   m_fDelta(0.5f),
   m_fWheelVelocity(2.5f),
   minDist(1),
   a(0.14f),
   b(0),
   m_fStayTurns(50),
   m_fLeaveTurns(50),
   m_fWalkTurns(50),
   m_pcRABA(NULL),
   m_pcRABS(NULL),
   m_pcRNG(NULL),
   avoidTurns(0), 
   stayTurns(0),
   leaveTurns(0),
   walkTurns(1),
   probaRule(1),
   lastMove(0),
   m_cGoStraightAngleRange(-ToRadians(m_cAlpha),
                           ToRadians(m_cAlpha)) {}
   
   

/****************************************/
/****************************************/

void CFootBotAggregation::Init(TConfigurationNode& t_node) {
   /*
    * Get sensor/actuator handles
    *
    * The passed string (ex. "differential_steering") corresponds to the
    * XML tag of the device whose handle we want to have. For a list of
    * allowed values, type at the command prompt:
    *
    * $ argos3 -q actuators
    *
    * to have a list of all the possible actuators, or
    *
    * $ argos3 -q sensors
    *
    * to have a list of all the possible sensors.
    *
    * NOTE: ARGoS creates and initializes actuators and sensors
    * internally, on the basis of the lists provided the configuration
    * file at the <controllers><footbot_aggregation><actuators> and
    * <controllers><footbot_aggregation><sensors> sections. If you forgot to
    * list a device in the XML and then you request it here, an error
    * occurs.
    */
   m_pcWheels    = GetActuator<CCI_DifferentialSteeringActuator>("differential_steering");
   m_pcProximity = GetSensor  <CCI_FootBotProximitySensor      >("footbot_proximity"    );
   m_pcRABA      = GetActuator<CCI_RangeAndBearingActuator     >("range_and_bearing"    );
   m_pcRABS      = GetSensor  <CCI_RangeAndBearingSensor       >("range_and_bearing"    );
   /*
    * Parse the configuration file
    */
   m_cAlpha = CDegrees(90.0f);
   m_cGoStraightAngleRange.Set(-ToRadians(m_cAlpha), ToRadians(m_cAlpha));
   m_fDelta = 0.5;
   m_fWheelVelocity = 10;
   GetNodeAttributeOrDefault(t_node, "minDist", minDist, minDist);
   GetNodeAttributeOrDefault(t_node, "leaveTurns", m_fLeaveTurns, m_fLeaveTurns);
   GetNodeAttributeOrDefault(t_node, "stayTurns", m_fStayTurns, m_fStayTurns);
   GetNodeAttributeOrDefault(t_node, "walkTurns", m_fWalkTurns, m_fWalkTurns);
   GetNodeAttributeOrDefault(t_node, "rule", probaRule, probaRule);
   
   GetNodeAttributeOrDefault(t_node, "aParam", a, a);
   if(probaRule>=2)
        GetNodeAttributeOrDefault(t_node, "bParam", b, b);
   
   /* Create a random number generator. We use the 'argos' category so
      that creation, reset, seeding and cleanup are managed by ARGoS. */
   m_pcRNG = CRandom::CreateRNG("argos");
   Reset();
}

void CFootBotAggregation::Reset() {
    lastMove = 0;
    avoidTurns = 0;
    leaveTurns = 0;
    stayTurns = 0;
    walkTurns = 1;
    m_pcRABA->ClearData();
    ChangeState(STATE_WALK);
}

/****************************************/
/****************************************/

void CFootBotAggregation::ControlStep() {
    ++lastMove;
    if(!avoidTurns) {
        switch(state) {
            case STATE_WALK:
                Walk();
                break;
            case STATE_STAY:
                Stay();
                break;
            case STATE_LEAVE:
                Leave();
                break;
        }
    }
    else
        --avoidTurns;
}

/****************************************/
/****************************************/

void CFootBotAggregation::Move() {
        /* Get readings from proximity sensor */
    const CCI_FootBotProximitySensor::TReadings& tProxReads = m_pcProximity->GetReadings();

        bool obstacle = false;
    // Check obstacles in the [-pi/2,pi/2] range
    for(size_t i = 0; i < tProxReads.size(); ++i) {
        if(m_cGoStraightAngleRange.WithinMinBoundIncludedMaxBoundIncluded(tProxReads[i].Angle)) {
            if(tProxReads[i].Value > m_fDelta) {
                obstacle = true;
            }
        }
    }
    
    if(obstacle) {
        m_pcWheels->SetLinearVelocity(-m_fWheelVelocity, m_fWheelVelocity);
        avoidTurns = m_pcRNG->Uniform(CRange<UInt32>(0.25*COMPLETE_TURN,0.75*COMPLETE_TURN));
    } else {
        m_pcWheels->SetLinearVelocity(m_fWheelVelocity, m_fWheelVelocity);
    }
    lastMove = 0;
}

void CFootBotAggregation::ChangeState(unsigned short int newState) {
    if(probaRule>1) {
        if(newState==STATE_LEAVE)
            newState = STATE_WALK;
    }
    m_pcRABA->SetData(0, newState);
    state = newState;
    switch(newState) {
        case STATE_WALK:
            Move();
            walkTurns = m_fWalkTurns;
            break;
        case STATE_STAY:
            m_pcWheels->SetLinearVelocity(0, 0);
            stayTurns = m_fStayTurns;
            break;
        case STATE_LEAVE:
            Move();
            leaveTurns = m_fLeaveTurns;
            break;
    }
}

unsigned int CFootBotAggregation::CountNeighbours() {
    const CCI_RangeAndBearingSensor::TReadings& tPackets = m_pcRABS->GetReadings();
    unsigned int counter = 0;
    for(size_t i = 0; i < tPackets.size(); ++i) {
        if(tPackets[i].Range < minDist and tPackets[i].Data[0] == STATE_STAY) {
            ++counter;
        }
    }
    
    return counter;
}

float CFootBotAggregation::ComputeProba(unsigned int n) {
    switch(probaRule) {
        case 1: //linear
            ++n;
            return n*a;
            break;
        case 2: //functions
            switch(state) {
                case STATE_WALK: //P_join
                    return 0.05+0.45*(1-exp(-a*n));
                    break;
                case STATE_STAY: //1-P_leave
                    return 1-0.75*exp(-b*n);
                    break;
            }
            break;
        case 3: //functions v1
            switch(state) {
                case STATE_WALK: //P_join
                    return 0.03+0.48*(1-exp(-a*n));
                    break;
                case STATE_STAY: //1-P_leave
                    return 1-exp(-b*n);
                    break;
            }
            break;
    }
}

void CFootBotAggregation::Walk() {
    --walkTurns;
    if(walkTurns == 0) {
        float p = ComputeProba(CountNeighbours());
        if(m_pcRNG->Uniform(CRange<Real>(0.0f,1.0f)) < p)
            ChangeState(STATE_STAY);
        else
            ChangeState(STATE_WALK);
    } else if(state==STATE_WALK)
        Move();
}

void CFootBotAggregation::Stay() {
    --stayTurns;
    if(stayTurns == 0) {        
        float p = ComputeProba(CountNeighbours());
        if(m_pcRNG->Uniform(CRange<Real>(0.0f,1.0f)) < p)
            ChangeState(STATE_STAY);
        else
            ChangeState(STATE_LEAVE);
    }
    
}

void CFootBotAggregation::Leave() {
    --leaveTurns;
    if(leaveTurns == 0)
        ChangeState(STATE_WALK);
    else
        Move();    
}

string CFootBotAggregation::GetState() {
    switch(state) {
        case STATE_WALK:
            return "WALK";
            break;
        case STATE_STAY:
            return "STAY";
            break;
        case STATE_LEAVE:
            return "LEAVE";
            break;
    }
    //return "0";
}

int CFootBotAggregation::LastMove() {
    return lastMove;
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
REGISTER_CONTROLLER(CFootBotAggregation, "footbot_aggregation_controller")
