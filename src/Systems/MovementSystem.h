#ifndef MOVEMENTSYSTEM_H
#define MOVEMENTSYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/RigidBodyComponent.h"
#include <spdlog/spdlog.h>

class MovementSystem: public System 
{
    public:
        MovementSystem() 
        {
            RequireComponent<TransformComponent>();
            RequireComponent<RigidBodyComponent>();
        }

        void Update(double deltaTime) 
        {
            // Loop all entities that the system is interested in
            for (auto entity: GetSystemEntities()) 
            {
				auto& transform = entity.GetComponent<TransformComponent>();
                const auto rigidbody = entity.GetComponent<RigidBodyComponent>();

				transform.position.x += rigidbody.velocity.x * deltaTime;
				transform.position.y += rigidbody.velocity.y * deltaTime;
				
                /*spdlog::info("Entity id = {} position is now {},{}", 
                    entity.GetId(), 
                    std::to_string(transform.position.x), 
                    transform.position.y
                );*/
            }
        }
};

#endif
