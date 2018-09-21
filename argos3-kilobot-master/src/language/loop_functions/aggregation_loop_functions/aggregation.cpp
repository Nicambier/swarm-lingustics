#include "aggregation.h"

#include <algorithm>
#include <cstring>
#include <cerrno>
/****************************************/
/****************************************/
using namespace std;

static const Real BOT_RADIUS            = 0.033f;
static const UInt32 MAX_PLACE_TRIALS = 20;

/****************************************/
/****************************************/

CAggregation::CAggregation() {
}

/****************************************/
/****************************************/

CAggregation::~CAggregation() {
   /* Nothing to do */
}

/****************************************/
/****************************************/

void CAggregation::Init(TConfigurationNode& t_tree) {
   /* Get output file name from XML tree */
   GetNodeAttribute(t_tree, "output", m_strOutFile);
   GetNodeAttributeOrDefault(t_tree, "timeStopCond", timeStopCond, timeStopCond);
   
   int nBots;
   GetNodeAttributeOrDefault(t_tree, "nBots", nBots, nBots);
   
   GetNodeAttributeOrDefault(t_tree, "aParam", a, a);
   GetNodeAttributeOrDefault(t_tree, "bParam", b, b);
   
   GetNodeAttributeOrDefault(t_tree, "link", link, link);

   /* Open the file for text writing */
   m_cOutFile.open(m_strOutFile.c_str(), std::ofstream::out | std::ofstream::trunc);
   if(m_cOutFile.fail()) {
      THROW_ARGOSEXCEPTION("Error opening file \"" << m_strOutFile << "\": " << ::strerror(errno));
   }
   
   ////////////////////////////////////////////////////////////////////////////////// CREATION AND POSITIONING OF THE ARENA WALLS////////////////////////////////////////////////////////////////////////////////
    CVector3 arena_size = GetSpace().GetArenaSize();
    float m_fArenaRadius = Min(arena_size[0],arena_size[1])/2;
    unsigned int m_unNumArenaWalls = 20;

    CRadians wall_angle = CRadians::TWO_PI/m_unNumArenaWalls;CVector3 wall_size(0.01, 2.0*m_fArenaRadius*Tan(CRadians::PI/m_unNumArenaWalls), 0.05);
    ostringstream entity_id;
    for( UInt32 i = 0; i < m_unNumArenaWalls; i++ ) {
        entity_id.str("");entity_id << "wall_" << i;
        CRadians wall_rotation = wall_angle*i;
        
        CVector3 wall_position( m_fArenaRadius*Cos(wall_rotation), m_fArenaRadius*Sin(wall_rotation), 0 );
        CQuaternion wall_orientation;
        wall_orientation.FromEulerAngles( wall_rotation, CRadians::ZERO, CRadians::ZERO );
        
        CBoxEntity* box = new CBoxEntity(entity_id.str(), wall_position, wall_orientation, false, wall_size, (Real)1.0 );
        AddEntity( *box );
    }
    
    CKilobotEntity* pcKB;
    for(int i=0; i<nBots; ++i) {        
        pcKB = new CKilobotEntity("kb_" + ToString(i),"kbc",CVector3(0,0,0),CQuaternion(0,0,0,0),0.07);
        bots.push_back(pcKB);
        AddEntity(*pcKB);
    }
    
    PlaceBots();
    ConfigBots();
}

