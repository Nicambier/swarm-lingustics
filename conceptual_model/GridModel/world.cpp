#include "world.h"
#include "agent.h"
#include "agentfactory.h"

#include <gridwindow.h>

using namespace std;

World::World(int x, int y, int pop, AgentFactory* factory) {
    size_x = x;
    size_y = y;
    
    this->pop = pop;
    
    t=0;
    
    map = new Agent**[size_x];
    for(int i=0; i<size_x; ++i) {
        map[i] = new Agent*[size_y];
        for(int j=0; j<size_y; ++j)
            map[i][j] = 0;
    }
    
    srand (time(NULL));
    bool placed;
    Vector2D pos;
    for(int i=0; i<pop; ++i) {
        placed = false;
        while(!placed) {
            pos.x = rand()%size_x;
            pos.y = rand()%size_y;
            
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

void World::BroadcastFrom(Vector2D from, int range, uint16_t msg) {
    for(int i=from.x-range; i<=from.x+range; ++i) {
        for(int j=from.y-(range-abs(from.x-i)); j<=from.y+(range-abs(from.x-i)); ++j) {
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

uint16_t World::CellColour(int x, int y) {
    if(isOccupied(x,y)) {
        return map[x][y]->GetColour();
    }else
        return 255;
}

ostream & operator<<(ostream & Str, World const & w) {
  Str<<w.t;
  for (list<Agent*>::const_iterator it=w.agents.begin(); it!=w.agents.end(); ++it) {
      Str<<" "<<**it;
  }
  return Str;
}
