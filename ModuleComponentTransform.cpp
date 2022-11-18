
#include "Application.h"
#include "Module.h"
#include "ModuleComponentTransform.h"

#include "ModuleGameObject.h"

ModuleComponentsTransform::ModuleComponentsTransform(ModuleGameObject* owner) : ModuleComponents(owner, COMPONENT_TYPES::TRANSFORM, "Transform")
{
	matrix = float4x4::identity;
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

void ModuleComponentsTransform::UpdateMatrix()
{
	matrix = float4x4::FromTRS(position, rotation, scale);
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
	UpdateMatrix();

}

void ModuleComponentsTransform::SetRotation(const float3& rotation_)
{
	euler_rotation = rotation_;

	rotation = Quat::FromEulerXYZ(rotation_.x, rotation_.y, rotation_.z);

	UpdateMatrix();

}

void ModuleComponentsTransform::SetScale(const float3& scale)
{
	this->scale = scale;
	UpdateMatrix();

}