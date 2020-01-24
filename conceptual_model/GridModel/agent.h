#pragma once

#include "entity.h"

class Agent : public Entity {
    protected:
        int broadcast_range;
        std::vector<uint32_t> msgs;
        
        std::vector<uint32_t> new_msgs;
        
    public:
        Agent(World* w, Vector2D pos);
        virtual ~Agent();
        
        void Run();
        virtual void Loop();

        void Broadcast(uint32_t msg);
        void Receive(uint32_t msg);
        
        void RandomWalk();

        int GetBroadCastRange() const {
            return broadcast_range;
        }


        virtual uint32_t GetColour() const {
            return 255<<8;
        }

        virtual std::string toString() const;
};
