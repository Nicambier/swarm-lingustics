#include <argos3/core/simulator/loop_functions.h>
#include <argos3/core/simulator/entity/entity.h>
#include <argos3/plugins/robots/foot-bot/simulator/footbot_entity.h>
#include <argos3/plugins/simulator/visualizations/qt-opengl/qtopengl_user_functions.h>
#include "../../controllers/footbot_aggregation_NG/footbot_aggregation_NG.h"
#include <fstream>
#include <sstream>

using namespace argos;

class CVisualise : public CLoopFunctions {

public:

   /**
    * Class constructor
    */
   CVisualise();

   /** 
    * Class destructor
    */
   virtual ~CVisualise();

   /**
    * Initializes the experiment.
    * It is executed once at the beginning of the experiment, i.e., when ARGoS is launched.
    * @param t_tree The parsed XML tree corresponding to the <loop_functions> section.
    */
   virtual void Init(TConfigurationNode& t_tree);

   /**
    * Resets the experiment to the state it was right after Init() was called.
    * It is executed every time you press the 'reset' button in the GUI.
    */
   virtual void Reset();

   /**
    * Undoes whatever Init() did.
    * It is executed once when ARGoS has finished the experiment.
    */
   virtual void Destroy();

   /**
    * Performs actions right before a simulation step is executed.
    */

private:


private:

   /**
    * The path of the output file.
    */
   std::string m_strInFile;

   /**
    * The stream associated to the output file.
    */
   std::ifstream m_cInFile;

};
