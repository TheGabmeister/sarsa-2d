#ifndef BOXCOLLIDERCOMPONENT_H
#define BOXCOLLIDERCOMPONENT_H

#include <glm/glm.hpp>

struct BoxColliderComponent {
    float width;
    float height;
    glm::vec2 offset;

    BoxColliderComponent(float width = 0, float height = 0, glm::vec2 offset = glm::vec2(0)) {
        this->width = width;
        this->height = height;
        this->offset = offset;
    }
};

#endif
