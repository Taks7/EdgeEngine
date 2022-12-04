#ifndef _ModuleComponentTransform_H_
#define _ModuleComponentTransform_H_

#include "Module.h"
#include "ModuleGameObject.h"
#include "MathGeo/src/Math/Quat.h"
#include "MathGeo/src/Math/float4x4.h"
#include "MathGeo/src/Math/float3.h"
#include "MathGeo/src/Math/float3x3.h"
#include <string>

class ModuleGameObject;

class ModuleComponentsTransform : public ModuleComponents
{
public:
	ModuleComponentsTransform(ModuleGameObject* owner);
	virtual ~ModuleComponentsTransform();

	virtual bool	Update();
	virtual bool	CleanUp();

	void Transform();

	float3 GetPosition() const;
	float3 GetRotation() const;
	float3 GetScale() const;

	void SetPosition(const float3& position_);
	void SetRotation(const float3& rotation_);
	void SetScale(const float3& scale_);

	void UpdateWorldTransform();
	void UpdateLocalTransform();

	void SyncLocalToWorld();

	float4x4 GetGlobalMatrix();
	float3 GetGlobalPosition();

	float4x4 GetWorldTransform() const;
	void SetWorldTransform(const float4x4& world_transform);
	void SetLocalPosition(const float3& new_position);

private:

	float4x4	matrix;

	float4x4	GlobalMatrix;

	float3		position = {0,0,0};
	Quat	rotation;
	float3		scale = {1,1,1};

	float3		euler_rotation = {0,0,0};

	float4x4	local_transform;													
	float4x4	world_transform;		

	float3		local_position;														
	Quat		local_rotation;														
	float3		local_scale;														

	float3		local_euler_rotation;

private:
	const char* name;
	bool		is_active;

private: 
	void UpdateMatrix();
	void UpdateGlobalMatrix();
};

#endif // !__ComponentMesh_H__