#include "Torch.h"

Torch::Torch(std::shared_ptr<GameObject> game_obj) :
    obj(game_obj),
    smoke_generator(AssetType::SMOKE, glm::vec3(game_obj->position.x, game_obj->position.y + 0.2f, game_obj->position.z))
{ }

void Torch::enable()
{
    this->isActive = true;
}

void Torch::disable()
{
    this->isActive = false;
}

void Torch::draw()
{
    this->obj->asset.draw();
}

void Torch::emitSmoke()
{
    if (!this->camera)
    {
        LOG_F(WARNING, "Camera neccessary for torch to emit particles. No camera found.");
        return;
    }

    if (this->isActive)
    {
        this->smoke_generator.emit(this->camera);
    }
}

void Torch::setCamera(std::shared_ptr<Camera> camera)
{
    this->camera = camera;
}