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
   if(kilocomm.GetTxStatus()==CKilobotCommunicationEntity::TX_SUCCESS) {
        const message_t* temp = kilocomm.GetTxMessage();
        if(temp->data[0]==WORD)
            lastWord[c_kilobot_entity.GetId()] = temp;
   }
   
   uint8_t a;
   uint8_t b;
   uint8_t c;
   uint8_t n;
   bool exists=false;
   if(lastWord.find(c_kilobot_entity.GetId())!=lastWord.end()) {
       const message_t* m = lastWord[c_kilobot_entity.GetId()];
       if(m->data[0]==WORD){
           a = m->data[2];
           b = m->data[3];
           c = m->data[4];
           n = m->data[6];
           exists = true;
       }
   }
   
   if(exists) {
        //std::cout<<a<<" "<<b<<std::endl;
        //char c[] = {p,'\0'};
        std::string p = std::to_string(c) + " " + std::to_string(a) + " " + std::to_string(b) + " "+ std::to_string(n);
        DrawText(CVector3(0.02, 0.02, 0.01),   // position
                    p.c_str()); // text
   }
}

/****************************************/
/****************************************/

REGISTER_QTOPENGL_USER_FUNCTIONS(CWORDQTUserFunctions, "word_qtuser_functions")
