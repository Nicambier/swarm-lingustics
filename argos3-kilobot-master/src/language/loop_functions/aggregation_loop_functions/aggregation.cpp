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
   GetNodeAttribute(t_tree, "words", m_strWordFile);
   
   GetNodeAttributeOrDefault(t_tree, "minDist", minDist, minDist);
   GetNodeAttributeOrDefault(t_tree, "timeStopCond", timeStopCond, timeStopCond);
   
   int nBots;
   GetNodeAttributeOrDefault(t_tree, "nBots", nBots, nBots);

   /* Open the file for text writing */
   /*m_cOutFile.open(m_strOutFile.c_str(), std::ofstream::out | std::ofstream::trunc);
   if(m_cOutFile.fail()) {
      THROW_ARGOSEXCEPTION("Error opening file \"" << m_strOutFile << "\": " << ::strerror(errno));
   }*/
   
   /* Open the file for words writing */
   /*m_cWordFile.open(m_strWordFile.c_str(), std::ofstream::out | std::ofstream::trunc);
   if(m_cWordFile.fail()) {
      THROW_ARGOSEXCEPTION("Error opening file \"" << m_strWordFile << "\": " << ::strerror(errno));
   }*/
   
   ////////////////////////////////////////////////////////////////////////////////// CREATION AND POSITIONING OF THE ARENA WALLS////////////////////////////////////////////////////////////////////////////////
    CVector3 arena_size = GetSpace().GetArenaSize();
    float m_fArenaRadius = Min(arena_size[0],arena_size[1])/2;
    int m_unNumArenaWalls = 20;

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
}

