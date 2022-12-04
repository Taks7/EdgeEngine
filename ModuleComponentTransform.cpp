
#include "Application.h"
#include "Module.h"
#include "ModuleComponent.h"
#include "ModuleComponentTransform.h"
#include "ModuleComponentCamera.h"

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

void ModuleComponentsTransform::UpdateWorldTransform()
{
	ModuleGameObject* owner = GetOwner();

	if (owner->parent != nullptr)
	{
		ModuleComponentsTransform* worldLocalTransform = (ModuleComponentsTransform*)owner->parent->GetComponent(COMPONENT_TYPES::TRANSFORM);

		worldLocalTransform->world_transform * worldLocalTransform->local_transform;
	}
	else
	{
		world_transform = local_transform;
	}

	for (uint i = 0; i < owner->childs.size(); ++i)
	{
		//owner->childs[i]->GetTransformComponent()->update_world_transform = true;

		ModuleComponentsTransform* worldTransformUpdate = (ModuleComponentsTransform*)owner->childs[i]->GetComponent(COMPONENT_TYPES::TRANSFORM);
		worldTransformUpdate->UpdateWorldTransform();
	}

	if (owner->GetComponent(COMPONENT_TYPES::CAMERA) != nullptr)
	{
		//owner->GetCameraComponent()->SetUpdateFrustumTransform(true);

		ModuleComponentCamera* updateCamera = (ModuleComponentCamera*)owner->GetComponent(COMPONENT_TYPES::CAMERA);
		updateCamera->UpdateFrustumTransform();
	}

}

void ModuleComponentsTransform::UpdateLocalTransform()
{
	local_transform = float4x4::FromTRS(local_position, local_rotation, local_scale);

	UpdateWorldTransform();
}

void ModuleComponentsTransform::SyncLocalToWorld()
{
	ModuleGameObject* owner = GetOwner();

	if (owner->parent != nullptr)
	{
		ModuleComponentsTransform* invertWorldTransform = (ModuleComponentsTransform*)owner->parent->GetComponent(COMPONENT_TYPES::TRANSFORM);
		local_transform = invertWorldTransform->world_transform.Inverted() * world_transform;
	}
	else
	{
		local_transform = world_transform;
	}

	local_position = local_transform.TranslatePart();
	float3 euler = local_transform.RotatePart().ToEulerXYZ();
	local_rotation = Quat::FromEulerXYZ(euler.x, euler.y, euler.z);
	local_scale = local_transform.GetScale();

	local_euler_rotation = euler;

	for (uint i = 0; i < owner->childs.size(); ++i)
	{
		ModuleComponentsTransform* updateWorldTransform = (ModuleComponentsTransform*)owner->childs[i]->GetComponent(COMPONENT_TYPES::TRANSFORM);
		updateWorldTransform->UpdateWorldTransform();
	}

	if (owner->GetComponent(COMPONENT_TYPES::CAMERA) != nullptr)
	{
		ModuleComponentCamera* updateCameraTransfrom = (ModuleComponentCamera*)owner->GetComponent(COMPONENT_TYPES::CAMERA);
		updateCameraTransfrom->UpdateFrustumTransform();
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

float4x4 ModuleComponentsTransform::GetWorldTransform() const
{
	return world_transform;
}

void ModuleComponentsTransform::SetWorldTransform(const float4x4& world_transform)
{
	this->world_transform = world_transform;

	SyncLocalToWorld();
}

void ModuleComponentsTransform::SetLocalPosition(const float3& new_position)
{
	local_position = new_position;

	UpdateLocalTransform();
}