
#include "Application.h"
#include "Module.h"
#include "ModuleComponent.h"
#include "ModuleComponentTransform.h"
#include "ModuleComponentCamera.h"

#include "ModuleGameObject.h"

ModuleComponentsTransform::ModuleComponentsTransform(ModuleGameObject* owner) : ModuleComponents(owner, COMPONENT_TYPES::TRANSFORM, "Transform")
{
	local_transform = float4x4::identity;
	world_transform = float4x4::identity;

	local_transform.Decompose(local_position, local_rotation, local_scale);
	local_euler_rotation = local_rotation.ToEulerXYX();
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
		world_transform = newTransform->world_transform * local_transform;
	}
	else
	{
		world_transform = local_transform;

	}

	for (int i = 0; i < owner->childs.size(); i++)
	{
		ModuleComponentsTransform* newTransform_child = (ModuleComponentsTransform*)owner->childs[i]->GetComponent(COMPONENT_TYPES::TRANSFORM);
		newTransform_child->world_transform = local_transform;
	}

}

void ModuleComponentsTransform::UpdateWorldTransform()
{
	ModuleGameObject* owner = GetOwner();

	if (owner->parent != nullptr)
	{
		ModuleComponentsTransform* worldLocalTransform = (ModuleComponentsTransform*)owner->parent->GetComponent(COMPONENT_TYPES::TRANSFORM);

		world_transform = worldLocalTransform->world_transform * worldLocalTransform->local_transform;
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

void ModuleComponentsTransform::SyncWorldToLocal()
{
	ModuleGameObject* owner = GetOwner();

	if (owner->parent != nullptr)
	{
		ModuleComponentsTransform* transform = (ModuleComponentsTransform*)owner->parent->GetComponent(COMPONENT_TYPES::TRANSFORM);

		world_transform = transform->world_transform * local_transform;
	}
	else
	{
		world_transform = local_transform;
	}

	for (uint i = 0; i < owner->childs.size(); ++i)
	{

		ModuleComponentsTransform* tramsform = (ModuleComponentsTransform*)owner->childs[i]->GetComponent(COMPONENT_TYPES::TRANSFORM);
		tramsform->UpdateWorldTransform();
		//owner->childs[i]->GetTransformComponent()->update_world_transform = true;
	}
}
void ModuleComponentsTransform::UpdateMatrix()
{
	local_transform = float4x4::FromTRS(local_position, local_rotation, local_scale);
	UpdateGlobalMatrix();
}

float4x4 ModuleComponentsTransform::GetGlobalMatrix() const
{
	return world_transform;
}
float3 ModuleComponentsTransform::GetPosition() const
{
	return local_position;
}

Quat ModuleComponentsTransform::GetRotation() const
{
	return local_rotation;
}

float3 ModuleComponentsTransform::GetLocalEulerRotation() const
{
	return local_euler_rotation;
}
float3 ModuleComponentsTransform::GetScale() const
{
	return local_scale;
}

void ModuleComponentsTransform::SetPosition(const float3& position_)
{
	local_position = position_;
	UpdateMatrix();

}

void ModuleComponentsTransform::SetRotation(const float3& rotation_)
{
	local_rotation = Quat::FromEulerXYZ(rotation_.x, rotation_.y, rotation_.z);

	local_euler_rotation = rotation_;
	UpdateMatrix();

}
void ModuleComponentsTransform::SetLocalEulerRotation(const float3& euler_rotation)
{
	float3 new_euler = euler_rotation * DEGTORAD;
	local_rotation = Quat::FromEulerXYZ(new_euler.x, new_euler.y, new_euler.z);
	local_euler_rotation = new_euler;

	UpdateLocalTransform();
}
void ModuleComponentsTransform::SetScale(const float3& scale_)
{
	local_scale = scale_;
	UpdateMatrix();

}
void ModuleComponentsTransform::SetLocalTransform(const float4x4& localTransform)
{
	local_transform = localTransform;

	local_position = local_transform.TranslatePart();
	float3 euler = local_transform.RotatePart().ToEulerXYZ();
	local_rotation = Quat::FromEulerXYZ(euler.x, euler.y, euler.z);
	local_scale = local_transform.GetScale();

	local_euler_rotation = euler;

	UpdateWorldTransform();
}

float4x4 ModuleComponentsTransform::GetLocalTransform() const
{
	return local_transform;
}
float3 ModuleComponentsTransform::GetGlobalPosition() const
{
	return world_transform.TranslatePart();
}

void ModuleComponentsTransform::SetWorldTransform(const float4x4& world_transform)
{
	this->world_transform = world_transform;

	SyncLocalToWorld();
}

void ModuleComponentsTransform::SetGlobalPosition(const float3& position_)
{
	world_transform.SetTranslatePart(position_);

	SyncLocalToWorld();
}
void ModuleComponentsTransform::SetGlobalRotation(const float3& rotation_)
{
	world_transform.SetRotatePart(Quat::FromEulerXYZ(rotation_.x, rotation_.y, rotation_.z));

	SyncLocalToWorld();
}
void ModuleComponentsTransform::SetGlobalScale(const float3& scale_)
{
	world_transform.Scale(world_transform.GetScale().Neg());

	if (scale_.x == 0.0f || scale_.y == 0.0f || scale_.z == 0.0f)
	{
		world_transform.Scale(float3(0.1f, 0.1f, 0.1f));
	}
	else
	{
		world_transform.Scale(scale_);
	}

	SyncLocalToWorld();
}

Quat ModuleComponentsTransform::GetGlobalRotation() const
{
	return world_transform.RotatePart().ToQuat();
}

float3 ModuleComponentsTransform::GetGlobalScale() const
{
	return world_transform.GetScale();
}

float3 ModuleComponentsTransform::GetGlobalEulerRotation() const
{
	return world_transform.RotatePart().ToEulerXYZ();
}

void ModuleComponentsTransform::Translate(const float3& velocity)
{
	local_position += velocity;

	UpdateLocalTransform();
}

void ModuleComponentsTransform::SetWorldPosition(const float3& new_position)
{
	world_transform.SetTranslatePart(new_position);

	SyncLocalToWorld();
}

float3 ModuleComponentsTransform::GetWorldPosition() const
{
	return world_transform.TranslatePart();
}