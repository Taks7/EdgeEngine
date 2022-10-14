#ifndef __ModuleMaterials_H__
#define __ModuleMaterials_H__

#include "Globals.h"
#include "Resource.h"
#include "Module.h"
#include "SDL/include/SDL.h"
#include "ModuleWindow.h"
#include "ModuleFBXLoader.h"
#include "cimport.h"
#include "material.h"
#include "glew/glew-2.2.0/include/GL/glew.h"
#include <vector>
#include <string>

struct aiScene;
struct aiMaterials;

//struct MaterialData
//{
//	GLuint  id_index = 0; // index in VRAM
//	GLuint  num_index = 0;
//	GLuint* index = nullptr;
//	GLuint  id_vertex = 0; // unique vertex in VRAM
//	GLuint  num_vertex = 0;
//	float* vertex = nullptr;
//	//std::vector<float>		vertices;
//	//std::vector<float>		normals;
//	//std::vector<uint>		indices;
//	Texture texture_data;
//
//};

class ModuleMaterial : public Module
{
public:

	bool Init();

	bool CleanUp();

	bool Import(const char* path, VertexData* newMaterial);
	//bool Import(const char* buffer, uint size, R_Material* r_material);

	uint CreateTexture(const void* data, uint width,
		uint height,
		int internal_format = 0x1908,		
		uint format = 0x1908,				
		uint target = 0x0DE1,				
		int filter_type = 0x2600,			
		int filling_type = 0x2901);

	/*void SetMaterial(VertexData* material);*/

	/*aiMaterial material;*/

};

#endif // !__ModuleMaterials_H__