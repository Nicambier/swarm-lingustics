#ifndef WORD_QTUSER_FUNCTIONS_H
#define WORD_QTUSER_FUNCTIONS_H

#include <argos3/plugins/simulator/visualizations/qt-opengl/qtopengl_user_functions.h>
#include <argos3/plugins/robots/kilobot/simulator/kilobot_entity.h>
#include <argos3/plugins/robots/kilobot/simulator/kilobot_communication_entity.h>
#include <argos3/plugins/robots/kilobot/control_interface/kilolib.h>
#include <argos3/plugins/robots/kilobot/control_interface/message_crc.h>
#include <argos3/plugins/robots/kilobot/control_interface/message.h>
#include <map>
#include <string>
#include <sstream>
#include <set>

using namespace argos;

class CWORDQTUserFunctions : public CQTOpenGLUserFunctions {

public:

   CWORDQTUserFunctions();

   virtual ~CWORDQTUserFunctions() {}
   
   virtual void PreStep();

   void Draw(CKilobotEntity& c_kilobot_entity);
   
private:
    
    std::map<std::string,uint8_t> lastWord;
    
   
};

#endif
