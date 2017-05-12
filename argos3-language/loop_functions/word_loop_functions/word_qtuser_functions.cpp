#include "word_qtuser_functions.h"

/****************************************/
/****************************************/

CWORDQTUserFunctions::CWORDQTUserFunctions() {
   RegisterUserFunction<CWORDQTUserFunctions,CFootBotEntity>(&CWORDQTUserFunctions::Draw);
}

/****************************************/
/****************************************/

void CWORDQTUserFunctions::Draw(CFootBotEntity& c_entity) {
   /* The position of the text is expressed wrt the reference point of the footbot
    * For a foot-bot, the reference point is the center of its base.
    * See also the description in
    * $ argos3 -q foot-bot
    */
   DrawText(CVector3(0.15, 0.15, 0.1),   // position
            (static_cast<CFootBotAggregation&>(c_entity.GetControllableEntity().GetController())).GetState().c_str()); // text
}

/****************************************/
/****************************************/

REGISTER_QTOPENGL_USER_FUNCTIONS(CWORDQTUserFunctions, "word_qtuser_functions")
