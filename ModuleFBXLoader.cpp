#include "Globals.h"
#include "Application.h"
#include "ModuleFBXLoader.h"
#include "ModuleRenderer3D.h"
#include "SDL/include/SDL_opengl.h"

#include "Assimp/include/assimp/cimport.h"
#include "Assimp/include/assimp/scene.h"
#include "Assimp/include/assimp/postprocess.h"
#include "Assimp/include/assimp/ai_assert.h"

#include <vector>

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
	 
	//---------------------------------------------------------------------------------------------
	//char* buffer = nullptr;
	//uint file_size = App->fs->Load(file_path, &buffer);
	//if (file_size > 0)
	//{
	//	const aiScene* scene = aiImportFileFromMemory(buffer, file_size, aiProcessPreset_TargetRealtime_MaxQuality, nullptr);

	//	if (scene == nullptr || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	//	{

	//	}

	//	for (uint i = 0; i < scene->mRootNode->mNumMeshes; i++)
	//	{
	//		aiMesh* aimesh = scene->mMeshes[scene->mRootNode->mMeshes[i]];

	//		if (aimesh != nullptr && aimesh->HasFaces())
	//		{
	//			VertexData newMesh;

	//			uint vertices_size = aimesh->mNumVertices * 3;
	//			newMesh.vertices.resize(vertices_size);

	//			uint normals_size = aimesh->mNumVertices * 3;
	//			newMesh.normals.resize(normals_size);

	//			uint indices_size = aimesh->mNumFaces * 3;
	//			newMesh.indices.resize(indices_size);

	//			if (aimesh->HasPositions())
	//			{
	//				memcpy(&newMesh.vertices[0], aimesh->mVertices, sizeof(float) * vertices_size);
	//			}

	//			if (aimesh->HasNormals())
	//			{
	//				memcpy(&newMesh.normals[0], aimesh->mNormals, sizeof(float) * normals_size);
	//			}

	//			if (aimesh->HasFaces())
	//			{
	//				uint ind = 0;

	//				for (uint i = 0; i < aimesh->mNumFaces; i++)
	//				{
	//					aiFace face = aimesh->mFaces[i];

	//					if (face.mNumIndices == 3)
	//					{
	//						for (uint j = 0; j < face.mNumIndices; j++)
	//						{
	//							newMesh.indices[ind] = face.mIndices[j];
	//							ind++;
	//						}
	//					}
	//				}
	//			}

	//			//Poner un metodo para que si newMesh != nullptr
	//			//Haga un push_back de la (newMesh) en un vecto mesh
	//			//Y que borre la newMesh:
	//			//delete newMesh
	//			//newMesh = nullptr

	//			if (newMesh != nullptr)
	//			{
	//				meshes.push_back(newMesh);
	//			}
	//			else
	//			{
	//				delete newMesh;
	//				newMesh = nullptr;
	//			}

	//			return newMesh;
	//		}
	//	}

	//}
}
bool ModuleFBXLoader::LoadConfig(JsonParsing& node)
{

	return true;
}

bool ModuleFBXLoader::SaveConfig(JsonParsing& node) const
{

	return true;
}
