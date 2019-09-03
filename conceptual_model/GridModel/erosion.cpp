#include "erosion.h"
#include "agent.h"

using namespace std;

Erosion::Erosion(int x, int y, int pop, AgentFactory* factory, unsigned int seed): World(x,y,pop,factory,seed)
{
    Vector2D pos;
    Vector2D centre = {x/2,y/2};
    int sqrtPop = sqrt(pop);

    //cleanup grid
    for(int i=0;i<size_x;++i)
        for(int j=0;j<size_y;++j)
            map[i][j] = 0;

    int i=0;
    for (list<Agent*>::iterator it=agents.begin(); it!=agents.end(); ++it) {
        pos.x = centre.x + ((i%sqrtPop) - (sqrtPop/2));
        pos.y = centre.y + ((i/sqrtPop) - (sqrtPop/2));

        map[pos.x][pos.y] = *it;
        (*it)->SetPos(pos);
        ++i;
    }
}

bool Erosion::MoveAgentTo(Agent *a, Vector2D pos)
{
    if(pos.x < 0 or pos.x >= size_x or pos.y < 0 or pos.y >= size_y) {
        Vector2D oldPos = a->GetPos();
        map[oldPos.x][oldPos.y] = 0;
        toDelete.push_back(a);
        return true;
    }
    else return World::MoveAgentTo(a,pos);
}

void Erosion::Run()
{
    World::Run();
    for (list<Agent*>::iterator it=toDelete.begin(); it!=toDelete.end(); ++it) {
        list<Agent*>::iterator it2;
        for (it2=agents.begin(); *it2!=*it; ++it2) {}
        agents.erase(it2);
        --pop;
    }
    toDelete.clear();
}
