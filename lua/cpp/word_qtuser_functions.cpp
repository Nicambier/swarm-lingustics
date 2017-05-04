#include "word_qtuser_functions.h"
#include <argos3/core/wrappers/lua/lua_controller.h>
 
/****************************************/
/****************************************/
 
CWordQTUserFunctions::CWordQTUserFunctions() {
   RegisterUserFunction<CWordQTUserFunctions,CFootBotEntity>(&CWordQTUserFunctions::Draw);
}
 
/****************************************/
/****************************************/
 
void CWordQTUserFunctions::Draw(CFootBotEntity& c_entity) {  
   /* Disable lighting, so it does not interfere with the chosen text color */
   glDisable(GL_LIGHTING);
   /* Disable face culling to be sure the text is visible from anywhere */
   glDisable(GL_CULL_FACE);
   /* Set the text color */
   CColor cColor(CColor::BLACK);
   glColor3ub(cColor.GetRed(), cColor.GetGreen(), cColor.GetBlue());
   /* The position of the text is expressed wrt the reference point of the footbot
    * For a foot-bot, the reference point is the center of its base.
    * See also the description in
    * $ argos3 -q foot-bot
    */
   
   /* Get reference to robot controller */
   CLuaController& cContr = dynamic_cast<CLuaController&>(c_entity.GetControllableEntity().GetController());
   /* Get getWord function */
   lua_getglobal(cContr.GetLuaState(), "getWord");
   /* call the function with 0 arguments, return 1 result */
   lua_call(cContr.GetLuaState(), 0, 1);
   
   //lua_getglobal(cContr.GetLuaState(), "current_state");
   
   GetQTOpenGLWidget().renderText(0.15, 0.15, 0.1,             // position
                                lua_tostring(cContr.GetLuaState(), -1)); // text
   
   /* Clean up Lua stack */
   lua_pop(cContr.GetLuaState(), 1);
   
   /* Restore face culling */
   glEnable(GL_CULL_FACE);
   /* Restore lighting */
   glEnable(GL_LIGHTING);
}
 
/****************************************/
/****************************************/
 
REGISTER_QTOPENGL_USER_FUNCTIONS(CWordQTUserFunctions, "word_qtuser_functions")