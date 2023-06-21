#pragma once
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>
#include <vector>
#include "../managers/ShaderManager.h"

struct ParticleSettings
{
    uint16_t max_particles = 2000;
    float particle_lifetime = 5.0f;
};

struct Particle
{
    glm::vec3 pos;
    glm::vec3 color;
    glm::vec3 velocity;
    float lifetime;
};

class ParticleSystem
{
public:
    ParticleSystem();
    ParticleSystem(const glm::vec3&);

    void render(int, const glm::mat4&, const glm::vec3&);
private:
    bool first = true;
    uint16_t cur_vbuffer = 0;
    uint16_t cur_tfbuffer = 1;
    GLuint particle_vbuffer[2];
    GLuint tfbuffer[2];
    int time_millis = 0.0f;
    // TODO

    ParticleSettings settings;

    void update(int);
    void draw(const glm::mat4&, const glm::vec3&);

    ShaderManager& shaders = ShaderManager::getInstance();
};