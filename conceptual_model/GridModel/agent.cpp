#include "agent.h"

using namespace std;

Agent::Agent(World* w, Vector2D pos): Entity(w,pos) {
    broadcast_range = 1;
}

Agent::~Agent() {
    
}
        
void Agent::Run() {
    Loop();
    msgs = new_msgs;
    new_msgs.clear();
}

void Agent::Loop() {
    RandomWalk();
}

void Agent::Broadcast(uint32_t msg) {
    world->BroadcastFrom(GetPos(),broadcast_range,msg);
}

void Agent::Receive(uint32_t msg) {
    new_msgs.push_back(msg);
}

void Agent::RandomWalk() {
    int dir = world->random(4);
    Vector2D newPos = pos;
    switch(dir) {
        case 0:
            ++newPos.x;
            break;
        case 1:
            --newPos.x;
            break;
        case 2:
            ++newPos.y;
            break;
        case 3:
            --newPos.y;
            break;
    }
    world->MoveAgentTo(this,newPos);
}

string Agent::toString() const {
    return to_string(pos.x)+","+to_string(pos.y);
}

ostream & operator<<(ostream & Str, Agent const & a) {
  Str<<a.toString();
  return Str;
}
