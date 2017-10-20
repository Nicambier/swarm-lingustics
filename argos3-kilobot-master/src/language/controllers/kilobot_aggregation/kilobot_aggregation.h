#ifndef KILOBOT_AGGREGATION_H
#define KILOBOT_AGGREGATION_H

/*
 * Include some necessary headers.
 */

/* Definition of the CCI_Controller class. */
#include <argos3/core/control_interface/ci_controller.h>
/* Definition of the differential steering actuator */
#include <argos3/plugins/robots/generic/control_interface/ci_differential_steering_actuator.h>

#include <argos3/plugins/robots/kilobot/control_interface/ci_kilobot_communication_sensor.h>

#include <argos3/plugins/robots/kilobot/control_interface/ci_kilobot_communication_actuator.h>
/* Random number generator */
#include <argos3/core/utility/math/rng.h>
/* Logging functions */
#include <argos3/core/utility/logging/argos_log.h>

#include <vector>
#include <string>


/*
 * All the ARGoS stuff in the 'argos' namespace.
 * With this statement, you save typing argos:: every time.
 */
using namespace argos;
using namespace std;

/*
 * A controller is simply an implementation of the CCI_Controller class.
 */
class CKilobotAggregation : public CCI_Controller {

public:

   /* Class constructor. */
   CKilobotAggregation();

   /* Class destructor. */
   virtual ~CKilobotAggregation() {}

   /*
    * This function initializes the controller.
    * The 't_node' variable points to the <parameters> section in the XML
    * file in the <controllers><footbot_diffusion_controller> section.
    */
   virtual void Init(TConfigurationNode& t_node);

   /*
    * This function is called once every time step.
    * The length of the time step is set in the XML file.
    */
   virtual void ControlStep();
   
   virtual void Move();
   
   virtual void ChangeState(unsigned short int newState);
   
   virtual unsigned int CountNeighbours();
   
   virtual float ComputeProba(unsigned int n);
   
   virtual void Walk();
   
   virtual void Stay();
   
   virtual string GetState();
   
   virtual int LastMove();

   /*
    * This function resets the controller to its state right after the
    * Init().
    * It is called when you press the reset button in the GUI.
    * In this example controller there is no need for resetting anything,
    * so the function could have been omitted. It's here just for
    * completeness.
    */
   virtual void Reset();

   /*
    * Called to cleanup what done by Init() when the experiment finishes.
    * In this example controller there is no need for clean anything up,
    * so the function could have been omitted. It's here just for
    * completeness.
    */
   virtual void Destroy() {}    

protected:

   /* Pointer to the differential steering actuator */
   CCI_DifferentialSteeringActuator* m_pcMotors;
   /* Pointer to the range and bearing actuator */
   CCI_KilobotCommunicationActuator*  m_pcRABA;
   /* Pointer to the range and bearing sensor */
   CCI_KilobotCommunicationSensor* m_pcRABS;
   /* The random number generator */
   CRandom::CRNG* m_pcRNG;
   
   int lastMove;
   
   int avoidTurns;
   int walkTurns;
   unsigned int minDist;
   unsigned short int state;
   
   /* Base staying proba */
   Real a,b;
};

#endif
