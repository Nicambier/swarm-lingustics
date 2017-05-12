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
   m_fBaseProba(0.14f),
   m_fStayTurns(50),
   m_fLeaveTurns(50),
   m_pcRABA(NULL),
   m_pcRABS(NULL),
   m_pcRNG(NULL),
   avoidTurns(0), 
   stayTurns(0),
   leaveTurns(0),
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
    *
    * The user defines this part. Here, the algorithm accepts three
    * parameters and it's nice to put them in the config file so we don't
    * have to recompile if we want to try other settings.
    */
   GetNodeAttributeOrDefault(t_node, "alpha", m_cAlpha, m_cAlpha);
   m_cGoStraightAngleRange.Set(-ToRadians(m_cAlpha), ToRadians(m_cAlpha));
   GetNodeAttributeOrDefault(t_node, "delta", m_fDelta, m_fDelta);
   GetNodeAttributeOrDefault(t_node, "velocity", m_fWheelVelocity, m_fWheelVelocity);
   GetNodeAttributeOrDefault(t_node, "baseProba", m_fBaseProba, m_fBaseProba);
   GetNodeAttributeOrDefault(t_node, "leaveTurns", m_fLeaveTurns, m_fLeaveTurns);
   GetNodeAttributeOrDefault(t_node, "stayTurns", m_fStayTurns, m_fStayTurns);
   
   /* Create a random number generator. We use the 'argos' category so
      that creation, reset, seeding and cleanup are managed by ARGoS. */
   m_pcRNG = CRandom::CreateRNG("argos");
   Reset();
}

void CFootBotAggregation::Reset() {
    state = STATE_WALK;
    avoidTurns = 0;
    leaveTurns = 0;
    stayTurns = 0;
    m_pcRABA->ClearData();
    m_pcRABA->SetData(0, state);
}

/****************************************/
/****************************************/

void CFootBotAggregation::ControlStep() {
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

        float thresh = 0.4;
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
        avoidTurns = m_pcRNG->Uniform(CRange<UInt32>(0,MAX_TURNS));
    } else {
        m_pcWheels->SetLinearVelocity(m_fWheelVelocity, m_fWheelVelocity);
    }
    
}

void CFootBotAggregation::ChangeState(unsigned short int newState) {
    state = newState;
    m_pcRABA->SetData(0, newState);
    switch(newState) {
        case STATE_WALK:
            m_pcWheels->SetLinearVelocity(m_fWheelVelocity, m_fWheelVelocity);
            break;
        case STATE_STAY:
            m_pcWheels->SetLinearVelocity(0, 0);
            stayTurns = m_fStayTurns;
            break;
        case STATE_LEAVE:
            m_pcWheels->SetLinearVelocity(m_fWheelVelocity, m_fWheelVelocity);
            leaveTurns = m_fLeaveTurns;
            break;
    }
}

unsigned int CFootBotAggregation::CountNeighbours() {
    const CCI_RangeAndBearingSensor::TReadings& tPackets = m_pcRABS->GetReadings();
    unsigned int counter = 1;
    for(size_t i = 0; i < tPackets.size(); ++i) {
        switch(tPackets[i].Data[0]) {
            case STATE_STAY:
                ++counter;
                break;
        }
    }
    return counter;
}
            

void CFootBotAggregation::Walk() {
    float p = CountNeighbours()*m_fBaseProba;
    cout<<p;
    if(m_pcRNG->Uniform(CRange<Real>(0.0f,1.0f)) < p)
        ChangeState(STATE_STAY);
    else
        Move();
}

void CFootBotAggregation::Stay() {
    --stayTurns;
    if(stayTurns == 0) {        
        float p = CountNeighbours()*m_fBaseProba;
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
