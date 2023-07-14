#pragma once
#include "GameObject.h"
#include "ParticleSystem.h"
#include "../Camera.h"

// TODO: Include Point Light!!!

class Torch
{
public:
    Torch(std::shared_ptr<GameObject>);
    void enable();
    void disable();
    void draw();
    void emitSmoke();
    void setCamera(std::shared_ptr<Camera>);
private:
    std::shared_ptr<GameObject> obj = nullptr;
    std::shared_ptr<Camera> camera = nullptr;
    ParticleSystem smoke_generator;
    bool isActive = true;
};