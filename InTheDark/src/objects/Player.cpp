#include "Player.h"

void Player::update()
{
    bool pos_dz = events.key.pressed(GLFW_KEY_S) || events.key.pressed(GLFW_KEY_DOWN);
    bool neg_dz = events.key.pressed(GLFW_KEY_W) || events.key.pressed(GLFW_KEY_UP);
    bool pos_dx = events.key.pressed(GLFW_KEY_D) || events.key.pressed(GLFW_KEY_RIGHT);
    bool neg_dx = events.key.pressed(GLFW_KEY_A) || events.key.pressed(GLFW_KEY_LEFT);

    glm::vec3 forward = glm::vec3(0.0, 0.0, 1.0);
    glm::vec3 right = glm::vec3(1.0, 0.0, 0.0);

    float dx = 0.0, dz = 0.0;
    if (pos_dx ^ neg_dx)
    {
        dx = pos_dx ? 1.0 : -1.0;
    }
    if (pos_dz ^ neg_dz)
    {
        dz = pos_dz ? 1.0 : -1.0;
    }

    this->movementVector = (forward * dz + right * dx) * speed;
    move();
}

void Player::move()
{
    double dt = clock.getDeltaTime();
    glm::vec3 weee = glm::vec3(
        this->movementVector.x * dt,
        this->movementVector.y * dt,
        this->movementVector.z * dt
    );
    
    this->position += weee;
    this->asset.translate(weee);
}