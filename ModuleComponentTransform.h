#ifndef _ModuleComponentTransform_H_
#define _ModuleComponentTransform_H_

#include "Module.h"
#include "ModuleGameObject.h"
#include "MathGeo/src/Math/Quat.h"
#include "MathGeo/src/Math/float4x4.h"
#include "MathGeo/src/Math/float3.h"
#include "MathGeo/src/Math/float3x3.h"
#include "MathGeo/src/Math/TransformOps.h"
#include <string>

class ModuleGameObject;

class ModuleComponentsTransform : public ModuleComponents
{
public:
	ModuleComponentsTransform(ModuleGameObject* owner);
	virtual ~ModuleComponentsTransform();

	virtual bool	Update();
	virtual bool	CleanUp();

	void SyncLocalToWorld();
	void SyncWorldToLocal();

	void UpdateWorldTransform();
	void UpdateLocalTransform();


	
	//LOCAL
	void SetLocalTransform(const float4x4& localTransform);
	
	void SetPosition(const float3& position_);
	void SetRotation(const float3& rotation_);
	void SetScale(const float3& scale_);
	void SetLocalEulerRotation(const float3& euler_rotation);

	float3 GetPosition() const;
	Quat GetRotation() const;
	float3 GetScale() const;
	float3 GetLocalEulerRotation() const;
	float4x4 GetLocalTransform() const;

	//WORLD
	void SetWorldTransform(const float4x4& world_transform);

	void SetGlobalPosition(const float3& position_);
	void SetGlobalRotation(const float3& rotation_);
	void SetGlobalScale(const float3& scale_);

	float4x4 GetGlobalMatrix() const;
	float3 GetGlobalPosition() const;
	Quat GetGlobalRotation() const;
	float3 GetGlobalScale() const;
	float3 GetGlobalEulerRotation() const;

	void Translate(const float3& velocity);
	

private:

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
	//LOCAL
	void UpdateMatrix();
	//WORLD
	void UpdateGlobalMatrix();
};

#endif // !__ComponentMesh_H__