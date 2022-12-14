
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Globals.h"
#include "Module.h"
#include "ModuleComponent.h"
#include "ModuleComponentTransform.h"
#include "ModuleGameObject.h"
#include "ModuleComponentCamera.h"
#include "ModuleCamera3D.h"
#include "ModuleFBXLoader.h"

#include <vector>
#include "MathGeoLib.h"
#pragma comment (lib, "MathGeo/lib/MathGeoLib.lib")

#define MAX_FRUSTUM_PLANES		6																			
#define MAX_FRUSTUM_VERTICES	8																	

ModuleComponentCamera::ModuleComponentCamera(ModuleGameObject* owner) : ModuleComponents(owner, COMPONENT_TYPES::CAMERA, "Camera"),
frustum_planes(nullptr),
frustum_vertices(nullptr),
show_culling(false),
show_orthogonal(false),
hide_frustum(false),
projection_update(true)
{
	frustum_planes = new Plane[MAX_FRUSTUM_PLANES];
	frustum_vertices = new float3[MAX_FRUSTUM_VERTICES];

	InitFrustum();
}

ModuleComponentCamera::~ModuleComponentCamera()
{
	delete[] frustum_planes;
	delete[] frustum_vertices;
}

bool ModuleComponentCamera::Update()
{
	bool ret = true;

	return ret;
}

bool ModuleComponentCamera::CleanUp()
{
	bool ret = true;

	if (is_culling)
	{
		App->scene_intro->SetCullingCamera(nullptr);
	}

	return ret;
}

void ModuleComponentCamera::InitFrustum()
{
	frustum.SetKind(FrustumSpaceGL, FrustumRightHanded);

	frustum.SetPos(float3(0.0f, 0.0f, 0.0f));
	frustum.SetFront(float3::unitZ);
	frustum.SetUp(float3::unitY);

	frustum.SetViewPlaneDistances(1.0f, 300.0f);
	frustum.SetPerspective(1.0f, 1.0f);

	UpdateFrustumPlanes();
	UpdateFrustumVertices();
}

void ModuleComponentCamera::UpdateFrustumTransform()
{

	ModuleComponentsTransform* getWorldTransfrom = (ModuleComponentsTransform*)this->GetOwner()->GetComponent(COMPONENT_TYPES::TRANSFORM);
	float4x4 world_transform = getWorldTransfrom->GetGlobalMatrix();

	float3x4 world_matrix = float3x4::identity;

	world_matrix.SetTranslatePart(world_transform.TranslatePart());
	world_matrix.SetRotatePart(world_transform.RotatePart());
	world_matrix.Scale(world_transform.GetScale());

	frustum.SetWorldMatrix(world_matrix);

	UpdateFrustumPlanes();
	UpdateFrustumVertices();

	projection_update = true;
}

Frustum ModuleComponentCamera::GetFrustum() const
{
	return frustum;
}
void ModuleComponentCamera::SetUpdateProjectionMatrix(const bool& set_to)
{
	projection_update = set_to;
}

bool ModuleComponentCamera::GetUpdateProjectionMatrix() const
{
	return projection_update;
}

float* ModuleComponentCamera::GetOGLViewMatrix()
{
	static float4x4 view_matrix;

	view_matrix = frustum.ViewMatrix();
	view_matrix.Transpose();

	return (float*)view_matrix.v;
}

float* ModuleComponentCamera::GetOGLProjectionMatrix()
{
	static float4x4 projection_matrix;

	projection_matrix = frustum.ProjectionMatrix().Transposed();

	return (float*)projection_matrix.v;
}

void ModuleComponentCamera::UpdateFrustumPlanes()
{
	frustum.GetPlanes(frustum_planes);
}

void ModuleComponentCamera::UpdateFrustumVertices()
{
	frustum.GetCornerPoints(frustum_vertices);
}

Plane* ModuleComponentCamera::GetFrustumPlanes() const
{
	return frustum_planes;
}

float3* ModuleComponentCamera::GetFrustumVertices() const
{
	return frustum_vertices;
}

void ModuleComponentCamera::SetFarPlaneDistance(const float& far_distance)
{
	if (far_distance < 0)
	{
		LOG_COMMENT("[ERROR] Far distance < 0.");
		return;
	}

	if (far_distance < frustum.NearPlaneDistance())
	{
		LOG_COMMENT("[ERROR] Far distance < Near distance");
		return;
	}

	frustum.SetViewPlaneDistances(frustum.NearPlaneDistance(), far_distance);

	UpdateFrustumPlanes();
	UpdateFrustumVertices();

	projection_update = true;
}

void ModuleComponentCamera::SetAspectRatio(const float& aspect_ratio)
{
	frustum.SetHorizontalFovAndAspectRatio(frustum.HorizontalFov(), aspect_ratio);

	UpdateFrustumPlanes();
	UpdateFrustumVertices();

	projection_update = true;
}

void ModuleComponentCamera::SetFrustumIsHidden(const bool& set_to)
{
	hide_frustum = set_to;
}

