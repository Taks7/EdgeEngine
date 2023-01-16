#ifndef __ModuleFBXLoader_H__
#define __ModuleFBXLoader_H__

#include "Module.h"
#include "ModuleComponentMaterial.h"
#include "Assimp/include/assimp/material.h"
#include "SDL/include/SDL.h"
#include "MathGeo/src/Math/float2.h"
#include "glew/glew-2.2.0/include/GL/glew.h"
#include "ModuleGameObject.h"
#include "MathGeo/src/Algorithm/Random/LCG.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include "Globals.h"

class Application;
enum class TEXTURE_TYPE
{
	NONE = 0x0,
	DIFFUSE = 0x1,
	SPECULAR = 0x2,
	UNKNOWN = 0xC
};

struct Texture
{

	std::string		path;
	TEXTURE_TYPE	type;
	uint			id;
	uint			width;
	uint			height;
	uint			uid;

	uint64 Save_texture(Texture* texture, char** buffer);
	uint Import(const char* path, Texture* texture);
	bool Load(Texture* Texture);

	void SetTextureData(uint id, uint width, uint height);

	
		uint CreateTexture(const void* data, uint width,
			uint height,
			uint target = 0x0DE1, 				// 0x0DE1 = GL_TEXTURE_2D
			int filter = 0x2600,				// 0x2600 = GL_NEAREST
			int wrapping = 0x2901,				// 0x2901 = GL_REPEAT
			int internal_format = 0x1908,		// 0x1908 = GL_RGBA
			uint format = 0x1908); 				// 0x1908 = GL_RGBA
	
};
struct VertexData
{
	

	GLuint  id_index = 0; // index in VRAM
	GLuint  num_index = 0;
	GLuint* index = nullptr;
	GLuint  id_vertex = 0; // unique vertex in VRAM
	GLuint  num_vertex = 0;
	float* vertex = nullptr;

	GLfloat* textCords = nullptr;
	GLuint num_uvs = 0;
	GLuint id_uvs = 0;

	Texture texture_data;

	AABB aabb;

	AABB GetAABB();
	void SetAABB();

	void Draw();
	void GenerateBillboardGPU();

	uint64	Save(const VertexData* mesh, char** buffer);
};
class ModuleFBXLoader : public Module
{
public:

	ModuleFBXLoader(bool start_enabled = true);

	// Destructor
	virtual ~ModuleFBXLoader();

	bool Init();
	bool CleanUp();

	bool LoadMesh(const char* file_path,const char* texture_path);
	bool LoadMeshToGameObject(ModuleGameObject* owner,const char* file_path, const char* texture_path);
	//bool LoadMeshToGameObject(ModuleGameObject* owner, const char* file_path, const char* texture_path);
	bool LoadConfig(JsonParsing& node) override;
	bool SaveConfig(JsonParsing& node) const override;

	bool LoadScene(JsonParsing& node) override;
	bool SaveScene(JsonParsing& node) const override;

	VertexData* getBillboard();

	void GenerateBillboard();

public:
	std::vector<VertexData> meshes;
	std::vector<VertexData*> textures;

	VertexData* billboard;
};

#endif // __ModuleWindow_H__