#include "Globals.h"
#include "Application.h"
#include "ModuleFBXLoader.h"
#include "ModuleRenderer3D.h"

#include "Assimp/include/assimp/cimport.h"
#include "Assimp/include/assimp/scene.h"
#include "Assimp/include/assimp/postprocess.h"

#pragma comment (lib, "Assimp/lib/assimp-vc142-mt.lib")

ModuleFBXLoader::ModuleFBXLoader(bool start_enabled) : Module(start_enabled)
{
	
}

// Destructor
ModuleFBXLoader::~ModuleFBXLoader()
{
}

// Called before render is available
bool ModuleFBXLoader::Init()
{
	LOG_COMMENT("Init FBX loader");

	// Stream log messages to Debug window
	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);

	return true;
}

// Called before quitting
bool ModuleFBXLoader::CleanUp()
{
	LOG_COMMENT("Cleaning up FBX loader");

	// detach log stream
	aiDetachAllLogStreams();
	return true;
}

bool ModuleFBXLoader::LoadConfig(JsonParsing& node)
{

	return true;
}

bool ModuleFBXLoader::SaveConfig(JsonParsing& node) const
{

	return true;
}
