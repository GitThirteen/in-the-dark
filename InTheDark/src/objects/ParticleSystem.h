#pragma once
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>
#include <vector>
#include "../managers/ShaderManager.h"
#include "../managers/Clock.h"
#include "GameAsset.h"

struct ParticleSettings
{
    asset::Texture texture;
    uint16_t max_particles = 200;
    float lifetime = 3.5f;
    float size = 0.05f;
    glm::vec3 initial_position = glm::vec3(0.0f);
    float default_emit_rate = 1.0f;
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
    ParticleSystem(const asset::Texture&, const glm::vec3&);

    void render(const glm::mat4&, const glm::vec3&);
private:
    bool first = true;
    GLuint vao = 0;
    uint16_t cur_vbuffer = 0;
    uint16_t cur_tfbuffer = 1;
    GLuint particle_vbuffer[2];
    GLuint tfbuffer[2];

    float elapsedTime = 0.75f * util::random(1.0); // random offset so it doesn't look eerie
    float emit_rate = 1.0f;

    ParticleSettings settings;

    void update();
    void draw(const glm::mat4&, const glm::vec3&);

    ShaderManager& shaders = ShaderManager::getInstance();
    Clock& clock = Clock::getInstance();
};