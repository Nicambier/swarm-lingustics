#pragma once

#include <list>
#include <stdint.h>

#include "world.h"

class World;

struct Vector2D{
    int x;
    int y;
};

class Agent {
    protected:
        int broadcast_range;
        
        World* world;
        
        Vector2D pos;
        std::list<uint16_t> msgs;
        
        std::list<uint16_t> new_msgs;
        
    public:
        Agent(World* w, Vector2D pos);
        virtual ~Agent();
        
        void Run();
        virtual void Loop();
        
        void Broadcast(uint16_t msg);
        void Receive(uint16_t msg);
        
        Vector2D GetPos();
        void SetPos(Vector2D newPos);
        
        void RandomWalk();

        virtual uint16_t GetColour() {
            return 0;
        }

        virtual std::string toString() const;

        friend std::ostream &operator <<(std::ostream &Str, const Agent &a);
};
