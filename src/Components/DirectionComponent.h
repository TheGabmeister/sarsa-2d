#ifndef DIRECTIONCOMPONENT_H
#define DIRECTIONCOMPONENT_H

#include <glm/glm.hpp>

struct DirectionComponent {
    glm::vec2 movement_direction;

    DirectionComponent(glm::vec2 movement_direction = glm::vec2(0, 0)) {
        this->movement_direction = movement_direction;
    }
};

#endif
