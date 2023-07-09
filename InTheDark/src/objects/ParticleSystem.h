#pragma once
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>
#include <vector>
#include "../managers/ShaderManager.h"
#include "../managers/Clock.h"

struct ParticleSettings
{
    uint16_t max_particles = 2000;
    float particle_lifetime = 5.0f;
};

struct Particle
{
    glm::vec3 pos;
    glm::vec3 velocity;
    float lifetime;
    glm::vec3 color;
};

class ParticleSystem
{
public:
    ParticleSystem();
    ParticleSystem(const glm::vec3&);

    void render(const glm::mat4&, const glm::vec3&);
private:
    bool first = true;
    GLuint vao = 0;
    uint16_t cur_vbuffer = 0;
    uint16_t cur_tfbuffer = 1;
    GLuint particle_vbuffer[2];
    GLuint tfbuffer[2];
    // TODO

    ParticleSettings settings;

    void update();
    void draw(const glm::mat4&, const glm::vec3&);

    ShaderManager& shaders = ShaderManager::getInstance();
    Clock& clock = Clock::getInstance();
};