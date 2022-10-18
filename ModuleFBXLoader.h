#ifndef __ModuleFBXLoader_H__
#define __ModuleFBXLoader_H__

#include "Module.h"
#include "material.h"
#include "SDL/include/SDL.h"
#include "float2.h"
#include "glew/glew-2.2.0/include/GL/glew.h"
#include <gl/GL.h>
#include <gl/GLU.h>

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
	bool LoadConfig(JsonParsing& node) override;
	bool SaveConfig(JsonParsing& node) const override;

public:
	std::vector<VertexData> meshes;
	std::vector<VertexData*> textures;
};

#endif // __ModuleWindow_H__