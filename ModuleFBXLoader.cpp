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
#include "FSdefinitions.h"
#include "DevIL/include/il.h"
#include "DevIL.h"
#include <vector>

#pragma comment (lib, "Assimp/lib/assimp-vc142-mt.lib")


#define HEADER_SIZE 8

ModuleFBXLoader::ModuleFBXLoader(bool start_enabled) : Module(start_enabled)
{
	name = "FBXloader";

	const char* dirs[] = {
		ASSETS_SCENES_PATH
	};

	for (uint i = 0; i < sizeof(dirs) / sizeof(const char*); ++i)
	{
		if (PHYSFS_exists(dirs[i]) == 0)
			PHYSFS_mkdir(dirs[i]);
	}

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
	App->loaderModels->LoadMeshToGameObject(App->scene_intro->CreateEmptyGameObject("house", nullptr), "Assets/Models/BakerHouse.fbx", "Assets/Textures/Baker_House.png");

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

bool ModuleFBXLoader::LoadMeshToGameObject(ModuleGameObject* owner,const char* file_path, const char* texturePath)
{
	const aiScene* scene = aiImportFile(file_path, aiProcessPreset_TargetRealtime_MaxQuality);
	
	//if(texturePath!= nullptr) 
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

				NewMesh->meshPath = file_path;
				
			}
			if (i > 0)
			{
				ModuleGameObject* child = App->scene_intro->CreateEmptyGameObject("child",owner);

				NewMesh = (ModuleComponentsMesh*)child->GetComponent(COMPONENT_TYPES::MESH);
				MaterialUsed = (ModuleComponentMaterial*)child->GetComponent(COMPONENT_TYPES::MATERIAL);

				NewMesh->meshPath = file_path;

			}
			
			NewMesh->mesh.num_vertex = scene->mMeshes[i]->mNumVertices;
			NewMesh->mesh.vertex = new float[NewMesh->mesh.num_vertex * 3];

			memcpy(NewMesh->mesh.vertex, scene->mMeshes[i]->mVertices, sizeof(float3) * NewMesh->mesh.num_vertex);

			//CREATING AABB FOR EACH MESH
			NewMesh->mesh.aabb.SetNegativeInfinity();
			NewMesh->mesh.aabb.Enclose((float3*)&NewMesh->mesh.vertex[0], NewMesh->mesh.num_vertex);

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

AABB VertexData::GetAABB()
{
	return aabb;
}


uint64 Texture::Save_texture(Texture* texture, char** buffer)
{
	uint64 written = 0;

	std::string directory = LIBRARY_TEXTURES_PATH;																	
	std::string file = std::to_string(texture->uid) + std::string(TEXTURES_EXTENSION);				 
	std::string full_path = directory + file;																	

	ilEnable(IL_FILE_OVERWRITE);																				
	ilSetInteger(IL_DXTC_FORMAT, IL_DXT5);																		

	ILuint size = ilSaveL(IL_DDS, nullptr, 0);																	
	if (size > 0)
	{
		ILubyte* data = new ILubyte[size];																		

		if (ilSaveL(IL_DDS, data, size) > 0)																	
		{
			*buffer = (char*)data;
			written = App->fs->Save(full_path.c_str(), *buffer, size, false);							

			if (written > 0)
			{
				LOG_COMMENT("[IMPORTER] Successfully saved %s in %s", file.c_str(), directory.c_str());
			}
			else
			{
				*buffer = nullptr;
				LOG_COMMENT("[ERROR] Could not save %s!", file.c_str());
			}
		}
		else
		{
			LOG_COMMENT("[ERROR] Could not save the texture! ilSaveL() Error: %s", iluErrorString(ilGetError()));
		}
	}
	else
	{
		LOG_COMMENT("[ERROR] Could not get the size of the texture to save! ilSaveL() Error: %s", iluErrorString(ilGetError()));
	}

	directory.clear();
	file.clear();
	full_path.clear();

	return written;
}

uint Texture::Import(const char* path, Texture* texture)											// FIX: FLIP TEXTURE
{
	uint tex_id = 0;

	LOG_COMMENT("[IMPORTER] Loading %s texture.", path);

	if (path == nullptr || texture == nullptr)
	{
		LOG_COMMENT("[ERROR] Texture could not be imported: Path and/or R_Texture* was nullptr!");
		return 0;
	}

	char* tex_data = nullptr;
	uint file_size = App->fs->Load(path, &tex_data);

	if (tex_data != nullptr && file_size > 0)
	{
		bool success = ilLoadL(IL_TYPE_UNKNOWN, (const void*)tex_data, file_size);
		if (success)
		{
			char* buffer = nullptr;
			uint written = Texture::Save_texture(texture, &buffer);

			if (buffer != nullptr && written > 0)
			{
				//texture->SetAssetsPath(path);
				//texture->SetAssetsFile(App->fs->GetFileAndExtension(path).c_str());

				//Importer::Textures::Load(buffer, written, r_texture);
				success = Texture::Load(texture);
				if (success)
				{
					tex_id = texture->uid;
					LOG_COMMENT("[IMPORTER] Successfully loaded %s from Library!", texture->path);
				}
				else
				{
					LOG_COMMENT("[IMPORTER] Could not load %s from Library!", texture->path);
				}
			}
			else
			{
				LOG_COMMENT("[ERROR] Could not Save() %s in the Library!", path);
			}

			RELEASE_ARRAY(buffer);
		}
		else
		{
			LOG_COMMENT("[ERROR] Could not load %s from Assets! ilLoadL() Error: %s", path, iluErrorString(ilGetError()));
		}
	}
	else
	{
		LOG_COMMENT("[ERROR] File System could not load %s!", path);
	}

	RELEASE_ARRAY(tex_data);

	return tex_id;
}

bool Texture::Load(Texture* texture)
{
	bool ret = true;

	char* tex_data = nullptr;																			
	uint file_size = App->fs->Load(texture->path.c_str(), &tex_data);
	uint texture_id;

	if (tex_data == nullptr || file_size == 0)
	{
		LOG_COMMENT("[ERROR] File System could not load tex data! Path: %s", ASSETS_TEXTURES_PATH);
		return false;
	}

	ILuint il_image = 0;																				// Will be used to generate, bind and delete the buffers created by DevIL.
	ilGenImages(1, &il_image);																			// DevIL's buffers work pretty much the same way as OpenGL's do.
	ilBindImage(il_image);																				// The first step is to generate a buffer and then bind it.

	bool success = ilLoadL(IL_TYPE_UNKNOWN, (const void*)tex_data, file_size);							// ilLoadL() loads a texture from some buffer data. size == 0 = no bounds check.
	if (success)																						// --- When type is IL_TYPE_UNKNOWN, DevIL will try to find the type on it's own.
	{
		uint color_channels = ilGetInteger(IL_IMAGE_CHANNELS);
		if (color_channels == 3)
		{
			success = ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE);											// ilConvertImage() will convert the image to the given format and type.
		}
		else if (color_channels == 4)
		{
			success = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);										// ilConvertImage() will return false if the system cannot store the image with
		}																								// its new format or the operation is invalid (no bound img. or invalid identifier).
		else
		{
			LOG_COMMENT("[WARNING] Texture has less than 3 color channels! Path: %s", ASSETS_TEXTURES_PATH);
		}

		if (success)
		{
			ILinfo il_info;
			iluGetImageInfo(&il_info);

			if (il_info.Origin == IL_ORIGIN_UPPER_LEFT)
			{
				iluFlipImage();
			}
			uint width = il_info.Width;															
			uint height = il_info.Height;															
			uint depth = il_info.Depth;															
			uint bpp = il_info.Bpp;																
			uint size = il_info.SizeOfData;														
			uint format = il_info.Format;															
			uint target = (uint)GL_TEXTURE_2D;														
			int wrapping = (int)GL_REPEAT;															
			int filter = (int)GL_LINEAR;
			uint tex_id = texture->CreateTexture(ilGetData(), width, height, target, wrapping, filter, format, format);	

			if (tex_id != 0)																							
			{
				
				texture->SetTextureData(tex_id, width, height);
			
			}
			else
			{
				LOG_COMMENT("[ERROR] Could not get texture ID! Path: %s", ASSETS_TEXTURES_PATH);
				ret = false;
			}
		}
		else
		{
			LOG_COMMENT("[ERROR] ilConvertImage() Error: %s", iluErrorString(ilGetError()));
			ret = false;
		}
	}
	else
	{
		LOG_COMMENT("[ERROR] ilLoadL() Error: %s", iluErrorString(ilGetError()));
		ret = false;
	}

	ilDeleteImages(1, &il_image);

	return ret;
}


