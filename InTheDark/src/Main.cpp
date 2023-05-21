#pragma once

#include "managers/GameStateManager.h"
#include "managers/SettingsManager.h"
#include "managers/ShaderManager.h"

#include "states/TestState.cpp"
#include "util/util.h"

/* ---------------------------- */
// Prototypes
/* ---------------------------- */

void setupLogger();
GLFWwindow* initGL();

/* ---------------------------- */
// Global variables
/* ---------------------------- */

GameStateManager& game = GameStateManager::getInstance();
SettingsManager& settings = SettingsManager::getInstance();
ShaderManager& shaders = ShaderManager::getInstance();
EventHandler& events = EventHandler::getInstance();

/* ---------------------------- */
// Main
/* ---------------------------- */

int main(int argc, char** argv)
{
	setupLogger();

	auto window = initGL();
	game.setWindow(window);

	shaders.add(Shader::Vertex, "../_shaders/shader.vert");
	shaders.add(Shader::Fragment, "../_shaders/shader.frag");
	shaders.create();
	//load the texture here and set the according uniform to the shader 
	shaders.use();

	events.setWindow(window);
	events.mouse.enableScrollCallback();

	auto test_state = std::make_unique<TestState>();
	game.changeState(std::move(test_state));
	
	while (!glfwWindowShouldClose(window))
	{
		game.update();
		game.draw();
	}

	shaders.destroy();
	glfwDestroyWindow(window);
	glfwTerminate();
}

GLFWwindow* initGL()
{
	/* ---- GLFW Init ---- */

	if (!glfwInit())
	{
		auto error_msg = "GLFW Initialization failed.";
		LOG_F(ERROR, error_msg);
		exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); // Prevents GLFW from getting resized, change if needed
	glfwWindowHint(GLFW_SAMPLES, 4);

	auto width = settings.get<int>("width");
	auto height = settings.get<int>("height");
	
	GLFWwindow* window = glfwCreateWindow(
		width,
		height,
		settings.get<std::string>("title").c_str(),
		NULL, NULL);
	
	glfwGetFramebufferSize(window, &width, &height);
	
	if (!window)
	{
		auto error_msg = "Window Initialization failed.";
		LOG_F(ERROR, error_msg);
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);

	/* ---- GLEW Init ---- */

	glewExperimental = true;
	GLenum glewResponse = glewInit();
	if (glewResponse != GLEW_OK)
	{
		auto error_msg = "GLEW Initialization failed.";
		LOG_F(ERROR, error_msg);
		exit(EXIT_FAILURE);
	}

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_CULL_FACE);

	glViewport(0, 0, width, height);

	return window;
}

void setupLogger()
{
	loguru::g_preamble_date = false;
	loguru::g_preamble_uptime = false;
	loguru::g_preamble_thread = false;
}