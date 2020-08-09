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
   
   /*GetNodeAttributeOrDefault(t_tree, "aParam", a, a);
   GetNodeAttributeOrDefault(t_tree, "bParam", b, b);
   
   GetNodeAttributeOrDefault(t_tree, "link", link, link);
   
   GetNodeAttributeOrDefault(t_tree, "mutation", m, m);*/

   /* Open the file for text writing */
   m_cOutFile.open(m_strOutFile.c_str(), std::ofstream::out | std::ofstream::trunc);
   if(m_cOutFile.fail()) {
      THROW_ARGOSEXCEPTION("Error opening file \"" << m_strOutFile << "\": " << ::strerror(errno));
   }
   
   unsigned int nBots;
   GetNodeAttributeOrDefault(t_tree, "nBots", nBots, nBots);
   double cornerProportion;
   GetNodeAttributeOrDefault(t_tree, "cornerProportion", cornerProportion, cornerProportion);
   
   ////////////////////////////////////////////////////////////////////////////////// CREATION AND POSITIONING OF THE ARENA WALLS////////////////////////////////////////////////////////////////////////////////
    CVector3 arena_size = GetSpace().GetArenaSize();

    unsigned int cornerWalls = 5;
    double cornerRadius = cornerProportion * Min(arena_size[0],arena_size[1]);
    
    CQuaternion wall_orientation;
    wall_orientation.FromEulerAngles(CRadians::ZERO, CRadians::ZERO, CRadians::ZERO );
    CBoxEntity* box = new CBoxEntity("west_wall", CVector3(0,-arena_size[1]/2,0), wall_orientation, false, CVector3(arena_size[0]-2*cornerRadius,0.01,0.05), (Real)1.0 );
    AddEntity( *box );
    box = new CBoxEntity("east_wall", CVector3(0,arena_size[1]/2,0), wall_orientation, false, CVector3(arena_size[0]-2*cornerRadius,0.01,0.05), (Real)1.0 );
    AddEntity( *box );
    box = new CBoxEntity("south_wall", CVector3(-arena_size[0]/2,0,0), wall_orientation, false, CVector3(0.01,arena_size[1]-2*cornerRadius,0.05), (Real)1.0 );
    AddEntity( *box );
    box = new CBoxEntity("north_wall", CVector3(arena_size[0]/2,0,0), wall_orientation, false, CVector3(0.01,arena_size[1]-2*cornerRadius,0.05), (Real)1.0 );
    AddEntity( *box );

    PlaceQuarterCircleWall(CVector3(arena_size[0]/2-cornerRadius,arena_size[1]/2-cornerRadius,0),cornerRadius,cornerWalls,0);
    PlaceQuarterCircleWall(CVector3(-arena_size[0]/2+cornerRadius,arena_size[1]/2-cornerRadius,0),cornerRadius,cornerWalls,1);
    PlaceQuarterCircleWall(CVector3(-arena_size[0]/2+cornerRadius,-arena_size[1]/2+cornerRadius,0),cornerRadius,cornerWalls,2);
    PlaceQuarterCircleWall(CVector3(arena_size[0]/2-cornerRadius,-arena_size[1]/2+cornerRadius,0),cornerRadius,cornerWalls,3);
    
    CKilobotEntity* pcKB;
    stayArray = new bool[nBots];
    for(unsigned int i=0; i<nBots; ++i) {  
        stayArray[i]=false;
        pcKB = new CKilobotEntity("kb_" + ToString(i),"kbc",CVector3(0,0,0),CQuaternion(0,0,0,0),0.07);
        bots.push_back(pcKB);
        AddEntity(*pcKB);
    }
    
    PlaceBots(arena_size, cornerRadius);
    //ConfigBots();
}

void CAggregation::PlaceQuarterCircleWall(CVector3 pos, double radius, unsigned int nbWalls, unsigned int quadrant) {
    CRadians wall_angle = CRadians::TWO_PI/(4*nbWalls);CVector3 wall_size(0.01, 2.0*radius*Tan(CRadians::PI/(4*nbWalls)), 0.05);
    ostringstream entity_id;
    for( UInt32 i = quadrant*nbWalls; i <= quadrant*nbWalls + nbWalls; i++ ) {
        entity_id.str("");entity_id << "wall_" <<quadrant<< i;
        CRadians wall_rotation = wall_angle*i;
        
        CVector3 wall_position(pos[0] + radius*Cos(wall_rotation), pos[1] + radius*Sin(wall_rotation), 0 );
        CQuaternion wall_orientation;
        wall_orientation.FromEulerAngles( wall_rotation, CRadians::ZERO, CRadians::ZERO );
        
        CBoxEntity* box = new CBoxEntity(entity_id.str(), wall_position, wall_orientation, false, wall_size, (Real)1.0 );
        AddEntity( *box );
    }
}

