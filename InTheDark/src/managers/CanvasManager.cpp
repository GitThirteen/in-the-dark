#include "CanvasManager.h"

void CanvasManager::clear()
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

void CanvasManager::clear(uint8_t r, uint8_t g, uint8_t b)
{
	clear(r, g, b, 255);
}

void CanvasManager::clear(uint8_t r, uint8_t g, uint8_t b, uint8_t alpha)
{
	float rgb_r = r / 255.0f;
	float rgb_g = g / 255.0f;
	float rgb_b = b / 255.0f;
	float rgb_a = std::min((int) alpha, 100) / 100.0f;
	glClearColor(rgb_r, rgb_g, rgb_b, alpha);
}

// Post Processor

void PostProcessor::create()
{
	// FBO
	GLuint fbo;
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	this->fbo = fbo;

	// Create texture
	
	GLuint tex;
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);

	this->tex = tex;

	SettingsManager& settings = SettingsManager::getInstance();
	auto width = settings.get<int>("width");
	auto height = settings.get<int>("height");

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex, 0);

	// Bind depth buffer (as render buffer object)
	GLuint rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, settings.get<int>("width"), settings.get<int>("height"));
	
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);

	this->rbo = rbo;

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		LOG_F(ERROR, "Incomplete framebuffer. Framebuffer has not been created.");
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		exit(EXIT_FAILURE);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	/* ------------------------- */
	/* ---- QUAD FOR SCREEN ---- */

	// Position (3) + UV (2)
	float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
		// positions   // texCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
	};

	/* ---- Create VAO and bind it ---- */

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	this->vao = vao;

	/* ---- Create VBO and bind it ---- */

	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);

	auto stride = 4 * sizeof(float);

	glVertexAttribPointer(ShaderLocation::POSITION, 2, GL_FLOAT, GL_FALSE, stride, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(ShaderLocation::UV, 2, GL_FLOAT, GL_FALSE, stride, (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);

	/* ---- Unbind VAO & VBO ---- */

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	this->created = true;

	/* ------------------------ */
	/* ---- SHADER PROGRAM ---- */

	shaders.add(Shader::Vertex, "../_shaders/post_proc.vert");
	shaders.add(Shader::Fragment, "../_shaders/post_proc.frag");
	this->shader = shaders.create();
}

void PostProcessor::draw()
{
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glUseProgram(this->shader);
	shaders.set(ShaderLocation::TEXTURE, 1); // I don't know why it's 1, but it just works

	glBindVertexArray(this->vao);
	glBindTexture(GL_TEXTURE_2D, this->tex);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindVertexArray(0);
	glUseProgram(0);
}

void PostProcessor::bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);
}

void PostProcessor::unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void PostProcessor::dump()
{
	glDeleteFramebuffers(1, &this->fbo);
	glDeleteRenderbuffers(1, &this->rbo);
	glDeleteTextures(1, &this->tex);
}

bool PostProcessor::isCreated()
{
	return this->created;
}