void CAggregation::PlaceBots() {
    CVector3 arena_size = GetSpace().GetArenaSize();
    float m_fArenaRadius = Min(arena_size[0],arena_size[1])/2;
    
    CVector3 cPosition;
    CQuaternion cOrientation;
    cPosition.SetZ(0.0);
    CRandom::CRNG* m_pcRNG = CRandom::CreateRNG("argos");
    unsigned int unTrials;
    CKilobotEntity* pcKB;
    for(unsigned int i=0; i<bots.size(); ++i) {
        bool bDone = false;
        unTrials = 0;
        
        pcKB = bots[i];
        do {
            CRadians cRandomAngle = CRadians(m_pcRNG->Uniform(CRange<Real>(-CRadians::PI.GetValue(), CRadians::PI.GetValue())));
            Real cRandomRadius = m_pcRNG->Uniform(CRange<Real>(-m_fArenaRadius,m_fArenaRadius));

            cPosition.SetX(cRandomRadius * Cos(cRandomAngle));
            cPosition.SetY(cRandomRadius * Sin(cRandomAngle));

            CRadians cRandomOrientation = CRadians(m_pcRNG->Uniform(CRange<Real>(-CRadians::PI.GetValue(), CRadians::PI.GetValue())));
            cOrientation.FromEulerAngles(cRandomOrientation, CRadians::ZERO, CRadians::ZERO);

            bDone = MoveEntity(pcKB->GetEmbodiedEntity(), cPosition, cOrientation);
            ++unTrials;
        } while(!bDone && unTrials <= MAX_PLACE_TRIALS);
        if(!bDone) {
            THROW_ARGOSEXCEPTION("Can't place " << "kb_" + ToString(i));
        }
    }
    
}

void CAggregation::ConfigBots() {
    CKilobotEntity* kbEntity;
    for(unsigned int i=0; i<bots.size(); ++i) {        
        kbEntity = bots[i];
        CKilobotCommunicationEntity kilocomm = kbEntity->GetKilobotCommunicationEntity();
        message_t msg = *(new message_t());
        msg.type = NORMAL;
        msg.data[2] = CONFIG;
        //PARAMS
        msg.data[0] = round((a-1.25)*4)*16 + round((b-1.25)*4);
        msg.data[1] = link;
        msg.crc = 0;//message_crc(&msg);
        GetSimulator().GetMedium<CKilobotCommunicationMedium>("kilocomm").SendOHCMessageTo(*kbEntity,&msg);
    }
    
}

/****************************************/
/****************************************/

void CAggregation::Reset() {
    PlaceBots();
   /* Close the output file */
   m_cOutFile.close();
   if(m_cOutFile.fail()) {
      THROW_ARGOSEXCEPTION("Error closing file \"" << m_strOutFile << "\": " << ::strerror(errno));
   }
   
   /* Open the file for text writing */
   m_cWordFile.open(m_strWordFile.c_str(), std::ofstream::out | std::ofstream::trunc);
   if(m_cWordFile.fail()) {
      THROW_ARGOSEXCEPTION("Error opening file \"" << m_strWordFile << "\": " << ::strerror(errno));
   }
}

/****************************************/
/****************************************/

void CAggregation::Destroy() {
   /* Close the output file */
   m_cOutFile.close();
   if(m_cOutFile.fail()) {
      THROW_ARGOSEXCEPTION("Error closing file \"" << m_strOutFile << "\": " << ::strerror(errno));
   }
}

/****************************************/
/****************************************/

void CAggregation::PreStep() {
   /* Nothing to do */
}

void CAggregation::PostStep() {
    int clock = GetSpace().GetSimulationClock();
    if(clock%100==0) {
        m_cOutFile << clock << " ";
        for(unsigned int i=0; i<bots.size(); ++i) {
            CKilobotEntity& kbEntity = *any_cast<CKilobotEntity*>(bots[i]);
            Real Robot_X = kbEntity.GetEmbodiedEntity().GetOriginAnchor().Position.GetX();
            Real Robot_Y = kbEntity.GetEmbodiedEntity().GetOriginAnchor().Position.GetY();
            CKilobotCommunicationEntity kilocomm = kbEntity.GetKilobotCommunicationEntity();
            unsigned char word;
            if(kilocomm.GetTxStatus()==CKilobotCommunicationEntity::TX_SUCCESS)
                word = kilocomm.GetTxMessage()->data[0];
            
            m_cOutFile << "( " << Robot_X << " , " << Robot_Y << " , " << ((unsigned int) word) << ") ";
        }
        m_cOutFile<<endl;
    }
}

/****************************************/
/****************************************/

/* Register this loop functions into the ARGoS plugin system */
REGISTER_LOOP_FUNCTIONS(CAggregation, "aggregation_loop_functions");
