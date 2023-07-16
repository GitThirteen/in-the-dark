#include "Player.h"

void Player::update(const glm::vec3& view_dir) // param temporary until camera is global
{
    bool pos_dz = events.key.pressed(GLFW_KEY_S) || events.key.pressed(GLFW_KEY_DOWN);
    bool neg_dz = events.key.pressed(GLFW_KEY_W) || events.key.pressed(GLFW_KEY_UP);
    bool pos_dx = events.key.pressed(GLFW_KEY_A) || events.key.pressed(GLFW_KEY_LEFT);
    bool neg_dx = events.key.pressed(GLFW_KEY_D) || events.key.pressed(GLFW_KEY_RIGHT);

    glm::vec3 forward = view_dir;
    forward.y = 0.0f;
    forward = glm::normalize(forward); //glm::vec3(0.0, 0.0, 1.0);
    glm::vec3 up = glm::vec3(0.0, 1.0, 0.0);
    glm::vec3 right = glm::normalize(glm::cross(forward, up)); // glm::vec3(1.0, 0.0, 0.0);

    float dx = 0.0, dz = 0.0;
    if (pos_dx ^ neg_dx)
    {
        dx = pos_dx ? -1.0 : 1.0;
    }
    if (pos_dz ^ neg_dz)
    {
        dz = pos_dz ? -1.0 : 1.0;
    }

    this->input_direction = (forward * dz + right * dx);
    this->input_direction.y = 0.0;
    //this->movement_vec.y = GRAVITY;

    auto force = calcForce();

    this->position.x += force.x;
    if (this->isColliding())
    {
        this->position.x -= force.x;
        force.x = 0;
    }

    this->position.z += force.z;
    if (this->isColliding())
    {
        this->position.z -= force.z;
        force.z = 0;
    }

    this->position.y += force.y;
    if (this->isColliding())
    {
        this->position.y -= force.y;
        force.y = 0;
    }

    this->asset.translate(force);

    // temp
    if (this->position.y < -3)
    {
        this->resetPosition();
    }
}

glm::vec3 Player::calcForce()
{
    float dt = clock.getDeltaTime();
    glm::vec3 max_velocity = MAX_SPEED * this->input_direction;
    max_velocity.y = GRAVITY;

    if (this->input_direction.x == 0 && this->input_direction.y == 0 && this->input_direction.z == 0)
    {
        TRANSITION_SPEED = 8.0f;
    }

    this->velocity = this->velocity * (1 - dt * TRANSITION_SPEED) + max_velocity * (dt * TRANSITION_SPEED);

    return velocity * dt;
}

bool Player::isCollidingWith(std::shared_ptr<GameObject> obj)
{
    BBox own_bbox = this->getTrueBBox();
    BBox obj_bbox = obj->getTrueBBox();
    return (
        own_bbox.lower.x < obj_bbox.upper.x &&
        own_bbox.upper.x > obj_bbox.lower.x &&
        own_bbox.lower.y < obj_bbox.upper.y &&
        own_bbox.upper.y > obj_bbox.lower.y &&
        own_bbox.lower.z < obj_bbox.upper.z &&
        own_bbox.upper.z > obj_bbox.lower.z
    );
}

void Player::resetPosition()
{
    this->asset.translate(glm::vec3(0) - this->position);
    this->position = glm::vec3(0);
}

void Player::setLevelObjs(std::vector<std::shared_ptr<GameObject>>& objs)
{
    this->level_objs = &objs;
}

bool Player::isColliding()
{
    for (auto& obj : *this->level_objs)
    {
        if (
            obj->asset.type == AssetType::PLAYER ||
            obj->asset.type == AssetType::TORCH
            ) continue;
        //if (obj->isGround) continue;

        if (this->isCollidingWith(obj))
        {
            return true;
        }
    }

    return false;
}

std::vector<std::shared_ptr<GameObject>> Player::getCollisions()
{
    auto collisions = std::vector<std::shared_ptr<GameObject>>();
    for (auto& obj : *this->level_objs)
    {
        if (
            obj->asset.type == AssetType::PLAYER ||
            obj->asset.type == AssetType::TORCH
            ) continue;
        if (obj->isGround) continue;

        if (this->isCollidingWith(obj))
        {
            collisions.push_back(obj);
        }
    }
    return collisions;
}