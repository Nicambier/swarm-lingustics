#pragma once

#include <iostream>
#include <cmath> 
#include <list>
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdint.h>
#include <gsl/gsl_rng.h>


class AgentFactory;
class Agent;
class Entity;
struct Vector2D;

class World {    
    public:
        
        World(int x, int y, int pop, AgentFactory* factory, unsigned int seed=0);
        ~World();
        
        virtual bool MoveAgentTo(Agent* a, Vector2D pos);
        
        void BroadcastFrom(Vector2D from, int range, uint32_t msg);
        virtual void Run();

        bool isOccupied(int x, int y);
        uint32_t CellColour(int x, int y);

        int GetSizeX() {return size_x;}
        int GetSizeY() {return size_y;}
        int GetTime() {return t;}

        inline double uniform() const {return gsl_rng_uniform(rng);}
        inline unsigned long int random(unsigned long int higher_bound) const {return gsl_rng_uniform_int(rng,higher_bound);}

        double Evaluate();

        virtual bool isFinished() {return false;}

        friend std::ostream & operator<<(std::ostream & Str, World const & w);
    
    protected:
        int size_x;
        int size_y;
        int pop;
        Entity*** map;
        std::list<Agent*> agents;
        int t;

        gsl_rng* rng;

        std::list<Agent*> findCluster(std::list<Agent*>::iterator seed, std::list<Agent*>& agents) const;
};
