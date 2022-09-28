#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "Light.h"

#define MAX_LIGHTS 8
struct Gl_Attributes
{
	bool Depth_test = false;
	bool Cull_Face = false;
	bool Lightning = false;
	bool Color_Materials = false;
	bool Texture_2D = false;
};
class ModuleRenderer3D : public Module
{
public:
	ModuleRenderer3D(bool start_enabled = true);
	~ModuleRenderer3D();

	bool Init();
	bool PreUpdate(float dt) override;
	bool PostUpdate();
	bool CleanUp();

	void OnResize(int width, int height);


public:

	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	mat3x3 NormalMatrix;
	mat4x4 ModelMatrix, ViewMatrix, ProjectionMatrix;
	Gl_Attributes atributes;

};