#pragma once

#include <iostream>
#include <cmath> 
#include <list>
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <stdint.h>
#include <time.h>       /* time */


class AgentFactory;
class Agent;
struct Vector2D;

class World {    
    public:
        
        World(int x, int y, int pop, AgentFactory* factory);
        ~World();
        
        bool MoveAgentTo(Agent* a, Vector2D pos);   
        
        void BroadcastFrom(Vector2D from, int range, uint16_t msg);
        void Run();

        bool isOccupied(int x, int y);
        uint16_t CellColour(int x, int y);

        int GetSizeX() {return size_x;}
        int GetSizeY() {return size_y;}
        int GetTime() {return t;}

        friend std::ostream & operator<<(std::ostream & Str, World const & w);
    
    private:  
        int size_x;
        int size_y;
        int pop;
        Agent*** map;
        std::list<Agent*> agents;

        int t;    
};
