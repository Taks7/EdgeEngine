#include "Globals.h"
#include "Application.h"
#include "ModuleFBXLoader.h"
#include "ModuleRenderer3D.h"
#include "ModuleMaterials.h"
#include "glew/glew-2.2.0/include/GL/glew.h"
#include <gl/GL.h>
#include <gl/GLU.h>
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

	LoadMesh("Assets/BakerHouse.fbx","Assets/Resources/Baker_House.png");

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

bool ModuleFBXLoader::LoadMesh(const char* file_path,const char* texturePath)
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

			memcpy(NewMesh.vertex, scene->mMeshes[i]->mVertices, sizeof(float3) * NewMesh.num_vertex);
			
			glGenBuffers(1, &NewMesh.id_vertex);
			glBindBuffer(GL_ARRAY_BUFFER, NewMesh.id_vertex);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * NewMesh.num_vertex * 3, NewMesh.vertex, GL_STATIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

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

				glGenBuffers(1, &NewMesh.id_index);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NewMesh.id_index);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * NewMesh.num_index, NewMesh.index, GL_STATIC_DRAW);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
				
			}

			if (scene->mMeshes[i]->HasTextureCoords(0))
			{
				NewMesh.num_uvs = scene->mMeshes[i]->mNumVertices;

				NewMesh.textCords = new float[NewMesh.num_uvs * 3];

				memcpy(NewMesh.textCords,scene->mMeshes[i]->mTextureCoords[0],NewMesh.num_uvs* sizeof(float3));
				int x = scene->mMeshes[i]->mNumUVComponents[0];
			}
			NewMesh.texture_data.id = scene->mMeshes[i]->mMaterialIndex;

			glGenBuffers(1, &(NewMesh.id_uvs));
			glBindBuffer(GL_ARRAY_BUFFER, NewMesh.id_uvs);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * NewMesh.num_uvs * 3, NewMesh.textCords, GL_STATIC_DRAW);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


			if (texturePath != nullptr)
			{
				App->materialImport->Import(texturePath, &NewMesh);
			}

			meshes.push_back(NewMesh);
		}

		return true;
	}
	else
	{
		LOG_COMMENT("Error loading scene % s",file_path);
	}

	aiReleaseImport(scene);
	 
	
}
bool ModuleFBXLoader::LoadConfig(JsonParsing& node)
{

	return true;
}

bool ModuleFBXLoader::SaveConfig(JsonParsing& node) const
{

	return true;
}
