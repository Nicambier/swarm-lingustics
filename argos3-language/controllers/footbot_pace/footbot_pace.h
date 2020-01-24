#ifndef FOOTBOT_AGGREGATION_cNG_H
#define FOOTBOT_AGGREGATION_cNG_H

#include "../footbot_aggregation_NG/footbot_aggregation_NG.h"
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <math.h>

#include "argos3/core/utility/logging/argos_log.h"
//#include <bitset>


/*
 * All the ARGoS stuff in the 'argos' namespace.
 * With this statement, you save typing argos:: every time.
 */
using namespace argos;
using namespace std;

class CFootBotPace {

public:

   /* Class constructor. */
   CFootBotPace();

   /* Class destructor. */
   virtual ~CFootBotPace() {}

   /*
    * This function initializes the controller.
    * The 't_node' variable points to the <parameters> section in the XML
    * file in the <controllers><footbot_diffusion_controller> section.
    */
   virtual void Init(TConfigurationNode& t_node);
   
   virtual void ControlStep();
   
   virtual float ComputeProba(unsigned int n);
   
   virtual unsigned short int Noise(unsigned short int w);
   
   virtual string text();
   
   bool hear(uint32_t w);
   void speak();
   
   //virtual void Stay();

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
   
private:
    /* Pointer to the differential steering actuator */
   CCI_DifferentialSteeringActuator* m_pcWheels;
   /* Pointer to the foot-bot proximity sensor */
   CCI_FootBotProximitySensor* m_pcProximity;
   /* Pointer to the range and bearing actuator */
   CCI_RangeAndBearingActuator*  m_pcRABA;
   /* Pointer to the range and bearing sensor */
   CCI_RangeAndBearingSensor* m_pcRABS;
   
   double noise;
   double a,b,rho;
   vector<uint32_t> lexicon;
   int t;
   int turn;
   bool STAY;
};

#endif
