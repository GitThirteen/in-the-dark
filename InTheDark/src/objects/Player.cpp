#include "Player.h"

void Player::update()
{
    // Adjust, neg_dz and pos_dz probably wrong due to RHCS
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

    this->input_direction = (forward * dz + right * dx);
    //this->movement_vec.y = GRAVITY;

    move();
    //std::cout << std::to_string(this->movement_vec.x) + " " + std::to_string(this->movement_vec.y) + " " + std::to_string(this->movement_vec.z) << std::endl;

    //movement_vec *= FRICTION_FACTOR;

    if (this->position.y < -3)
    {
        resetPosition();
    }
}

void Player::move()
{
    float dt = clock.getDeltaTime();
    glm::vec3 max_velocity = MAX_SPEED * this->input_direction;

    if (this->input_direction.x == 0 && this->input_direction.y == 0 && this->input_direction.z == 0)
    {
        TRANSITION_SPEED = 8.0f;
    }

    this->velocity = this->velocity * (1 - dt * TRANSITION_SPEED) + max_velocity * (dt * TRANSITION_SPEED);

    glm::vec3 force = velocity * dt;

    // update player bbox
    this->bbox.lower += force;
    this->bbox.upper += force;
    
    // update player position and translate asset accordingly
    this->position += force;
    this->asset.translate(force);
}

void Player::resetPosition()
{
    this->asset.translate(-this->position);
    this->position = glm::vec3(0, 0, 0);
}

bool Player::isCollidingWith(std::shared_ptr<GameObject> obj)
{
    return (
        this->bbox.lower.x <= obj->bbox.upper.x &&
        this->bbox.upper.x >= obj->bbox.lower.x &&
        this->bbox.lower.y <= obj->bbox.upper.y &&
        this->bbox.upper.y >= obj->bbox.lower.y &&
        this->bbox.lower.z <= obj->bbox.upper.z &&
        this->bbox.upper.z >= obj->bbox.lower.z
    );
}