#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "Frustum.h"

class ModuleCamera3D : public Module
{
public:
	ModuleCamera3D(bool start_enabled = true);
	~ModuleCamera3D();

	bool Start();
	bool Update(float dt) override;
	bool CleanUp();

	void Look(const vec3 &Position, const vec3 &Reference, bool RotateAroundReference = false);
	void LookAt(const vec3 &Spot);
	void Move(const vec3 &Movement);
	float* GetViewMatrix();

private:

	void CalculateViewMatrix();
	void Zoom();
	void SetZoomSpeed(const float& zoom_speed);
	float GetZoomSpeed() const;
	vec3 ToVec3(float3 float3);
	float3 ToFloat3(vec3 vec3);

public:
	
	vec3 X, Y, Z, Position, Reference;

	float zoomSpeed;

	Frustum frustum;

private:

	mat4x4 ViewMatrix, ViewMatrixInverse;
};