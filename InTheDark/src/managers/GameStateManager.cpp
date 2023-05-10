#include "GameStateManager.h"

void GameStateManager::changeState(std::unique_ptr<GameState> state)
{
	this->discard();

	state->init();
	this->active_state.swap(state);
}

void GameStateManager::update()
{
	if (this->active_state)
	{
		this->active_state->update();
	}
}

void GameStateManager::draw() // TODO don't pass radius in as param but rather have some sort of global entity tracking that, maybe same for mouse position
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	this->camera.unlock(); // TODO remove and properly use lock & unlock once main menu and co are defined
	
	events.poll();
	bool mouse_pressed = events.mouse.pressed(GLFW_MOUSE_BUTTON_LEFT);
	glm::vec2 mouse_pos = events.mouse.getPosition();
	double cam_offset = events.mouse.getOffset();

	this->camera.update(mouse_pressed, mouse_pos, cam_offset); // <-- replace 6.0f with radius

	shaders.set(ShaderLocation::VIEWPROJECTION_MAT, this->camera.getViewProjMatrix());

	if (this->active_state)
	{
		this->active_state->draw();
	}

	glfwSwapBuffers(this->window);
}

void GameStateManager::discard()
{
	if (this->active_state)
	{
		this->active_state->discard();
	}
}