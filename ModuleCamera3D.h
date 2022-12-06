#pragma once
#include "Module.h"
#include "ModuleGameObject.h"
#include "ModuleComponent.h"
#include "ModuleComponentCamera.h"
#include "Globals.h"
#include "glmath.h"
#include "MathGeo/src/Geometry/LineSegment.h";
#include "MathGeo/src/Geometry/Frustum.h"

class ModuleCamera3D : public Module
{
public:
	ModuleCamera3D(bool start_enabled = true);
	~ModuleCamera3D();

	bool Init();
	bool Start();
	bool Update(float dt) override;
	bool CleanUp();

	float3 GetPosition() const;
	void Look(const vec3 &Position, const vec3 &Reference, bool RotateAroundReference = false);
	void LookAt(const vec3 &Spot);
	void LookAtFloat3(const float3& Spot);
	void FreeMovement();
	void Move(const vec3 &Movement);
	void MoveCameras(const float3& velocity);
	float* GetViewMatrix();
	void CastRay();
	void CreateGameCamera();
	ModuleComponentCamera* GetCurrentCamera() const;
	void SetCurrentCamera(ModuleComponentCamera* c_camera);
	void SetMasterCameraAsCurrentCamera();
private:

	void CalculateViewMatrix();
	void Zoom();
	void SetZoomSpeed(const float& zoom_speed);
	float GetZoomSpeed() const;
	vec3 ToVec3(float3 float3);
	float3 ToFloat3(vec3 vec3);

public:

	vec3 X, Y, Z, Position, Reference;

	float movementSpeed;
	float rotationSpeed;
	float zoomSpeed;

	Frustum frustum;

	LineSegment	last_raycast;

	ModuleGameObject* game_camera;
	ModuleComponentCamera* current_camera;

	float3 reference;

private:

	mat4x4 ViewMatrix, ViewMatrixInverse;
};