/*
 * AUTHOR: Vito Trianni <vito.trianni@istc.cnr.it>
 *
 * An example diffusion controller for the kilobot.
 *
 * This controller makes the robots move randomly and diffuse in the
 * environment.  Each robot performs a random walk: it moves straight
 * until a timeout expires, and then turns for a random amount of
 * time.
 *
 * The controller uses the two motors to move the robot around.
 *
 * This controller is meant to be used with the XML files:
 *    experiments/kilobot_diffusion.argos
 */

#ifndef KILOBOT_DIFFUSION_H
#define KILOBOT_DIFFUSION_H

/*
 * Include some necessary headers.
 */

/* Definition of the CCI_Controller class. */
#include <argos3/core/control_interface/ci_controller.h>
/* Definition of the differential steering actuator */
#include <argos3/plugins/robots/generic/control_interface/ci_differential_steering_actuator.h>

/* Random number generator */
#include <argos3/core/utility/math/rng.h>
/* Logging functions */
#include <argos3/core/utility/logging/argos_log.h>
#include <vector>


/*
 * All the ARGoS stuff in the 'argos' namespace.
 * With this statement, you save typing argos:: every time.
 */
using namespace argos;


enum TStateNames {KILOBOT_STATE_STOP, KILOBOT_STATE_TURNING, KILOBOT_STATE_MOVING};


/*
 * A controller is simply an implementation of the CCI_Controller class.
 */
class CKilobotDiffusion : public CCI_Controller {

public:

   /* Class constructor. */
   CKilobotDiffusion();

   /* Class destructor. */
   virtual ~CKilobotDiffusion() {}

   /*
    * This function initializes the controller.
    * The 't_node' variable points to the <parameters> section in the XML
    * file in the <controllers><kilobot_diffusion_controller> section.
    */
   virtual void Init(TConfigurationNode& t_node);

   /*
    * This function is called once every time step.
    * The length of the time step is set in the XML file.
    */
   virtual void ControlStep();

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

   /*
    * These functions allow to track the current state of the robot
    */
   inline const TStateNames GetCurrentState() const {return m_tCurrentState;};
   inline const bool StateChanged() const {return (m_tPreviousState != m_tCurrentState);};

   
private:

   /*
    * The following variables are used as parameters for the
    * algorithm. You can set their value in the <parameters> section
    * of the XML configuration file, under the
    * <controllers><kilobot_diffusion_controller> section.
    */

   /* Pointer to the differential steering actuator */
   CCI_DifferentialSteeringActuator* m_pcMotors;

   /* behavioural state (moving/turning) */
   TStateNames m_tCurrentState;
   TStateNames m_tPreviousState;
   
   /* counters for random walk behaviour */
   UInt32 m_unMaxMotionSteps;
   UInt32 m_unCountMotionSteps;

   UInt32 m_unMaxTurningSteps;
   UInt32 m_unCountTurningSteps;

   /* actual motor speed */
   Real   m_fMotorL;
   Real   m_fMotorR;

   /* variables for the random number generation */
   CRandom::CRNG*  m_pcRNG;
};

#endif
