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

	void SetAspectRatio(const float& aspect_ratio);

	void LookAt(const float3& target);
	void Focus(const float3& target, const float& distance_to_target);
	void SetPosition(const float3& position);
	void Move(const float3& velocity);
	void PointAt(const float3& position, const float3& target);

	float GetNearPlaneDistance() const;
	float GetFarPlaneDistance() const;
	float GetHorizontalFOV() const;
	float GetVerticalFOV() const;

	void SetNearPlaneDistance(const float& near_distance);
	void SetFarPlaneDistance(const float& far_distance);
	void SetHorizontalFOV(const float& horizontal_fov);									
	void SetVerticalFOV(const float& vertical_fov);

	void GetMinMaxFOV(uint& min_fov, uint& max_fov) const;
	void SetMinMaxFOV(const uint& min_fov, const uint& max_fov);																								
	bool IsCulling() const;
	bool OrthogonalView() const;
	bool FrustumIsHidden() const;

	void SetIsCulling(const bool& set_to);
	void SetOrthogonalView(const bool& set_to);
	void SetFrustumIsHidden(const bool& set_to);


private:
	Frustum frustum;																						

	Plane* frustum_planes;																					
	float3* frustum_vertices;																				
	bool	show_culling;																						
	bool	show_orthogonal;																			
	bool	hide_frustum;
	uint	min_fov = 1;																					
	uint	max_fov = 120;																						
	bool	is_culling;																					
	bool	in_orthogonal_view;																				
	bool	projection_update;																		
};

#endif // !__ComponentMesh_H__
