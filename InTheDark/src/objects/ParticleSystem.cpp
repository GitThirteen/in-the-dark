#include "ParticleSystem.h"

ParticleSystem::ParticleSystem() : ParticleSystem({ }, glm::vec3(0.0f)) { };

ParticleSystem::ParticleSystem(const asset::Texture& texture, const glm::vec3& pos)
{
    this->settings.texture = texture;
    this->settings.initial_position = pos;

    std::vector<Particle> particles = std::vector<Particle>(this->settings.max_particles * sizeof(Particle));

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

    GLint maxAttribs;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &maxAttribs);
    std::cout << maxAttribs << std::endl;

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

    shaders.set(ShaderLocation::Particle::DELTA_TIME, (float) clock.getDeltaTime());
    shaders.set(ShaderLocation::Particle::LIFETIME, this->settings.lifetime);
    shaders.set(ShaderLocation::Particle::SYSTEM_POSITION, this->settings.initial_position);
    shaders.set(ShaderLocation::Particle::GENERATION_FLAG, gen_particle);

    glBindVertexArray(this->vao);
    glEnable(GL_RASTERIZER_DISCARD);

    glBindBuffer(GL_ARRAY_BUFFER, this->particle_vbuffer[this->cur_vbuffer]);
    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, this->tfbuffer[this->cur_tfbuffer]);

    auto stride = sizeof(Particle);

    glVertexAttribPointer(ShaderLocation::Particle::POSITION, 3, GL_FLOAT, GL_FALSE, stride, 0);
    glEnableVertexAttribArray(ShaderLocation::Particle::POSITION);
    glVertexAttribPointer(ShaderLocation::Particle::VELOCITY, 3, GL_FLOAT, GL_FALSE, stride, (const GLvoid*)(3 * sizeof(float)));
    glEnableVertexAttribArray(ShaderLocation::Particle::VELOCITY);
    glVertexAttribPointer(ShaderLocation::Particle::AGE, 1, GL_FLOAT, GL_FALSE, stride, (const GLvoid*)(6 * sizeof(float)));
    glEnableVertexAttribArray(ShaderLocation::Particle::AGE);
    glVertexAttribPointer(ShaderLocation::Particle::COLOR, 3, GL_FLOAT, GL_FALSE, stride, (const GLvoid*)(7 * sizeof(float)));
    glEnableVertexAttribArray(ShaderLocation::Particle::COLOR);

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

    glDisableVertexAttribArray(ShaderLocation::Particle::POSITION);
    glDisableVertexAttribArray(ShaderLocation::Particle::VELOCITY);
    glDisableVertexAttribArray(ShaderLocation::Particle::AGE);
    glDisableVertexAttribArray(ShaderLocation::Particle::COLOR);

    glBindVertexArray(0);

    if (gen_particle) {
        this->elapsedTime = 0.0f;
    }
}

void ParticleSystem::draw(const glm::mat4& viewproj, const glm::vec3& cam_pos)
{
    shaders.use("ps_billboard");

    shaders.set(ShaderLocation::Billboard::VIEWPROJECTION_MAT, viewproj);
    shaders.set(ShaderLocation::Billboard::CAMERA_POSITION, cam_pos);
    shaders.set(ShaderLocation::Billboard::PARTICLE_SIZE, this->settings.size);
    shaders.set(ShaderLocation::Billboard::TEXTURE, this->settings.texture.tex_unit);

    this->settings.texture.bind();

    glBindVertexArray(this->vao);
    glDisable(GL_RASTERIZER_DISCARD);

    glBindBuffer(GL_ARRAY_BUFFER, this->particle_vbuffer[this->cur_tfbuffer]);
    
    glVertexAttribPointer(ShaderLocation::Billboard::POSITION, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(ShaderLocation::Billboard::POSITION);

    glDrawTransformFeedback(GL_POINTS, this->tfbuffer[this->cur_tfbuffer]);

    glBindVertexArray(0);
    glDisableVertexAttribArray(ShaderLocation::Billboard::POSITION);
    this->settings.texture.unbind();
}