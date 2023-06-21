#include "ParticleSystem.h"

ParticleSystem::ParticleSystem() : ParticleSystem(glm::vec3(0.0f)) { };

ParticleSystem::ParticleSystem(const glm::vec3& pos)
{
    std::vector<Particle> particles = std::vector<Particle>(this->settings.max_particles);

    particles[0].pos = pos;
    particles[0].velocity = glm::vec3(0.0f, 0.0001f, 0.0f);
    particles[0].lifetime = 0.0f;
    particles[0].color = glm::vec3(1.0f, 0.0f, 0.0f);

    glGenTransformFeedbacks(2, this->tfbuffer);
    glGenBuffers(2, this->particle_vbuffer);

    for (uint8_t i = 0; i < 2; i++)
    {
        glBindBuffer(GL_ARRAY_BUFFER, this->particle_vbuffer[i]);
        glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, this->tfbuffer[i]);
        glBufferData(GL_ARRAY_BUFFER, particles.size(), &particles.front(), GL_DYNAMIC_DRAW);
        glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, this->particle_vbuffer[i]);
    }

    shaders.add(Shader::Vertex, "../_shaders/particle_system.vert");
    shaders.add(Shader::Fragment, "../_shaders/particle_system.frag");
    shaders.add(Shader::Geometry, "../_shaders/particle_system.geom");
    GLuint shader = shaders.create();

    const GLchar* varyings[4];
    varyings[0] = "positionOut";
    varyings[1] = "velocityOut";
    varyings[2] = "ageOut";
    varyings[3] = "colorOut";

    glTransformFeedbackVaryings(shader, 4, varyings, GL_INTERLEAVED_ATTRIBS);
    shaders.link(shader);
}

void ParticleSystem::render()
{
    update();
    draw();

    this->cur_vbuffer = this->cur_tfbuffer;
    this->cur_tfbuffer = (this->cur_tfbuffer + 1) & 0x1;
}

void ParticleSystem::update()
{
    shaders.use("particle_system");

    shaders.set(ShaderLocation::DELTA_TIME, clock.getDeltaTimeAsMillis());

    glEnable(GL_RASTERIZER_DISCARD);

    glBindBuffer(GL_ARRAY_BUFFER, this->particle_vbuffer[this->cur_vbuffer]);
    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, this->tfbuffer[this->cur_tfbuffer]);

    glVertexAttribPointer(ShaderLocation::PARTICLE_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), 0);
    glEnableVertexAttribArray(ShaderLocation::PARTICLE_POSITION);
    glVertexAttribPointer(ShaderLocation::PARTICLE_VELOCITY, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (const GLvoid*)12);
    glEnableVertexAttribArray(ShaderLocation::PARTICLE_VELOCITY);
    glVertexAttribPointer(ShaderLocation::PARTICLE_AGE, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (const GLvoid*)16);
    glEnableVertexAttribArray(ShaderLocation::PARTICLE_AGE);
    glVertexAttribPointer(ShaderLocation::PARTICLE_COLOR, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (const GLvoid*)28);
    glEnableVertexAttribArray(ShaderLocation::PARTICLE_COLOR);

    glBeginTransformFeedback(GL_POINTS);

    if (this->first)
    {
        glDrawArrays(GL_POINTS, 0, 1);
        this->first = false;
    }
    else
    {
        glDrawTransformFeedback(GL_POINTS, this->tfbuffer[this->cur_vbuffer]);
    }

    glEndTransformFeedback();

    glDisableVertexAttribArray(ShaderLocation::PARTICLE_POSITION);
    glDisableVertexAttribArray(ShaderLocation::PARTICLE_VELOCITY);
    glDisableVertexAttribArray(ShaderLocation::PARTICLE_AGE);
    glDisableVertexAttribArray(ShaderLocation::PARTICLE_COLOR);
}

void ParticleSystem::draw()
{
    // Maybe set camera pos and view-projection?

    glDisable(GL_RASTERIZER_DISCARD);

    glBindBuffer(GL_ARRAY_BUFFER, this->particle_vbuffer[this->cur_tfbuffer]);
    
    glVertexAttribPointer(ShaderLocation::PARTICLE_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (const GLvoid*)4);
    glEnableVertexAttribArray(ShaderLocation::PARTICLE_POSITION);

    glDrawTransformFeedback(GL_POINTS, this->tfbuffer[this->cur_tfbuffer]);

    glDisableVertexAttribArray(0);
}