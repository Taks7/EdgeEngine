#ifndef __ModuleMaterials_H__
#define __ModuleMaterials_H__

#include "Globals.h"
#include "Resource.h"
#include "Module.h"
#include "SDL/include/SDL.h"
#include "ModuleWindow.h"
#include "glew/glew-2.2.0/include/GL/glew.h"
#include <vector>
#include <string>

struct aiScene;
struct aiMaterials;

enum class TEXTURE_TYPE									// The enum values are set according to the values of Assimp's aiTextureType enum.
{
	NONE = 0x0,
	DIFFUSE = 0x1,
	SPECULAR = 0x2,
	UNKNOWN = 0xC
};

struct Texture
{

	std::string		path;								// Path of the file in the directory. Will be used to avoid making duplicates.
	TEXTURE_TYPE	type;								// Diffuse or Specular.
	uint			id;									// Id of the texture.
	uint			width;								// Width of the texture in pixels.
	uint			height;								// Height of the texture in pixels.
};

struct MaterialData
{
	GLuint  id_index = 0; // index in VRAM
	GLuint  num_index = 0;
	GLuint* index = nullptr;
	GLuint  id_vertex = 0; // unique vertex in VRAM
	GLuint  num_vertex = 0;
	float* vertex = nullptr;
	//std::vector<float>		vertices;
	//std::vector<float>		normals;
	//std::vector<uint>		indices;
	Texture texture_data;

};

class ModuleMaterial : public Module
{
public:

	bool Init();

	bool CleanUp();

	bool Import(const char* path, MaterialData* r_material);
	//bool Import(const char* buffer, uint size, R_Material* r_material);

	uint CreateTexture(const void* data, uint width,
		uint height,
		int internal_format = 0x1908,		// 0x1908 = GL_RGBA
		uint format = 0x1908,				// 0x1908 = GL_RGBA
		uint target = 0x0DE1,				// 0x0DE1 = GL_TEXTURE_2D
		int filter_type = 0x2600,			// 0x2600 = GL_NEAREST
		int filling_type = 0x2901);		// 0x2901 = GL_REPEAT

	uint64 Save(const MaterialData* r_material, char** buffer);

	void Load(const char* buffer, uint size, MaterialData* r_material);

};

#endif // !__ModuleMaterials_H__