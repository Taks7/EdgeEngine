#ifndef _ModuleComponentTransform_H_
#define _ModuleComponentTransform_H_

#include "Module.h"
#include "ModuleGameObject.h"
#include "Math/float4x4.h"
#include "Math/float3.h"
#include <string>

class ModuleGameObject;

class ModuleComponentsTransform : ModuleComponents
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
	void SetRotation(const float3& rotation);
	void SetScale(const float3& scale);

public:

	float4x4	matrix;

	float3		position;
	float4x4	rotation;
	float3		scale;

	float3		euler_rotation;

public:
	COMPONENT_TYPES	type;
	ModuleGameObject* owner;

private:
	const char* name;
	bool		is_active;
};

#endif // !__ComponentMesh_H__