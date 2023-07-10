#include "ParticleSystem.h"

ParticleSystem::ParticleSystem() : ParticleSystem({ }, glm::vec3(0.0f)) { };

ParticleSystem::ParticleSystem(const asset::Texture& texture, const glm::vec3& pos)
{
    this->settings.texture = texture;
    this->settings.initial_position = pos;

    std::vector<Particle> particles = std::vector<Particle>(this->settings.max_particles);

    particles[0].pos = pos;
    particles[0].velocity = glm::vec3(0.0f, 0.0001f, 0.0f);
    particles[0].lifetime = 0.0f;
    particles[0].color = glm::vec3(1.0f, 0.0f, 0.0f);

    /* ---- Create VAO and bind it ---- */

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    this->vao = vao;

    /* ---- Create 2 array buffers for particles and 2 transform feedback buffers ---- */

    glGenTransformFeedbacks(2, this->tfbuffer);
    glGenBuffers(2, this->particle_vbuffer);

    for (uint8_t i = 0; i < 2; i++)
    {
        glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, this->tfbuffer[i]);
        glBindBuffer(GL_ARRAY_BUFFER, this->particle_vbuffer[i]);
        glBufferData(GL_ARRAY_BUFFER, particles.size(), &particles.front(), GL_DYNAMIC_DRAW);
        glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, this->particle_vbuffer[i]);
    }

    shaders.add(Shader::Vertex, "../_shaders/ps_default.vert");
    shaders.add(Shader::Geometry, "../_shaders/ps_default.geom");
    shaders.add(Shader::Fragment, "../_shaders/ps_default.frag");
    GLuint shader = shaders.create();

    // The out variables of the geometry shader
    const GLchar* varyings[4];
    varyings[0] = "positionOut";
    varyings[1] = "velocityOut";
    varyings[2] = "ageOut";
    varyings[3] = "colorOut";

    glTransformFeedbackVaryings(shader, 4, varyings, GL_INTERLEAVED_ATTRIBS);
    shaders.link(shader);

    shaders.add(Shader::Vertex, "../_shaders/ps_billboard.vert");
    shaders.add(Shader::Geometry, "../_shaders/ps_billboard.geom");
    shaders.add(Shader::Fragment, "../_shaders/ps_billboard.frag");
    shader = shaders.create();
    shaders.link(shader);
}

void ParticleSystem::render(const glm::mat4& viewproj, const glm::vec3& cam_pos)
{
    this->elapsedTime += clock.getDeltaTime();

    update();
    draw(viewproj, cam_pos);

    this->cur_vbuffer = this->cur_tfbuffer;
    this->cur_tfbuffer = (this->cur_tfbuffer + 1) & 0x1;
}

void ParticleSystem::update()
{
    shaders.use("ps_default");

    bool gen_particle = this->elapsedTime >= this->settings.emit_rate;

    shaders.set(ShaderLocation::DELTA_TIME, (float) clock.getDeltaTime());
    shaders.set(ShaderLocation::PARTICLE_LIFETIME, this->settings.lifetime);
    shaders.set(ShaderLocation::PS_POSITION, this->settings.initial_position);
    shaders.set(ShaderLocation::PS_GEN_FLAG, gen_particle);

    this->settings.texture.bind();

    glBindVertexArray(this->vao);
    glEnable(GL_RASTERIZER_DISCARD);

    glBindBuffer(GL_ARRAY_BUFFER, this->particle_vbuffer[this->cur_vbuffer]);
    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, this->tfbuffer[this->cur_tfbuffer]);

    auto stride = sizeof(Particle);

    glVertexAttribPointer(ShaderLocation::PARTICLE_POSITION, 3, GL_FLOAT, GL_FALSE, stride, 0);
    glEnableVertexAttribArray(ShaderLocation::PARTICLE_POSITION);
    glVertexAttribPointer(ShaderLocation::PARTICLE_VELOCITY, 3, GL_FLOAT, GL_FALSE, stride, (const GLvoid*)(3 * sizeof(float)));
    glEnableVertexAttribArray(ShaderLocation::PARTICLE_VELOCITY);
    glVertexAttribPointer(ShaderLocation::PARTICLE_AGE, 1, GL_FLOAT, GL_FALSE, stride, (const GLvoid*)(6 * sizeof(float)));
    glEnableVertexAttribArray(ShaderLocation::PARTICLE_AGE);
    glVertexAttribPointer(ShaderLocation::PARTICLE_COLOR, 3, GL_FLOAT, GL_FALSE, stride, (const GLvoid*)(7 * sizeof(float)));
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

    glBindVertexArray(0);
    this->settings.texture.unbind();

    if (gen_particle) {
        this->elapsedTime = 0.0f;
    }
}

void ParticleSystem::draw(const glm::mat4& viewproj, const glm::vec3& cam_pos)
{
    shaders.use("ps_billboard");

    shaders.set(ShaderLocation::VIEWPROJECTION_MAT, viewproj);
    shaders.set(ShaderLocation::CAMERA_POSITION, cam_pos);
    shaders.set(ShaderLocation::PARTICLE_SIZE, this->settings.size);
    shaders.set(ShaderLocation::TEXTURE, this->settings.texture.tex_unit);

    this->settings.texture.bind();

    glBindVertexArray(this->vao);
    glDisable(GL_RASTERIZER_DISCARD);

    glBindBuffer(GL_ARRAY_BUFFER, this->particle_vbuffer[this->cur_tfbuffer]);
    
    glVertexAttribPointer(ShaderLocation::POSITION, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(ShaderLocation::POSITION);

    glDrawTransformFeedback(GL_POINTS, this->tfbuffer[this->cur_tfbuffer]);

    glBindVertexArray(0);
    glDisableVertexAttribArray(ShaderLocation::POSITION);
    this->settings.texture.unbind();
}