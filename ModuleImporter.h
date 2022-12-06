#ifndef __ModuleImporter_H__
#define __ModuleImporter_H__

#include "Application.h"
#include "Module.h"
#include "ModuleGameObject.h"
#include "ModuleComponentMesh.h"
#include "ModuleComponentMaterial.h"
#include "ModuleComponentTransform.h"

#include "glew/glew-2.2.0/include/GL/glew.h"

#define CHECKERS_HEIGHT 50
#define CHECKERS_WIDTH 50

class ModuleComponentsMesh;
class ModuleComponentMaterial;
enum class TextureMaterialType
{
	NONE = 0,
	DIFFUSE,
	METALLIC,
	ROUGHNESS,
	HEIGH_MAP,
	NORMAL_MAP,
	EMISSIVE
};

struct TextureData
{
	TextureMaterialType materialType;
	uint texture_ID = 0;
	uint image_ID = 0;

	uint width = 0;
	uint height = 0;

	uint format = 0;
	uint internalFormat = 0;

	const void* texture = nullptr;
	std::string	path;
};

struct MeshVertexData
{
	uint id_index = 0; 
	uint num_indices = 0;
	uint* indices = nullptr;

	uint id_vertex = 0; 
	uint num_vertices = 0;
	float* vertices = nullptr;


	uint id_UV = 0; 
	uint num_UVs = 0;
	GLfloat* texture_coords_indices = nullptr;

	TextureData meshTexturesData;
	std::string	path;
};

class ModuleImporter : public Module
{
public:
	ModuleImporter(bool start_enabled = true);
	~ModuleImporter();

	bool Init();
	bool Update(float dt);
	bool CleanUp();

	bool Save_Mesh(ModuleComponentsMesh* mesh, char** pointer);

	bool Load_Mesh(ModuleComponentsMesh* mesh, char* pointer);

	bool Save_Texture(ModuleComponentMaterial* texture, char** pointer);

	bool Load_Texture(ModuleComponentMaterial* texture, char** pointer, uint size);

	void LoadModel_Textured(ModuleGameObject* objMain, const char* meshPath, const char* texturePath); 
	uint LoadTexture(ModuleGameObject* objMain, const char* path); 

	uint CheckerTexture();

public:

	uint childPostfix = 0;
	uint textPostfix = 0;

};

#endif