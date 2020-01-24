#ifndef MAZEWORLD_H
#define MAZEWORLD_H

#include "world.h"
#include <vector>

class MazeWorld : public World
{
public:
    MazeWorld(int x, int y, int pop, AgentFactory* factory, int tile_size, float free_ratio, unsigned int seed=0);
};


#endif // MAZEWORLD_H
