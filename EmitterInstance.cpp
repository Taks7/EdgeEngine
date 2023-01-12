#include "Application.h"
#include "EmitterInstance.h"
#include "ParticlesEmitter.h"
#include "ModuleSceneIntro.h"
#include "ModuleFBXLoader.h"
#include "glew/glew-2.2.0/include/GL/glew.h"

#include "Color.h"
#include "ModuleComponentParticles.h"
#include "ModuleComponentBillboard.h"

#include "ModuleCamera3D.h"
#include "ModuleGameObject.h"



//Arreglar el module particles

EmitterInstance::EmitterInstance(ParticleEmitter* emitterReference)
{
    this->emitter = emitterReference;
}

void EmitterInstance::Init()
{
    for (uint i = 0; i < emitter->modules.size(); ++i)
    {
       /* emitter->modules[i]->Init(this);*/
    }
}

void EmitterInstance::UpdateModules()
{
    for (uint i = 0; i < emitter->modules.size(); ++i)
    {
       /* emitter->modules[i]->Update(this);*/
    }
}

void EmitterInstance::Draw()
{
    for (uint i = 0; i < emitter->modules.size(); ++i)
    {
        /*emitter->modules[i]->DrawParticles();*/
    }
}

void EmitterInstance::UpdateParticleReference()
{
    for (uint i = 0; i < emitter->modules.size(); ++i)
    {
       /* emitter->modules[i]->UpdateParticleReference(this);*/
    }
}

void EmitterInstance::Reset()
{
    for (uint i = 0; i < emitter->modules.size(); ++i)
    {
       /* emitter->modules[i]->Reset();*/
    }
}

