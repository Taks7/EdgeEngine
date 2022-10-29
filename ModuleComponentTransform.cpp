
#include "Application.h"
#include "Module.h"
#include "ModuleComponentTransform.h"

#include "ModuleGameObject.h"

ModuleComponentsTransform::ModuleComponentsTransform(ModuleGameObject* owner) : ModuleComponents(owner, COMPONENT_TYPES::TRANSFORM, "Transform")
{

}

ModuleComponentsTransform::~ModuleComponentsTransform()
{

}

bool ModuleComponentsTransform::Update()
{
	bool ret = true;

	return ret;
}

bool ModuleComponentsTransform::CleanUp()
{
	bool ret = true;

	return ret;
}

void ModuleComponentsTransform::Transform()
{
	//Aqui poner todo lo que tenga que ver con rotación y posicion y todo
}
float3 ModuleComponentsTransform::GetPosition() const
{
	return position;
}

float3 ModuleComponentsTransform::GetRotation() const
{
	return euler_rotation;
}

float3 ModuleComponentsTransform::GetScale() const
{
	return scale;
}

void ModuleComponentsTransform::SetPosition(const float3& position)
{
	this->position = position;

}

void ModuleComponentsTransform::SetRotation(const float3& rotation)
{
	euler_rotation = rotation;

	this->rotation.RotateX(euler_rotation.x);
	this->rotation.RotateY(euler_rotation.y);
	this->rotation.RotateZ(euler_rotation.z);

}

void ModuleComponentsTransform::SetScale(const float3& scale)
{
	this->scale = scale;

}