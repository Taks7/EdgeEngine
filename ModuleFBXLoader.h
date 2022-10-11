#ifndef __ModuleFBXLoader_H__
#define __ModuleFBXLoader_H__

#include "Module.h"
#include "SDL/include/SDL.h"
#include "glew/glew-2.2.0/include/GL/glew.h"
#include <gl/GL.h>
#include <gl/GLU.h>

class Application;
struct VertexData
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

};
class ModuleFBXLoader : public Module
{
public:

	ModuleFBXLoader(bool start_enabled = true);

	// Destructor
	virtual ~ModuleFBXLoader();

	bool Init();
	bool CleanUp();

	bool LoadMesh(const char* file_path);
	bool LoadConfig(JsonParsing& node) override;
	bool SaveConfig(JsonParsing& node) const override;

public:
	std::vector<VertexData> meshes;
};

#endif // __ModuleWindow_H__