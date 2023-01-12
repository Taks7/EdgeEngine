#include "Particles.h"

Particles::Particles()
{
}

Particles::Particles(Particles* particleReference)
{
	position = particleReference->position;
	lifetime = particleReference->lifetime;
	speed = particleReference->speed;
	direction = particleReference->direction;
	color = particleReference->color;
	size = particleReference->size;
	dirVariation = particleReference->dirVariation;
	active = true;
	billboard = particleReference->billboard;  //TODO:ModuleComponentBillboard
	distanceToCamera = 0.0f;
}

Particles::~Particles()
{
	billboard = nullptr;  //TODO:ModuleComponentBillboard
}