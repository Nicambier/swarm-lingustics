#include "aggregation.h"

#include <algorithm>
#include <cstring>
#include <cerrno>
/****************************************/
/****************************************/

static const Real BOT_RADIUS            = 0.14f;

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
   GetNodeAttribute(t_tree, "words", m_strWordFile);
   
   GetNodeAttributeOrDefault(t_tree, "minDist", minDist, minDist);
   GetNodeAttributeOrDefault(t_tree, "timeStopCond", timeStopCond, timeStopCond);

   /* Open the file for text writing */
   m_cOutFile.open(m_strOutFile.c_str(), std::ofstream::out | std::ofstream::trunc);
   if(m_cOutFile.fail()) {
      THROW_ARGOSEXCEPTION("Error opening file \"" << m_strOutFile << "\": " << ::strerror(errno));
   }
   
   /* Open the file for words writing */
   m_cWordFile.open(m_strWordFile.c_str(), std::ofstream::out | std::ofstream::trunc);
   if(m_cWordFile.fail()) {
      THROW_ARGOSEXCEPTION("Error opening file \"" << m_strWordFile << "\": " << ::strerror(errno));
   }
   
   ////////////////////////////////////////////////////////////////////////////////// CREATION AND POSITIONING OF THE ARENA WALLS////////////////////////////////////////////////////////////////////////////////
    CVector3 arena_size = GetSpace().GetArenaSize();
    int m_fArenaRadius = Min(arena_size[0],arena_size[1])/2;
    int m_unNumArenaWalls = 20;

    CRadians wall_angle = CRadians::TWO_PI/m_unNumArenaWalls;CVector3 wall_size(0.05, 2.0*m_fArenaRadius*Tan(CRadians::PI/m_unNumArenaWalls), 0.1);
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
}

/****************************************/
/****************************************/

