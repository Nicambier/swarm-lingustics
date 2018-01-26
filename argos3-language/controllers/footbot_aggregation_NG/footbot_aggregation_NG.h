#ifndef FOOTBOT_AGGREGATION_NG_H
#define FOOTBOT_AGGREGATION_NG_H

#include "../footbot_aggregation/footbot_aggregation.h"
#include <sstream>
#include <string>
#include <vector>


/*
 * All the ARGoS stuff in the 'argos' namespace.
 * With this statement, you save typing argos:: every time.
 */
using namespace argos;
using namespace std;

class CFootBotAggregation_NG : public CFootBotAggregation {

public:

   /* Class constructor. */
   CFootBotAggregation_NG();

   /* Class destructor. */
   virtual ~CFootBotAggregation_NG() {}

   /*
    * This function initializes the controller.
    * The 't_node' variable points to the <parameters> section in the XML
    * file in the <controllers><footbot_diffusion_controller> section.
    */
   virtual void Init(TConfigurationNode& t_node);
   
   virtual void speak(bool activate, int channel = 0);
   virtual bool hear(unsigned short int w);
   
   virtual unsigned short int GetWord();
   virtual string text();
   virtual vector<unsigned short int> GetLexicon();

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
    unsigned short int currentWord;
    vector<unsigned short int> lexicon;


};

#endif