void Texture::SetTextureData(uint id, uint width, uint height)
{
	Texture::id = id;
	Texture::width = width;
	Texture::height = height;
}

uint Texture::CreateTexture(const void* data, uint width, uint height, uint target, int wrapping, int filter, int internal_format, uint format)
{
	uint texture_id = 0;

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glGenTextures(1, (GLuint*)&texture_id);
	glBindTexture(target, texture_id);

	glTexParameteri(target, GL_TEXTURE_WRAP_S, wrapping);
	glTexParameteri(target, GL_TEXTURE_WRAP_T, wrapping);

	if (filter == GL_NEAREST)																					
	{
		glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	}
	else if (filter == GL_LINEAR)																				
	{
		glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		if (glewIsSupported("GL_EXT_texture_filter_anisotropic"))												
		{
			GLfloat max_anisotropy;

			glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &max_anisotropy);
			glTexParameteri(target, GL_TEXTURE_MAX_ANISOTROPY_EXT, (GLint)max_anisotropy);
		}
	}
	else
	{
		LOG_COMMENT("[ERROR] Invalid filter type! Supported filters: GL_LINEAR and GL_NEAREST.");
	}

	glTexImage2D(target, 0, internal_format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

	glGenerateMipmap(target);

	glBindTexture(target, 0);

	if (texture_id != 0)
	{
		LOG_COMMENT("[STATUS] Texture Successfully loaded! Id: %u, Size: %u x %u", texture_id, width, height);
	}

	return texture_id;
}

