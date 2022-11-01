#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "Light.h"
#include "ModuleFBXLoader.h"

#define MAX_LIGHTS 8
#define CWIDTH 64
#define CHEIGHT 64

struct Gl_Attributes
{
	bool Depth_test;
	bool Cull_Face;
	bool Lightning;
	bool Color_Materials;
	bool Texture_2D;
	bool Front;
	bool AmbientOclussion;
	bool Wireframe;
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
	void DrawGameObjects(ModuleGameObject gameObject);

	bool LoadConfig(JsonParsing& node) override;
	bool SaveConfig(JsonParsing& node) const override;

	void			LoadCheckerTexture();

public:

	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	mat3x3 NormalMatrix;
	mat4x4 ModelMatrix, ViewMatrix, ProjectionMatrix;
	Gl_Attributes atributes;
	uint ckeckerTextureid;
	bool checkerTextureApplied = false;

};