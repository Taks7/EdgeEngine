#ifndef _MESH_IMPORTER_H_
#define _MESH_IMPORTER_H_

//#include "Application.h"
#include "Module.h"
#include "ModuleGameObject.h"
#include "ModuleComponentMesh.h"
#include "ModuleComponentMaterial.h"
#include "ModuleComponentTransform.h"

#include "glew/glew-2.2.0/include/GL/glew.h"

#define CHECKERS_HEIGHT 50
#define CHECKERS_WIDTH 50

//struct TransformComponent;


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

	uint id_index = 0; // index in VRAM
	uint num_indices = 0;
	uint* indices = nullptr;

	uint id_vertex = 0; // unique vertex in VRAM
	uint num_vertices = 0;
	float* vertices = nullptr;

	/*GLuint id_normal = 0;
	aiVector3D num_normals[3] = {};
	float* normals = nullptr;
	GLuint id_normalVertices = 0;
	GLfloat* normalVertices = nullptr;*/

	uint id_UV = 0; // id of the UV
	uint num_UVs = 0;
	GLfloat* texture_coords_indices = nullptr;

	TextureData meshTexturesData;
	std::string	path;

	//TransformComponent* transform = nullptr;
};

class ModuleModelImport : public Module
{
public:
	ModuleModelImport(bool start_enabled = true);
	~ModuleModelImport();

	bool Init();
	bool Update(float dt);
	bool CleanUp();

	//void Import_Mesh(MeshComponent* mesh); Already done in LoadModel_Textured()
	//void Import_Texture(MeshComponent* mesh); Already done in LoadModel_Textured()

	bool Save_Mesh(ModuleComponentsMesh* mesh, char** pointer);

	bool Load_Mesh(ModuleComponentsMesh* mesh, char* pointer);

	bool Save_Texture(ModuleComponentMaterial* texture, char** pointer);

	bool Load_Texture(ModuleComponentMaterial* texture, char** pointer, uint size);

	void LoadModel_Textured(ModuleGameObject* objMain, const char* meshPath, const char* texturePath); //Loads the model (meshes and its textures) and assigns texture IDs to it
	uint LoadTexture(ModuleGameObject* objMain, const char* path); //Loads the textures

	uint CheckerTexture();

public:
	//std::vector<MeshVertexData> meshes;
	//std::vector<TextureData> textures;

	uint childPostfix = 0;
	uint textPostfix = 0;

};

#endif // !_MESH_IMPORTER_H_
