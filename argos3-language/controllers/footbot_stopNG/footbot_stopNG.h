#ifndef FOOTBOT_STOPNG_H
#define FOOTBOT_STOPNG_H

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

class CFootBotStopNG : public CFootBotAggregation_NG {

public:

   /* Class constructor. */
   CFootBotStopNG();

   /* Class destructor. */
   virtual ~CFootBotStopNG() {}

   /*
    * This function initializes the controller.
    * The 't_node' variable points to the <parameters> section in the XML
    * file in the <controllers><footbot_diffusion_controller> section.
    */
   virtual void Init(TConfigurationNode& t_node);
   
   virtual void ChangeState(unsigned short int newState);
   
   virtual unsigned int CountNeighbours();
   
   virtual float ComputeProba(unsigned int n);

   /*
    * Called to cleanup what done by Init() when the experiment finishes.
    * In this example controller there is no need for clean anything up,
    * so the function could have been omitted. It's here just for
    * completeness.
    */
   virtual void Destroy() {}

private:
    unsigned short int currentWord;
    vector<unsigned short int> lexicon;
    
    bool success;
    bool empty;


};

#endif
