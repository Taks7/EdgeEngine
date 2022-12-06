#ifndef __ModuleComponentCamera_H__
#define __ModuleComponentCamera_H__

#include "Module.h"
#include "ModuleComponent.h"
#include "MathGeo/src/Math/float2.h"
#include "ModuleGameObject.h"
#include "MathGeo/src/Geometry/Frustum.h"
#include "MathGeo/src/Geometry/Plane.h"

class ModuleGameObject;

class ModuleComponentCamera : public ModuleComponents
{
public:
	ModuleComponentCamera(ModuleGameObject* owner);
	~ModuleComponentCamera();

	bool	Update();
	bool	CleanUp();

public:																										
	void InitFrustum();
	void UpdateFrustumTransform();
	Frustum	GetFrustum() const;

	void SetUpdateProjectionMatrix(const bool& set);
	bool GetUpdateProjectionMatrix() const;

	float* GetOGLViewMatrix();
	float* GetOGLProjectionMatrix();

public:																					
	void UpdateFrustumPlanes();
	void UpdateFrustumVertices();
	Plane* GetFrustumPlanes() const;
	float3* GetFrustumVertices() const;

	bool FrustumCointainsAABB(const AABB& aabb) const;										
	bool FrustumIntersectsAABB(const AABB& aabb) const;

	void SetFarPlaneDistance(const float& far_distance);
	void SetAspectRatio(const float& aspect_ratio);
	void SetFrustumIsHidden(const bool& set_to);

	void LookAt(const float3& target);

	void Move(const float3& velocity);


private:
	Frustum frustum;																						

	Plane* frustum_planes;																					
	float3* frustum_vertices;																				
	bool	show_culling;																						
	bool	show_orthogonal;																			
	bool	hide_frustum;																					
	bool	projection_update;																		
};

#endif // !__ComponentMesh_H__