void CAggregation::PlaceBots(CVector3 arenaSize, double cornerRadius) {
    
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
            double x = m_pcRNG->Uniform(CRange<Real>(-arenaSize[0]/2, arenaSize[0]/2));
            double y = m_pcRNG->Uniform(CRange<Real>(-arenaSize[1]/2, arenaSize[1]/2));
            if(abs(x)<arenaSize[0]/2-cornerRadius or abs(y)<arenaSize[1]/2-cornerRadius or Distance(CVector3(abs(x),abs(y),0),CVector3(arenaSize[0]/2-cornerRadius,arenaSize[1]/2-cornerRadius,0))<cornerRadius) {
                cPosition.SetX(x);
                cPosition.SetY(y);

                CRadians cRandomOrientation = CRadians(m_pcRNG->Uniform(CRange<Real>(-CRadians::PI.GetValue(), CRadians::PI.GetValue())));
                cOrientation.FromEulerAngles(cRandomOrientation, CRadians::ZERO, CRadians::ZERO);

                bDone = MoveEntity(pcKB->GetEmbodiedEntity(), cPosition, cOrientation);
            }
            ++unTrials;
        } while(!bDone && unTrials <= MAX_PLACE_TRIALS);
        if(!bDone) {
            THROW_ARGOSEXCEPTION("Can't place " << "kb_" + ToString(i));
        }
    }
    
}

/*void CAggregation::ConfigBots() {
    CKilobotEntity* kbEntity;    
    
    for(unsigned int i=0; i<bots.size(); ++i) {        
        kbEntity = bots[i];
        CKilobotCommunicationEntity kilocomm = kbEntity->GetKilobotCommunicationEntity();
        message_t msg = *(new message_t());
        msg.type = NORMAL;
        msg.data[0] = CONFIG;
        //PARAMS
        msg.data[1] = link;
        //msg.data[2] = (((uint8_t) round((a)*4))<<4) + (uint8_t) round((b)*4);
        msg.data[2] = (((uint8_t) round((a-1.25)*4))<<4) + (uint8_t) round((b-1.25)*4);
        msg.data[4] = double_to_uint8(m);
        msg.crc = 0;//message_crc(&msg);
        GetSimulator().GetMedium<CKilobotCommunicationMedium>("kilocomm").SendOHCMessageTo(*kbEntity,&msg);
    }
    
}*/

/****************************************/
/****************************************/

void CAggregation::Reset() {
    //PlaceBots();
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

/*void CAggregation::PreStep() {
    int clock = GetSpace().GetSimulationClock();
    if(clock<20)
        ConfigBots();
}*/

void CAggregation::PostStep() {
    int recordSteps = 100;
    int clock = GetSpace().GetSimulationClock();
    if(clock%recordSteps==0) 
        m_cOutFile << clock << " ";
        
    for(unsigned int i=0; i<bots.size(); ++i) {
        CKilobotEntity& kbEntity = *any_cast<CKilobotEntity*>(bots[i]);
        CKilobotCommunicationEntity kilocomm = kbEntity.GetKilobotCommunicationEntity();
        if(kilocomm.GetTxStatus()==CKilobotCommunicationEntity::TX_SUCCESS)
            stayArray[i] = kilocomm.GetTxMessage()->data[0]!=0;
        
        int clock = GetSpace().GetSimulationClock();
        if(clock%recordSteps==0) {
            Real Robot_X = kbEntity.GetEmbodiedEntity().GetOriginAnchor().Position.GetX();
            Real Robot_Y = kbEntity.GetEmbodiedEntity().GetOriginAnchor().Position.GetY();
            m_cOutFile << "( " << Robot_X << " , " << Robot_Y << " , " << stayArray[i] << ") ";
        }
    }
    
    if(clock%recordSteps==0)
        m_cOutFile<<endl;
}

/****************************************/
/****************************************/

/* Register this loop functions into the ARGoS plugin system */
REGISTER_LOOP_FUNCTIONS(CAggregation, "aggregation_loop_functions");
