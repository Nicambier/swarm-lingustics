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
        lastWord[c_kilobot_entity.GetId()] = kilocomm.GetTxMessage()->data[0];
   
   unsigned char p;
   bool exists=false;
   if(lastWord.find(c_kilobot_entity.GetId())!=lastWord.end()) {
       p = lastWord[c_kilobot_entity.GetId()];
       exists = true;
   }
   
   if(exists) {
        //std::cout<<p<<" "<<(1.25+0.25*(p&(unsigned char) 15))<<" "<<(1.25+0.25*(p>>4))<<std::endl;
       std::cout<<p<<std::endl;
        char c[] = {p,'\0'};
        
        DrawText(CVector3(0.02, 0.02, 0.01),   // position
                    std::to_string(p).c_str()); // text
   }
}

/****************************************/
/****************************************/

REGISTER_QTOPENGL_USER_FUNCTIONS(CWORDQTUserFunctions, "word_qtuser_functions")
