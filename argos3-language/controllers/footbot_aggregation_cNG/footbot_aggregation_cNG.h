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

class CFootBotAggregation_cNG : public CFootBotAggregation_NG {

public:

   /* Class constructor. */
   CFootBotAggregation_cNG();

   /* Class destructor. */
   virtual ~CFootBotAggregation_cNG() {}

   /*
    * This function initializes the controller.
    * The 't_node' variable points to the <parameters> section in the XML
    * file in the <controllers><footbot_diffusion_controller> section.
    */
   virtual void Init(TConfigurationNode& t_node);
   
   virtual void ControlStep();
   
   virtual void ChangeState(unsigned short int newState);
   
   virtual unsigned int CountNeighbours();
   
   virtual float ComputeProba(unsigned int n);
   
   virtual unsigned short int Noise(unsigned short int w);
   
   virtual string text();
   
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
    double noise;    
    string ntype;
};

#endif
