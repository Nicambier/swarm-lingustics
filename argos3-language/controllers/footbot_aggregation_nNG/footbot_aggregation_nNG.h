#ifndef FOOTBOT_AGGREGATION_nNG_H
#define FOOTBOT_AGGREGATION_nNG_H

#include "../footbot_aggregation_NG/footbot_aggregation_NG.h"
#include <sstream>
#include <string>
#include <vector>


/*
 * All the ARGoS stuff in the 'argos' namespace.
 * With this statement, you save typing argos:: every time.
 */
using namespace argos;
using namespace std;

class CFootBotAggregation_nNG : public CFootBotAggregation_NG {

public:

   /* Class constructor. */
   CFootBotAggregation_nNG();

   /* Class destructor. */
   virtual ~CFootBotAggregation_nNG() {}

   /*
    * This function initializes the controller.
    * The 't_node' variable points to the <parameters> section in the XML
    * file in the <controllers><footbot_diffusion_controller> section.
    */
   virtual void Init(TConfigurationNode& t_node);
   
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


};

#endif