void CAggregation::Reset() {
   /* Close the output file */
   m_cOutFile.close();
   if(m_cOutFile.fail()) {
      THROW_ARGOSEXCEPTION("Error closing file \"" << m_strOutFile << "\": " << ::strerror(errno));
   }
   /* Open the file for text writing */
   m_cOutFile.open(m_strOutFile.c_str(), std::ofstream::out | std::ofstream::trunc);
   if(m_cOutFile.fail()) {
      THROW_ARGOSEXCEPTION("Error opening file \"" << m_strOutFile << "\": " << ::strerror(errno));
   }
   
   /* Close the output file */
   m_cWordFile.close();
   if(m_cWordFile.fail()) {
      THROW_ARGOSEXCEPTION("Error closing file \"" << m_strWordFile << "\": " << ::strerror(errno));
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
   
   m_cWordFile.close();
   if(m_cWordFile.fail()) {
      THROW_ARGOSEXCEPTION("Error closing file \"" << m_strWordFile << "\": " << ::strerror(errno));
   }
}

/****************************************/
/****************************************/

void CAggregation::PreStep() {
   /* Nothing to do */
}

/****************************************/
/****************************************/

list< pair<float,float> > CAggregation::findCluster(list< pair<float,float> >::iterator seed, list< pair<float,float> >& pos) {
    list< pair<float,float> > cluster;
    pair<float,float> s = *seed;
    cluster.push_back(s);
    pos.erase(seed);
    
    list< pair<float,float> > temp;
    float d = minDist*0.01;
    if(pos.size() > 0) {
        bool hasNeighbours = true;
        while(hasNeighbours) {
            hasNeighbours = false;
            for (list< pair<float,float> >::iterator it=pos.begin(); it != pos.end(); ++it) {
                if(sqrt(pow((it->first - s.first),2)+pow((it->second - s.second),2)) < d) {
                    hasNeighbours = true;
                    temp = findCluster(it,pos);
                    for (list< pair<float,float> >::iterator it2=temp.begin(); it2 != temp.end(); ++it2)
                        cluster.push_back(*it2);
                    break;
                }                
            }
        }
    }
    
    return cluster;
}

double CAggregation::std2D(list< pair<float,float> > pos) {
    pair<float, float> accPos = make_pair(0,0);
    for (list< pair<float,float> >::iterator it=pos.begin(); it != pos.end(); ++it) {
        accPos.first += it->first;
        accPos.second += it->second;
    }
    pair<float, float> meanPos = make_pair(accPos.first/pos.size(),accPos.second/pos.size());
    
    double accStd = 0;
    for (list< pair<float,float> >::iterator it=pos.begin(); it != pos.end(); ++it) {
        accStd += pow((it->first - meanPos.first),2)+pow((it->second - meanPos.second),2);
    }
    
    return accStd/(pos.size()*4*pow(BOT_RADIUS,2));
}

float CAggregation::connectivity(list< pair<float,float> > pos) {
    pair<float, float> accPos = make_pair(0,0);
    float tot;
    float d = minDist*0.01;

    for (list< pair<float,float> >::iterator it=pos.begin(); it != pos.end(); ++it) {
        for (list< pair<float,float> >::iterator it2=pos.begin(); it2 != pos.end(); ++it2) {
            if(sqrt(pow((it->first - it2->first),2)+pow((it->second - it2->second),2)) < d) {
                ++tot;
            }
        }
    }
    
    return tot/pos.size();
}

int CAggregation::clustersInfo(list< pair<float,float> > pos, vector<int>& sizes, vector<double>& stds) {
    pair<float,float> seed;
    list< pair<float,float> > cluster;
    while(pos.size() > 0) {
        cluster = findCluster(pos.begin(),pos);
        if(cluster.size() >= 2) {
            sizes.push_back(cluster.size());
            stds.push_back(std2D(cluster));
        }
    }
            
    return sizes.size();
}

void CAggregation::PostStep() {
    int clock = GetSpace().GetSimulationClock();
    if(clock%100==0) {
        list< pair<float,float> > positions;
        set<unsigned short int> words;
        vector<unsigned short int> lexi;
        m_cOutFile << clock << "\t";
        m_cWordFile << clock << "\t";
        CSpace::TMapPerType& cFBMap = GetSpace().GetEntitiesByType("foot-bot");
        for(CSpace::TMapPerType::iterator it = cFBMap.begin(); it != cFBMap.end(); ++it) {
            CFootBotEntity& footbotEntity = *any_cast<CFootBotEntity*>(it->second);
            CFootBotAggregation_NG& controller = static_cast<CFootBotAggregation_NG&>(footbotEntity.GetControllableEntity().GetController());
            string state = controller.GetState();
            //if(state=="STAY") {
                Real Robot_X = footbotEntity.GetEmbodiedEntity().GetOriginAnchor().Position.GetX();
                Real Robot_Y = footbotEntity.GetEmbodiedEntity().GetOriginAnchor().Position.GetY();
                positions.push_back(make_pair(Robot_X,Robot_Y));
                lexi = controller.GetLexicon();
                for(int i=0; i<lexi.size(); ++i) {
                    words.insert(lexi[i]);
                    m_cWordFile<<" "<<lexi[i];
                }
            //}
        }
        vector<int> sizes;
        vector<double> stds;
        clustersInfo(positions,sizes,stds);
        
        int sizeAcc = 0;
        double stdAcc = 0;
        int nb = sizes.size();
        for(int i=0; i<nb; ++i) {
            sizeAcc += sizes[i];
            stdAcc += stds[i];
        }

        m_cOutFile<<connectivity(positions)<<" "<<words.size()<<" "<<sizes.size()<<" "<<(float) sizeAcc/nb<<" "<<stdAcc/nb;
        for(int i=0; i<nb; ++i)
            m_cOutFile<<" "<<sizes[i];
        
        m_cOutFile<<endl;
        m_cWordFile<<endl;
    }
}

bool CAggregation::IsExperimentFinished() {
    if(timeStopCond > 0) {
        bool stabilised = true;
        CSpace::TMapPerType& cFBMap = GetSpace().GetEntitiesByType("foot-bot");
                    for(CSpace::TMapPerType::iterator it = cFBMap.begin(); it != cFBMap.end() and stabilised; ++it) {
                CFootBotEntity& footbotEntity = *any_cast<CFootBotEntity*>(it->second);
                            CFootBotAggregation& footbot = static_cast<CFootBotAggregation&>(footbotEntity.GetControllableEntity().GetController());
                            if(footbot.LastMove() < timeStopCond)
                    stabilised = false;
                    }
        return stabilised;
    }
}

/****************************************/
/****************************************/

/* Register this loop functions into the ARGoS plugin system */
REGISTER_LOOP_FUNCTIONS(CAggregation, "aggregation_loop_functions");
