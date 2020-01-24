#ifndef ENTITY_H
#define ENTITY_H

#include <list>
#include <vector>
#include <stdint.h>
#include "world.h"

class World;

struct Vector2D{
    int x;
    int y;
};

class Entity
{
protected:
    World* world;
    Vector2D pos;

public:
    Entity(World* w, Vector2D pos);
    virtual ~Entity();

    Vector2D GetPos() const;
    void SetPos(Vector2D newPos);

    virtual void Receive(uint32_t msg) {}

    virtual uint32_t GetColour() const {
        return 0;
    }

    virtual std::string toString() const;

    friend std::ostream &operator <<(std::ostream &Str, const Entity &e);
};

#endif // ENTITY_H
