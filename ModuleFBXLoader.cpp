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
#include "ModuleGameObject.h"
#include "ModuleSceneIntro.h"
#include "ModuleComponentMesh.h"
#include "ModuleComponent.h"
#include "ModuleComponentMaterial.h"
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

	//LoadMesh("Assets/BakerHouse.fbx","Assets/Resources/Baker_House.png");
	App->loaderModels->LoadMeshToGameObject(App->scene_intro->CreateEmptyGameObject("house", nullptr), "Assets/BakerHouse.fbx", "Assets/Resources/Baker_House.png");

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

//bool ModuleFBXLoader::LoadMesh(const char* file_path,const char* texturePath)
//{
//	const aiScene* scene = aiImportFile(file_path, aiProcessPreset_TargetRealtime_MaxQuality);
//	if (scene != nullptr && scene->HasMeshes())
//	{
//		VertexData NewMesh;
//		// Use scene->mNumMeshes to iterate on scene->mMeshes array
//		for (unsigned int i = 0; i < scene->mNumMeshes; i++)
//		{
//			NewMesh.num_vertex = scene->mMeshes[i]->mNumVertices;
//			NewMesh.vertex = new float[NewMesh.num_vertex*3];
//
//			memcpy(NewMesh.vertex, scene->mMeshes[i]->mVertices, sizeof(float3) * NewMesh.num_vertex);
//			
//			glGenBuffers(1, &NewMesh.id_vertex);
//			glBindBuffer(GL_ARRAY_BUFFER, NewMesh.id_vertex);
//			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * NewMesh.num_vertex * 3, NewMesh.vertex, GL_STATIC_DRAW);
//			glBindBuffer(GL_ARRAY_BUFFER, 0);
//
//			LOG_COMMENT("New mesh with %d vertices", NewMesh.num_vertex);
//
//			// copy faces
//			if (scene->mMeshes[i]->HasFaces())
//			{
//				NewMesh.num_index = scene->mMeshes[i]->mNumFaces * 3;
//				NewMesh.index = new GLuint[NewMesh.num_index]; // assume each face is a triangle
//
//				
//				for (uint z = 0; z < scene->mMeshes[i]->mNumFaces; z++)
//				{
//					if (scene->mMeshes[i]->mFaces[z].mNumIndices != 3)
//					{
//						LOG_COMMENT("WARNING, geometry face with != 3 indices!");
//
//					}
//					else
//					{
//						memcpy(&NewMesh.index[z * 3], scene->mMeshes[i]->mFaces[z].mIndices, 3 * sizeof(uint));
//
//						
//					}
//				}
//
//				glGenBuffers(1, &NewMesh.id_index);
//				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NewMesh.id_index);
//				glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * NewMesh.num_index, NewMesh.index, GL_STATIC_DRAW);
//				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
//				
//			}
//
//			if (scene->mMeshes[i]->HasTextureCoords(0))
//			{
//				NewMesh.num_uvs = scene->mMeshes[i]->mNumVertices;
//
//				NewMesh.textCords = new float[NewMesh.num_uvs * 3];
//
//				memcpy(NewMesh.textCords,scene->mMeshes[i]->mTextureCoords[0],NewMesh.num_uvs* sizeof(float3));
//				int x = scene->mMeshes[i]->mNumUVComponents[0];
//			}
//			NewMesh.texture_data.id = scene->mMeshes[i]->mMaterialIndex;
//
//			glGenBuffers(1, &(NewMesh.id_uvs));
//			glBindBuffer(GL_ARRAY_BUFFER, NewMesh.id_uvs);
//			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * NewMesh.num_uvs * 3, NewMesh.textCords, GL_STATIC_DRAW);
//			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
//
//
//			if (texturePath != nullptr)
//			{
//				App->materialImport->Import(texturePath, &NewMesh);
//			}
//
//			meshes.push_back(NewMesh);
//			
//		}
//
//		return true;
//	}
//	else
//	{
//		LOG_COMMENT("Error loading scene % s",file_path);
//	}
//
//	aiReleaseImport(scene);
//	 
//	
//}

