#include "Globals.h"
#include "Application.h"
#include "ModuleFBXLoader.h"
#include "ModuleRenderer3D.h"
#include "DevIL.h"
#include "glew/glew-2.2.0/include/GL/glew.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include "Assimp/include/assimp/cimport.h"
#include "Assimp/include/assimp/scene.h"
#include "Assimp/include/assimp/postprocess.h"
#include "Assimp/include/assimp/ai_assert.h"
#include "ModuleFileSystem.h"
#include "ModuleMaterials.h"

#include <vector>

#pragma comment (lib, "Assimp/lib/assimp-vc142-mt.lib")
#pragma comment (lib, "DevIL/libx86/DevIL.lib")
#pragma comment (lib, "DevIL/libx86/ILU.lib")
#pragma comment (lib, "DevIL/libx86/ILUT.lib")


bool ModuleMaterial::Init()
{
	bool ret = true;
	ilInit();																	
	iluInit();																	
	ilutInit();																	
	ilutRenderer(ILUT_OPENGL);

	return ret;
}

bool ModuleMaterial::CleanUp()
{
	bool ret = true;

	ilShutDown();

	return ret;
}

bool ModuleMaterial::Import(const char* file_path, Texture* newMaterial)
{
	bool ret = true;

	uint texture_buffer;
	uint texture_id;

	if (file_path != nullptr)
	{
		ilGenImages(1, (ILuint*)&texture_buffer);
		ilBindImage(texture_buffer);

		char* data = nullptr;
		uint file_size = App->fs->Load(file_path, &data);

		if (file_size > 0)
		{
			ILenum type = IL_TYPE_UNKNOWN;

			if (App->fs->GetFileExtension(file_path) == "png" || App->fs->GetFileExtension(file_path) == "PNG")
			{
				type = IL_PNG;
			}

			if (App->fs->GetFileExtension(file_path) == "dds" || App->fs->GetFileExtension(file_path) == "DDS")
			{
				type = IL_DDS;
			}

			bool loadedSuccessfully = ilLoadL(type, (const void*)data, file_size);

			if (loadedSuccessfully)
			{
				texture_id = ilutGLBindTexImage();

				loadedSuccessfully = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);

				if (loadedSuccessfully)																	
				{
					texture_id = CreateTexture(ilGetData(), ilGetInteger(IL_IMAGE_WIDTH),	
						ilGetInteger(IL_IMAGE_HEIGHT),
						ilGetInteger(IL_IMAGE_FORMAT),
						ilGetInteger(IL_IMAGE_FORMAT),
						GL_TEXTURE_2D, GL_NEAREST, GL_REPEAT);

					if (texture_id != 0)
					{
						newMaterial->path = file_path;
						newMaterial->type = TEXTURE_TYPE::DIFFUSE;
						newMaterial->id = texture_id;
						newMaterial->width = ilGetInteger(IL_IMAGE_WIDTH);
						newMaterial->height = ilGetInteger(IL_IMAGE_HEIGHT);
					}
					else
					{
						ret = false;
					}
				}
				else
				{
					ILenum error = ilGetError();
					ret = false;
				}
			}
			else
			{
				ILenum error = ilGetError();
				ret = false;
			}
		}
		else
		{
			ilDeleteImages(1, &texture_buffer);
			ret = false;
		}
	}

	return ret;
}

uint ModuleMaterial::CreateTexture(const void* data, uint width, uint height, int internal_format, uint format, uint target, int filter_type, int filling_type)
{
	uint textureId = 0;

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glGenTextures(1, (GLuint*)&textureId);
	glBindTexture(target, textureId);

	glTexParameteri(target, GL_TEXTURE_WRAP_S, filling_type);
	glTexParameteri(target, GL_TEXTURE_WRAP_T, filling_type);

	if (filter_type == GL_NEAREST)
	{
		glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	}
	else if (filter_type == GL_LINEAR)
	{
		glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);											
	}

	glTexImage2D(target, 0, internal_format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

	glGenerateMipmap(target);

	glBindTexture(target, 0);

	return textureId;

}
