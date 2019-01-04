#include "word_qtuser_functions.h"

/****************************************/
/****************************************/

CWORDQTUserFunctions::CWORDQTUserFunctions() {
   RegisterUserFunction<CWORDQTUserFunctions,CKilobotEntity>(&CWORDQTUserFunctions::Draw);
}

/****************************************/
/****************************************/

void CWORDQTUserFunctions::PreStep() {
   //words.clear();
}

void CWORDQTUserFunctions::Draw(CKilobotEntity& c_kilobot_entity) {
   /* The position of the text is expressed wrt the reference point of the footbot
    * For a foot-bot, the reference point is the center of its base.
    * See also the description in
    * $ argos3 -q foot-bot
    */
   
   CKilobotCommunicationEntity kilocomm = c_kilobot_entity.GetKilobotCommunicationEntity();
   if(kilocomm.GetTxStatus()==CKilobotCommunicationEntity::TX_SUCCESS)
        lastWord[c_kilobot_entity.GetId()] = kilocomm.GetTxMessage()->data[2];
   
   float a,b;
   bool exists=false;
   if(lastWord.find(c_kilobot_entity.GetId())!=lastWord.end()) {
       a = 1.25 + 0.25*(lastWord[c_kilobot_entity.GetId()]>>4);
       b = 1.25 + 0.25*(lastWord[c_kilobot_entity.GetId()]%16);
       exists = true;
   }
   
   if(exists) {
        std::cout<<a<<" "<<b<<std::endl;
        //char c[] = {p,'\0'};
        std::string p = std::to_string((int) (a*100)) + " " + std::to_string((int) (b*100));
        DrawText(CVector3(0.02, 0.02, 0.01),   // position
                    p.c_str()); // text
   }
}

/****************************************/
/****************************************/

REGISTER_QTOPENGL_USER_FUNCTIONS(CWORDQTUserFunctions, "word_qtuser_functions")
