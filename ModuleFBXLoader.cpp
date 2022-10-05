#include "Globals.h"
#include "Application.h"
#include "ModuleFBXLoader.h"
#include "ModuleRenderer3D.h"
#include "SDL/include/SDL_opengl.h"

#include "Assimp/include/assimp/cimport.h"
#include "Assimp/include/assimp/scene.h"
#include "Assimp/include/assimp/postprocess.h"

#pragma comment (lib, "Assimp/lib/assimp-vc142-mt.lib")

ModuleFBXLoader::ModuleFBXLoader(bool start_enabled) : Module(start_enabled)
{
	name = "FBXloader";
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

VertexData ModuleFBXLoader::LoadMesh(const char* file_path)
{
	const aiScene* scene = aiImportFile(file_path, aiProcessPreset_TargetRealtime_MaxQuality);
	if (scene != nullptr && scene->HasMeshes())
	{
		VertexData NewMesh;
		// Use scene->mNumMeshes to iterate on scene->mMeshes array
		for (unsigned int i = 0; i < scene->mNumMeshes; i++)
		{
			NewMesh.num_vertex = scene->mMeshes[i]->mNumVertices;
			NewMesh.vertex = new float[NewMesh.num_vertex*3];

			memcpy(NewMesh.vertex, scene->mMeshes[i]->mVertices, sizeof(float) * NewMesh.num_vertex * 3);
			LOG_COMMENT("New mesh with %d vertices", NewMesh.num_vertex);

			// copy faces
			if (scene->mMeshes[i]->HasFaces())
			{
				NewMesh.num_index = scene->mMeshes[i]->mNumFaces * 3;
				NewMesh.index = new GLuint[NewMesh.num_index]; // assume each face is a triangle


				for (uint z = 0; z < scene->mMeshes[i]->mNumFaces; z++)
				{
					if (scene->mMeshes[i]->mFaces[z].mNumIndices != 3)
					{
						LOG_COMMENT("WARNING, geometry face with != 3 indices!");

					}
					else
					{
						memcpy(&NewMesh.index[z * 3], scene->mMeshes[i]->mFaces[z].mIndices, 3 * sizeof(uint));
					}
				}
				
			}
		}
		aiReleaseImport(scene);

		return NewMesh;
	}
	else
	{
		LOG_COMMENT("Error loading scene % s",file_path);
	}

}
bool ModuleFBXLoader::LoadConfig(JsonParsing& node)
{

	return true;
}

bool ModuleFBXLoader::SaveConfig(JsonParsing& node) const
{

	return true;
}