uint64 VertexData::Save(const VertexData* mesh, char** buffer)
{
	uint64 written = 0;

	uint header_data[HEADER_SIZE] = {
		mesh->num_vertex,																
		mesh->num_index,																
		mesh->num_uvs,																	
		mesh->textCords[0],															
		mesh->vertex[0], 																				
		mesh->index[0], 																				
	};

	uint header_data_size = sizeof(header_data) + sizeof(uint);
	uint array_data_size = (header_data[0] + header_data[1] + header_data[2]) * sizeof(float) + header_data[3] * sizeof(uint);
	uint precalculated_data_size = mesh->aabb.NumVertices() * sizeof(float) * 3;
	uint size = header_data_size + array_data_size + precalculated_data_size;
	

	if (size == 0)
	{
		LOG_COMMENT("[WARNING] Mesh had no data to Save!");
		return 0;
	}

	*buffer = new char[size];
	char* cursor = *buffer;
	uint bytes = 0;

	// --- HEADER DATA ---

	// --- VERTEX ARRAY DATA ---
	bytes = mesh->num_vertex * sizeof(float);
	memcpy_s(cursor, size, &mesh->vertex[0], bytes);
	cursor += bytes;

	bytes = mesh->num_uvs * sizeof(float);
	memcpy_s(cursor, size, &mesh->textCords[0], bytes);
	cursor += bytes;

	bytes = mesh->num_index * sizeof(uint);
	memcpy_s(cursor, size, &mesh->index[0], bytes);
	cursor += bytes;

	// --- PRECALCULATED DATA ---
	math::vec* aabb_corners = new math::vec[8];
	mesh->aabb.GetCornerPoints(aabb_corners);

	bytes = mesh->aabb.NumVertices() * sizeof(float) * 3;
	memcpy_s(cursor, size, aabb_corners, bytes);
	cursor += bytes;

	delete[] aabb_corners;

	// --- SAVING THE BUFFER ---
	std::string path = std::string(LIBRARY_MESHES_PATH) + std::to_string(mesh->id_index) + std::string(MESHES_EXTENSION);

	written = App->fs->Save(path.c_str(), *buffer, size);

	if (written > 0)
	{
		//LOG_COMMENT("[IMPORTER] Meshes: Successfully Saved %s into the Library!", mesh->GetAssetsFile());
	}
	else
	{
		//LOG_COMMENT("[ERROR] Meshes Importer: Could not Save %s into the Library!", mesh->GetAssetsFile());
	}

	path.clear();

	return written;
}

bool ModuleFBXLoader::SaveScene(JsonParsing& node) const
{
	node.SetNewJsonString(node.ValueToObject(node.GetRootValue()), "Parent mesh", App->scene_intro->selectedGameObject->GetMeshPath().c_str());
	node.SetNewJsonString(node.ValueToObject(node.GetRootValue()), "Parent texture", App->scene_intro->selectedGameObject->GetTexturePath().c_str());
	node.SetNewJson3Number(node.ValueToObject(node.GetRootValue()), "Parent position", App->scene_intro->selectedGameObject->GetGlobalPosition());
	node.SetNewJson4Number(node.ValueToObject(node.GetRootValue()), "Parent rotation", App->scene_intro->selectedGameObject->GetGlobalRotation());
	node.SetNewJson3Number(node.ValueToObject(node.GetRootValue()), "Parent scale", App->scene_intro->selectedGameObject->GetGlobalScale());
	for (int i = 0 ; i < App->scene_intro->selectedGameObject->childs.size(); i++)
	{
		node.SetNewJsonString(node.ValueToObject(node.GetRootValue()), "child texture" , App->scene_intro->selectedGameObject->childs.at(i)->GetTexturePath().c_str());

	}

	return true;
}
bool ModuleFBXLoader::LoadScene(JsonParsing& node)
{

	return true;
}