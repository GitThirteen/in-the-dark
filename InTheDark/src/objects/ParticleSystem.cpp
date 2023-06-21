#include "ParticleSystem.h"

ParticleSystem::ParticleSystem() : ParticleSystem(glm::vec3(0.0f)) { };

ParticleSystem::ParticleSystem(const glm::vec3& pos)
{
    std::vector<Particle> particles;

    particles[0].pos = pos;
    particles[0].color = glm::vec3(255.0f, 0.0f, 0.0f); // CHECK IF COLOR IS 0-1 or 0-255!!!!!
    particles[0].velocity = glm::vec3(0.0f, 0.0001f, 0.0f);
    particles[0].lifetime = 0.0f;

    glGenTransformFeedbacks(2, this->tfbuffer);
    glGenBuffers(2, this->particle_vbuffer);

    for (uint8_t i = 0; i < 2; i++)
    {
        glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, this->tfbuffer[i]);
        glBindBuffer(GL_ARRAY_BUFFER, this->particle_vbuffer[i]);
        glBufferData(GL_ARRAY_BUFFER, particles.size(), &particles.front(), GL_DYNAMIC_DRAW);
        glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, this->particle_vbuffer[i]);
    }

    // TODO SHADER SET TEXTURE, LAUNCHER LIFETIME, SHELL LIFETIME

    // TODO BIND TEXTURE


}

void ParticleSystem::render(int dt, const glm::mat4& vp, const glm::vec3& cam_pos)
{
    this->time_millis += dt;

    update(dt);
    draw(vp, cam_pos);

    this->cur_vbuffer = this->cur_tfbuffer;
    this->cur_tfbuffer = (this->cur_tfbuffer + 1) & 0x1;
}

void ParticleSystem::update(int dt)
{

}

void ParticleSystem::draw(const glm::mat4& vp, const glm::vec3& cam_pos)
{

}