#pragma once
#include "GameObject.h"
#include "LightSource.h"
#include "ParticleSystem.h"
#include "../Camera.h"

class Torch
{
public:
    lightSource::Point light;
    bool isActive = false;
    std::shared_ptr<GameObject> obj = nullptr;
    int id = -1;

    Torch(std::shared_ptr<GameObject>, asset::Texture);
    void enable();
    void disable();
    void draw();
    void emitSmoke();
    void setCamera(std::shared_ptr<Camera>);

private:
    std::shared_ptr<Camera> camera = nullptr;
    ParticleSystem smoke_generator;

    inline static int global_torch_id = -1;
};