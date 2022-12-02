
#include "Globals.h"
#include "Module.h"
#include "ModuleComponent.h"
#include "ModuleGameObject.h"
#include "ModuleComponentCamera.h"
#include "ModuleFBXLoader.h"

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

Frustum ModuleComponentCamera::GetFrustum() const
{
	return frustum;
}
