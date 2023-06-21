#pragma once
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>
#include <vector>

struct ParticleSettings
{
    uint16_t max_particles = 2000;
    float particle_lifetime = 5.0f;
};

struct Particle
{
    float type;
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
    void update();

private:
    bool is_first;
    uint16_t cur_vbuffer;
    uint16_t cur_tfbuffer;
    GLuint particle_buffer[2];
    GLuint transform_feedback[2];
    // TODO

    ParticleSettings settings;
};