#include "entity.h"
using namespace std;

Entity::Entity(World* w, Vector2D pos) {
    world = w;
    this->pos = pos;
}

Entity::~Entity() {

}

Vector2D Entity::GetPos() const {
    return pos;
}

void Entity::SetPos(Vector2D newPos) {
    pos = newPos;
}

string Entity::toString() const {
    return to_string(pos.x)+","+to_string(pos.y);
}

ostream & operator<<(ostream & Str, Entity const & a) {
  Str<<a.toString();
  return Str;
}

