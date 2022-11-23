
#include "Application.h"
#include "Module.h"
#include "ModuleComponent.h"
#include "ModuleComponentTransform.h"

#include "ModuleGameObject.h"

ModuleComponentsTransform::ModuleComponentsTransform(ModuleGameObject* owner) : ModuleComponents(owner, COMPONENT_TYPES::TRANSFORM, "Transform")
{
	matrix = float4x4::identity;
	GlobalMatrix = matrix;
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
void ModuleComponentsTransform::UpdateGlobalMatrix()
{
	///**/ModuleGameObject* NewOwner = GetOwner();
	
	if (owner->parent != nullptr)
	{
		ModuleComponentsTransform* newTransform = (ModuleComponentsTransform*)owner->parent->GetComponent(COMPONENT_TYPES::TRANSFORM);
		/*ModuleComponentsTransform* newTransform = (ModuleComponentsTransform*)owner->GetComponent(COMPONENT_TYPES::TRANSFORM);*/
		GlobalMatrix = newTransform->GlobalMatrix * matrix;
	}
	else
	{
		GlobalMatrix = matrix;

	}

	for (int i = 0; i < owner->childs.size(); i++)
	{
		ModuleComponentsTransform* newTransform_child = (ModuleComponentsTransform*)owner->childs[i]->GetComponent(COMPONENT_TYPES::TRANSFORM);
		newTransform_child->GlobalMatrix = matrix;
	}

}
void ModuleComponentsTransform::UpdateMatrix()
{
	matrix = float4x4::FromTRS(position, rotation, scale);
	UpdateGlobalMatrix();
}

float4x4 ModuleComponentsTransform::GetGlobalMatrix()
{
	return GlobalMatrix;
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

void ModuleComponentsTransform::SetPosition(const float3& position_)
{
	position = position_;
	UpdateMatrix();

}

void ModuleComponentsTransform::SetRotation(const float3& rotation_)
{
	euler_rotation = rotation_;

	rotation = Quat::FromEulerXYZ(rotation_.x, rotation_.y, rotation_.z);

	UpdateMatrix();

}

void ModuleComponentsTransform::SetScale(const float3& scale_)
{
	scale = scale_;
	UpdateMatrix();

}

float3 ModuleComponentsTransform::GetGlobalPosition()
{
	return GlobalMatrix.TranslatePart();
}