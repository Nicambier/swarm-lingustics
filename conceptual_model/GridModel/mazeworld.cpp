#include "mazeworld.h"
#include "agent.h"
#include "entity.h"
#include "agentfactory.h"

using namespace std;

int vectorCount(std::vector<Vector2D> v, Vector2D p)
{
    int c = 0;
    for(int i=0; i<v.size(); ++i)
        if(v[i].x==p.x and v[i].y==p.y)
            ++c;
    return c;
}

MazeWorld::MazeWorld(int x, int y, int pop, AgentFactory* factory, int tile_size, float free_ratio, unsigned int seed): World(x,y,pop,factory,seed)
{
    /*size_x = x;
    size_y = y;

    this->pop = pop;

    t=0;

    map = new Agent**[size_x];
    for(int i=0; i<size_x; ++i) {
        map[i] = new Agent*[size_y];
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
    }*/
    Vector2D pos;
    for(int i=0; i<size_x; ++i) {
        for(int j=0; j<size_x; ++j) {
            pos.x=i;pos.y=j;
            map[i][j] = new Entity(this,pos);
        }
    }


    int area = x*y;
    int digged = 0;
    int tile_area = tile_size * tile_size;
    vector<Vector2D> dig_points;
    pos.x = random(size_x/tile_size - 1)*tile_size;
    pos.y = random(size_y/tile_size - 1)*tile_size;
    dig_points.push_back(pos);
    Vector2D dig;
    while(digged/(float) area <= free_ratio and dig_points.size()>0) {
        //take a random element prioritising elements with fewer occurences
        bool found = false;
        int idx;
        while(!found) {
            idx = random(dig_points.size());
            int idx_occ = vectorCount(dig_points,dig_points[idx]);
            int min_occ = idx_occ;
            if(idx_occ>1)
                for(int i=0; i<dig_points.size() and idx_occ<=min_occ; ++i) {
                    int temp = vectorCount(dig_points,dig_points[i]);
                    if(temp<min_occ) min_occ=temp;
                }

            if(idx_occ == min_occ) {
                found=true;
                dig = dig_points[idx];
                dig_points.erase(dig_points.begin() + idx);
            }
        }

        //cout<<dig.x<<","<<dig.y<<endl;

        //dig a tile there
        for(int i=dig.x; i<dig.x+tile_size; ++i) {
            for(int j=dig.y; j<dig.y+tile_size; ++j) {
                if(i<size_x and j<size_y) {
                    delete map[i][j];
                    map[i][j] = 0;
                }
            }
        }
        //cout<<dig.x<<","<<dig.y<<endl;
        //add concomitant points as possible digging points
        for(int i=-tile_size; i<=tile_size; i+=2*tile_size) {
            if(dig.x+i>=0 and dig.x+i<size_x and isOccupied(dig.x+i,dig.y)) {
                pos.x=dig.x+i; pos.y=dig.y;
                dig_points.push_back(pos);
            }
        }

        for(int j=-tile_size; j<=tile_size; j+=2*tile_size) {
            if(dig.y+j>=0 and dig.y+j<size_y and isOccupied(dig.x,dig.y+j)) {
                pos.x=dig.x; pos.y=dig.y+j;
                dig_points.push_back(pos);
            }
        }

        digged+=tile_area;
    }

    //replace agents
    for(list<Agent*>::iterator it = agents.begin(); it!=agents.end(); ++it) {
        bool placed = false;
        while(!placed) {
            pos.x = random(size_x);
            pos.y = random(size_y);

            if(!isOccupied(pos.x,pos.y)) {
                map[pos.x][pos.y] = *it;
                (*it)->SetPos(pos);
                placed = true;
            }
        }
    }
}