void ModuleComponentCamera::LookAt(const float3& target)
{
	
	float3 new_Z = float3(target - frustum.Pos()).Normalized();											
	float3x3 look_at_matrix = float3x3::LookAt(frustum.Front(), new_Z, frustum.Up(), float3::unitY);				
	frustum.SetFront(look_at_matrix.MulDir(frustum.Front()).Normalized());
	frustum.SetUp(look_at_matrix.MulDir(frustum.Up()).Normalized());

	float4x4 world_matrix = frustum.WorldMatrix();

	ModuleComponentsTransform* transfromWorldMatrix;
	transfromWorldMatrix = (ModuleComponentsTransform*)this->GetOwner()->GetComponent(COMPONENT_TYPES::TRANSFORM);
	transfromWorldMatrix->SetWorldTransform(world_matrix);


}

void ModuleComponentCamera::Focus(const float3& target, const float& distance_to_target)
{
	float abs_distance = (distance_to_target < 0) ? -distance_to_target : distance_to_target;

	float3 distance = float3(target - frustum.Pos()).Normalized();

	float3 position = frustum.Pos() - target;

	position = target + distance * position.Length();

	PointAt(position, target);
}

void ModuleComponentCamera::SetPosition(const float3& position)
{
	ModuleComponentsTransform* setPosition;
	setPosition = (ModuleComponentsTransform*)this->GetOwner()->GetComponent(COMPONENT_TYPES::TRANSFORM);
	setPosition->SetWorldPosition(position);
}

void ModuleComponentCamera::Move(const float3& velocity)
{
	ModuleComponentsTransform* moveCameras;
	moveCameras = (ModuleComponentsTransform*)this->GetOwner()->GetComponent(COMPONENT_TYPES::TRANSFORM);
	moveCameras->Translate(velocity);
}

void ModuleComponentCamera::PointAt(const float3& position, const float3& target)
{
	SetPosition(position);
	LookAt(target);
}

float ModuleComponentCamera::GetNearPlaneDistance() const
{
	return frustum.NearPlaneDistance();
}

float ModuleComponentCamera::GetFarPlaneDistance() const
{
	return frustum.FarPlaneDistance();
}

float ModuleComponentCamera::GetHorizontalFOV() const
{
	return (frustum.HorizontalFov() * RADTODEG);
}

float ModuleComponentCamera::GetVerticalFOV() const
{
	return (frustum.VerticalFov() * RADTODEG);
}

void ModuleComponentCamera::SetNearPlaneDistance(const float& near_distance)
{
	if (near_distance < 0)
	{
		LOG_COMMENT("[ERROR] Component Camera: Cannot set the new distance of the near plane! Error: New near distance < 0.");
		return;
	}

	if (near_distance > frustum.FarPlaneDistance())
	{
		LOG_COMMENT("[ERROR] Component Camera: Cannot set the new distance of the near plane! Error: New near distance > far distance.");
		return;
	}

	frustum.SetViewPlaneDistances(near_distance, frustum.FarPlaneDistance());

	UpdateFrustumPlanes();
	UpdateFrustumVertices();

	projection_update = true;
}

void ModuleComponentCamera::SetHorizontalFOV(const float& horizontal_fov)
{
	frustum.SetHorizontalFovAndAspectRatio((horizontal_fov * DEGTORAD), frustum.AspectRatio());

	UpdateFrustumPlanes();
	UpdateFrustumVertices();

	projection_update = true;
}

void ModuleComponentCamera::SetVerticalFOV(const float& vertical_fov)
{
	frustum.SetVerticalFovAndAspectRatio((vertical_fov * DEGTORAD), frustum.AspectRatio());

	UpdateFrustumPlanes();
	UpdateFrustumVertices();

	projection_update = true;
}

void ModuleComponentCamera::GetMinMaxFOV(uint& min_fov, uint& max_fov) const
{
	min_fov = this->min_fov;
	max_fov = this->max_fov;
}

void ModuleComponentCamera::SetMinMaxFOV(const uint& min_fov, const uint& max_fov)
{
	this->min_fov = min_fov;
	this->max_fov = max_fov;
}

bool ModuleComponentCamera::IsCulling() const
{
	return is_culling;
}

bool ModuleComponentCamera::OrthogonalView() const
{
	return in_orthogonal_view;
}

bool ModuleComponentCamera::FrustumIsHidden() const
{
	return hide_frustum;
}

void ModuleComponentCamera::SetIsCulling(const bool& set_to)
{
	is_culling = set_to;

	if (set_to)
	{
		App->scene_intro->SetCullingCamera(this);
	}
	else
	{
		if (App->scene_intro->GetCullingCamera() == this)
		{
			App->scene_intro->SetCullingCamera(nullptr);
		}
	}
}

void ModuleComponentCamera::SetOrthogonalView(const bool& set_to)
{
	in_orthogonal_view = set_to;

	if (in_orthogonal_view)
	{

	}
}

//Frustum ModuleComponentCamera::GetFrustum() const
//{
//	return frustum;
//}
