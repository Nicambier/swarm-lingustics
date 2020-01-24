#include "world.h"
#include "agent.h"
#include "entity.h"
#include "agentfactory.h"

#include <gridwindow.h>

using namespace std;

World::World(int x, int y, int pop, AgentFactory* factory, unsigned int seed) {
    size_x = x;
    size_y = y;
    
    this->pop = pop;
    
    t=0;
    
    map = new Entity**[size_x];
    for(int i=0; i<size_x; ++i) {
        map[i] = new Entity*[size_y];
        for(int j=0; j<size_y; ++j)
            map[i][j] = 0;
    }
    
    const gsl_rng_type * T;
    gsl_rng_env_setup();
    T = gsl_rng_default; // Generator setup
    rng = gsl_rng_alloc (T);
    if(seed==0)
        seed = time(NULL);
    gsl_rng_set(rng, seed);

    bool placed;
    Vector2D pos;
    if(size_x*size_y<pop)
        throw "Too many agents for this world.";
    for(int i=0; i<pop; ++i) {
        placed = false;
        while(!placed) {
            pos.x = random(size_x);
            pos.y = random(size_y);

            if(!isOccupied(pos.x,pos.y)) {
                agents.push_back(factory->GetNewAgent(this, pos));
                map[pos.x][pos.y] = agents.back();
                placed = true;
            }
        }
    }


}

World::~World() {
    for(int i=0; i<size_x; ++i) {
        for(int j=0; j<size_y; ++j) {
            if(isOccupied(i,j))
                delete map[i][j];
        }
        delete map[i];
    }
    delete map;
    gsl_rng_free (rng);
}
        
bool World::MoveAgentTo(Agent* a, Vector2D pos) {
    bool placed = false;
    if(pos.x >= 0 and pos.x < size_x and pos.y >= 0 and pos.y < size_y) {
        if(!isOccupied(pos.x,pos.y)) {
            map[pos.x][pos.y] = a;
            Vector2D oldPos = a->GetPos();
            map[oldPos.x][oldPos.y] = 0;
            a->SetPos(pos);
            placed = true;
        }
    }
    return placed;
}

void World::BroadcastFrom(Vector2D from, int range, uint32_t msg) {
    for(int i=from.x-range; i<=from.x+range; ++i) {
        //for(int j=from.y-(range-(from.x-i)); j<=from.y+(range-(from.x-i)); ++j) {
        for(int j=from.y-range; j<=from.y+range; ++j) {
            if(i>=0 and i<size_x and j>=0 and j<size_y and (i!=from.x or j!=from.y)) {
                if(isOccupied(i,j))
                    map[i][j]->Receive(msg);
            }
        }
    }
}

void World::Run() {
    for (list<Agent*>::iterator it=agents.begin(); it!=agents.end(); ++it) {
        (*it)->Run();
    }
    ++t;
}

bool World::isOccupied(int x, int y) {
    return map[x][y] != 0;
}

uint32_t World::CellColour(int x, int y) {
    if(isOccupied(x,y)) {
        return map[x][y]->GetColour();
    }else
        return 16777215;
}

list<Agent*> World::findCluster(list<Agent*>::iterator seed, list<Agent*>& unassigned) const
{
    list<Agent*> cluster;
    Vector2D s = (*seed)->GetPos();
    cluster.push_back(*seed);
    unassigned.erase(seed);

    float d = (*seed)->GetBroadCastRange();
    if(unassigned.size() > 0) {
        bool hasNeighbours = true;
        while(hasNeighbours) {
            hasNeighbours = false;
            for (list<Agent*>::iterator it=unassigned.begin(); it != unassigned.end(); ++it) {
                Vector2D other = (*it)->GetPos();
                if(abs(other.x - s.x)+abs(other.y - s.y) <= d) {
                    hasNeighbours = true;
                    list<Agent*> temp = findCluster(it,unassigned);
                    for (list<Agent*>::iterator it2=temp.begin(); it2 != temp.end(); ++it2)
                        cluster.push_back(*it2);
                    break;
                }
            }
        }
    }
    return cluster;
}

double World::Evaluate() {
    list<Agent*> unassigned;
    for(list<Agent*>::iterator it=agents.begin(); it != agents.end(); ++it) {
        unassigned.push_back(*it);
    }
    int totSize = 0;
    unsigned int max = 0;
    while(agents.size()-totSize > max) {
        unsigned int size = findCluster(unassigned.begin(), unassigned).size();
        if(size>max)
            max=size;
        totSize+=size;
    }
    return ((double) max)/agents.size();
}

ostream & operator<<(ostream & Str, World const & w) {
  Str<<w.t;
  for (list<Agent*>::const_iterator it=w.agents.begin(); it!=w.agents.end(); ++it) {
      Str<<" "<<**it;
  }
  return Str;
}
