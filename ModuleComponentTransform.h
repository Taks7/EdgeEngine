#ifndef _ModuleComponentTransform_H_
#define _ModuleComponentTransform_H_

#include "Module.h"
#include "ModuleGameObject.h"
#include "MathGeo/src/Math/float4x4.h"
#include "MathGeo/src/Math/float3.h"
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

	void SetPosition(const float3& position);
	void SetRotation(const float3& rotation_);
	void SetScale(const float3& scale);

	void UpdateMatrix();

public:

	float4x4	matrix;

	float3		position = {0,0,0};
	Quat	rotation;
	float3		scale = {1,1,1};

	float3		euler_rotation = {0,0,0};



private:
	const char* name;
	bool		is_active;
};

#endif // !__ComponentMesh_H__