#ifndef __ModuleFBXLoader_H__
#define __ModuleFBXLoader_H__

#include "Module.h"
#include "SDL/include/SDL.h"

class Application;
struct VertexData
{
	uint id_index = 0; // index in VRAM
	uint num_index = 0;
	uint* index = nullptr;
	uint id_vertex = 0; // unique vertex in VRAM
	uint num_vertex = 0;
	float* vertex = nullptr;
};
class ModuleFBXLoader : public Module
{
public:

	ModuleFBXLoader(bool start_enabled = true);

	// Destructor
	virtual ~ModuleFBXLoader();

	bool Init();
	bool CleanUp();

	VertexData LoadMesh(const char* file_path);
	bool LoadConfig(JsonParsing& node) override;
	bool SaveConfig(JsonParsing& node) const override;

public:
	
};

#endif // __ModuleWindow_H__