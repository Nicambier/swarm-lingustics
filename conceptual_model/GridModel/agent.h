#pragma once

#include <list>
#include <vector>
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
        std::vector<uint32_t> msgs;
        
        std::vector<uint32_t> new_msgs;
        
    public:
        Agent(World* w, Vector2D pos);
        virtual ~Agent();
        
        void Run();
        virtual void Loop();

        void Broadcast(uint32_t msg);
        void Receive(uint32_t msg);
        
        Vector2D GetPos() const;
        void SetPos(Vector2D newPos);
        
        void RandomWalk();

        int GetBroadCastRange() const {
            return broadcast_range;
        }


        virtual uint32_t GetColour() const {
            return 0;
        }

        virtual std::string toString() const;

        friend std::ostream &operator <<(std::ostream &Str, const Agent &a);
};