bool ModuleFBXLoader::LoadMeshToGameObject(ModuleGameObject* owner,const char* file_path, const char* texturePath)
{
	const aiScene* scene = aiImportFile(file_path, aiProcessPreset_TargetRealtime_MaxQuality);
	if (scene != nullptr && scene->HasMeshes())
	{
		
		// Use scene->mNumMeshes to iterate on scene->mMeshes array
		for (unsigned int i = 0; i < scene->mNumMeshes; i++)
		{
			
			
			ModuleComponentsMesh* NewMesh; 
			ModuleComponentMaterial* MaterialUsed;
			Texture* textureUsed = new Texture();
			if (i == 0)
			{
				NewMesh = (ModuleComponentsMesh*)owner->GetComponent(COMPONENT_TYPES::MESH);
				MaterialUsed = (ModuleComponentMaterial*)owner->GetComponent(COMPONENT_TYPES::MATERIAL);
				
				
			}
			if (i > 0)
			{
				ModuleGameObject* child = App->scene_intro->CreateEmptyGameObject("child",owner);

				NewMesh = (ModuleComponentsMesh*)child->GetComponent(COMPONENT_TYPES::MESH);
				MaterialUsed = (ModuleComponentMaterial*)child->GetComponent(COMPONENT_TYPES::MATERIAL);
			}
			
			NewMesh->mesh.num_vertex = scene->mMeshes[i]->mNumVertices;
			NewMesh->mesh.vertex = new float[NewMesh->mesh.num_vertex * 3];

			memcpy(NewMesh->mesh.vertex, scene->mMeshes[i]->mVertices, sizeof(float3) * NewMesh->mesh.num_vertex);

			glGenBuffers(1, &NewMesh->mesh.id_vertex);
			glBindBuffer(GL_ARRAY_BUFFER, NewMesh->mesh.id_vertex);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * NewMesh->mesh.num_vertex * 3, NewMesh->mesh.vertex, GL_STATIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			LOG_COMMENT("New mesh with %d vertices", NewMesh->mesh.num_vertex);

			// copy faces
			if (scene->mMeshes[i]->HasFaces())
			{
				NewMesh->mesh.num_index = scene->mMeshes[i]->mNumFaces * 3;
				NewMesh->mesh.index = new GLuint[NewMesh->mesh.num_index]; // assume each face is a triangle


				for (uint z = 0; z < scene->mMeshes[i]->mNumFaces; z++)
				{
					if (scene->mMeshes[i]->mFaces[z].mNumIndices != 3)
					{
						LOG_COMMENT("WARNING, geometry face with != 3 indices!");

					}
					else
					{
						memcpy(&NewMesh->mesh.index[z * 3], scene->mMeshes[i]->mFaces[z].mIndices, 3 * sizeof(uint));


					}
				}

				glGenBuffers(1, &NewMesh->mesh.id_index);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NewMesh->mesh.id_index);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * NewMesh->mesh.num_index, NewMesh->mesh.index, GL_STATIC_DRAW);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

			}

			if (scene->mMeshes[i]->HasTextureCoords(0))
			{
				NewMesh->mesh.num_uvs = scene->mMeshes[i]->mNumVertices;

				NewMesh->mesh.textCords = new float[NewMesh->mesh.num_uvs * 3];

				memcpy(NewMesh->mesh.textCords, scene->mMeshes[i]->mTextureCoords[0], NewMesh->mesh.num_uvs * sizeof(float3));
				int x = scene->mMeshes[i]->mNumUVComponents[0];
			}
			NewMesh->mesh.texture_data.id = scene->mMeshes[i]->mMaterialIndex;


			glGenBuffers(1, &(NewMesh->mesh.id_uvs));
			glBindBuffer(GL_ARRAY_BUFFER, NewMesh->mesh.id_uvs);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * NewMesh->mesh.num_uvs * 3, NewMesh->mesh.textCords, GL_STATIC_DRAW);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


			if (texturePath != nullptr)
			{
				App->materialImport->Import(texturePath,textureUsed);
				MaterialUsed->materialUsed = textureUsed;
				MaterialUsed->textures.push_back(textureUsed);
			}

			//meshes.push_back(NewMesh);

		}

		return true;
	}
	else
	{
		LOG_COMMENT("Error loading scene % s", file_path);
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