void CAggregation::PlaceBots() {
    CVector3 arena_size = GetSpace().GetArenaSize();
    float m_fArenaRadius = Min(arena_size[0],arena_size[1])/2;
    
    CVector3 cPosition;
    CQuaternion cOrientation;
    cPosition.SetZ(0.0);
    CRandom::CRNG* m_pcRNG = CRandom::CreateRNG("argos");
    int unTrials;
    CKilobotEntity* pcKB;
    for(int i=0; i<bots.size(); ++i) {
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

/****************************************/
/****************************************/

void CAggregation::Reset() {
    PlaceBots();
   /* Close the output file */
   /*m_cOutFile.close();
   if(m_cOutFile.fail()) {
      THROW_ARGOSEXCEPTION("Error closing file \"" << m_strOutFile << "\": " << ::strerror(errno));
   }*/
   /* Open the file for text writing */
   /*m_cOutFile.open(m_strOutFile.c_str(), std::ofstream::out | std::ofstream::trunc);
   if(m_cOutFile.fail()) {
      THROW_ARGOSEXCEPTION("Error opening file \"" << m_strOutFile << "\": " << ::strerror(errno));
   }*/
   
   /* Close the output file */
   /*m_cWordFile.close();
   if(m_cWordFile.fail()) {
      THROW_ARGOSEXCEPTION("Error closing file \"" << m_strWordFile << "\": " << ::strerror(errno));
   }*/
   /* Open the file for text writing */
   /*m_cWordFile.open(m_strWordFile.c_str(), std::ofstream::out | std::ofstream::trunc);
   if(m_cWordFile.fail()) {
      THROW_ARGOSEXCEPTION("Error opening file \"" << m_strWordFile << "\": " << ::strerror(errno));
   }*/
}

/****************************************/
/****************************************/

void CAggregation::Destroy() {
   /* Close the output file */
   /*m_cOutFile.close();
   if(m_cOutFile.fail()) {
      THROW_ARGOSEXCEPTION("Error closing file \"" << m_strOutFile << "\": " << ::strerror(errno));
   }
   
   m_cWordFile.close();
   if(m_cWordFile.fail()) {
      THROW_ARGOSEXCEPTION("Error closing file \"" << m_strWordFile << "\": " << ::strerror(errno));
   }*/
}

/****************************************/
/****************************************/

void CAggregation::PreStep() {
   /* Nothing to do */
}

/****************************************/
/****************************************/

/*list< pair<float,float> > CAggregation::findCluster(list< pair<float,float> >::iterator seed, list< pair<float,float> >& pos) {
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
                if(sqrt(pow((it->first - s.first),2)+pow((it->second - s.second),2)) <= d) {
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

list< pair<float,float> > CAggregation::findCluster2(list< pair<float,float> >::iterator seed, list< pair<float,float> >& pos) {
    list< pair<float,float> > cluster;
    float d = minDist*0.01;
    cluster.push_back(*seed);
    pos.erase(seed);
    
    list< pair<float,float> >::iterator s2;
    bool s2Found = false;
    //find first neighbour
    for (list< pair<float,float> >::iterator it=pos.begin(); it != pos.end(); ++it) {
        if(sqrt(pow((it->first - seed->first),2)+pow((it->second - seed->second),2)) <= d) {
            cluster.push_back(*it);
            pos.erase(it);
            s2Found = true;
            break;
        }
    }
    
    if(s2Found) {    
        //find all bots with two neighbours already in the cluster
        int n;
        bool updated = true;
        while(updated) {
            updated=false;
            for (list< pair<float,float> >::iterator it=pos.begin(); it != pos.end(); ++it) {
                n=0;
                for (list< pair<float,float> >::iterator itClust=cluster.begin(); itClust != cluster.end(); ++itClust) {
                    if(sqrt(pow((it->first - itClust->first),2)+pow((it->second - itClust->second),2)) <= d) {
                        ++n;
                        if(n==2) break;
                    }
                }
                
                if(n>=2) { //this bot has at least two neighbours in the current cluster
                    updated=true;                    
                    cluster.push_back(*it);
                    pos.erase(it);
                    break;
                }
            }
        }
    }
    
    if(cluster.size()<3) {
        pos.push_back(cluster.front());//make the list shift left
        cluster.pop_front();  
        if(s2Found) {
            pos.push_front(cluster.front());
            cluster.pop_front();
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
            if(sqrt(pow((it->first - it2->first),2)+pow((it->second - it2->second),2)) <= d) {
                ++tot;
            }
        }
    }
    
    return tot/pos.size();
}

int CAggregation::clustersInfo(list< pair<float,float> > pos, vector<int>& sizes, vector<double>& stds) {
    pair<float,float> seed;
    list< pair<float,float> > cluster;
    list< pair<float,float> > cluster2;
    list< pair<float,float> > pos2;
    for (list< pair<float,float> >::iterator it=pos.begin(); it != pos.end(); ++it)
        pos2.push_back(make_pair(it->first,it->second));
    int c2=0;
    while(pos.size() > 0) {
        cluster = findCluster(pos.begin(),pos);
        if(cluster.size() >= 2) {
            sizes.push_back(cluster.size());
            stds.push_back(std2D(cluster));
        }
    }
    
    vector<int> sizes2;
    int res;
    int fails=0;
    while(fails<pos2.size()) {
        res = findCluster2(pos2.begin(),pos2).size();
        if(res!=0) {
            sizes2.push_back(res); 
            fails=0;
        }
        else
            ++fails;        
    }
    
    LOG<<sizes.size()<<" "<<sizes2.size()<<"\n";
    for (vector<int>::iterator it=sizes.begin(); it != sizes.end(); ++it)
        LOG<<*it<<" ";
    LOG<<"\n";
    for (vector<int>::iterator it=sizes2.begin(); it != sizes2.end(); ++it)
        LOG<<*it<<" ";
    LOG<<"\n\n";
        
    
            
    return sizes.size();
}*/

void CAggregation::PostStep() {
    /*int clock = GetSpace().GetSimulationClock();
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
    }*/
}

/****************************************/
/****************************************/

/* Register this loop functions into the ARGoS plugin system */
REGISTER_LOOP_FUNCTIONS(CAggregation, "aggregation_loop_functions");
