#ifndef WORD_QTUSER_FUNCTIONS_H
#define WORD_QTUSER_FUNCTIONS_H

#include <argos3/plugins/simulator/visualizations/qt-opengl/qtopengl_user_functions.h>
#include <argos3/plugins/robots/foot-bot/simulator/footbot_entity.h>
#include "../../controllers/footbot_aggregation/footbot_aggregation.h"
#include <sstream>

using namespace argos;

class CWORDQTUserFunctions : public CQTOpenGLUserFunctions {

public:

   CWORDQTUserFunctions();

   virtual ~CWORDQTUserFunctions() {}

   void Draw(CFootBotEntity& c_entity);
   
};

#endif
