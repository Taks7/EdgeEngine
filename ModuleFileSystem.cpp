#include "ModuleFileSystem.h"
#include "Application.h"
#include "Globals.h"
#include "Application.h"
#include "ModuleUI.h"
//#include "ModelImporter.h"
//#include "ResourceManager.h"
//#include "ModuleEditor.h"
//#include "GameObject.h"

#include "Assimp/include/assimp/cimport.h"
#include "AssimpDefs.h"

#include <vector>
#include <stack>

#include "SDL/include/SDL_filesystem.h"

#pragma comment (lib, "Assimp/lib/assimp-vc142-mt.lib")

ModuleFileSystem::ModuleFileSystem(const char* assetsPath) : name("ModuleFileSystem")
{
	name = "File System";
	char* basePath = SDL_GetBasePath();
	PHYSFS_init(basePath);
	SDL_free(basePath);

	// workaround VS string directory mess
	AddPath(".");

	if (0 && assetsPath != nullptr)
		AddPath(assetsPath);

	// Dump list of paths
	LOG_COMMENT("FileSystem Operations base is [%s] plus:", GetBasePath());
	LOG_COMMENT(GetReadPaths());

	// enable us to write in the game's dir area
	if (PHYSFS_setWriteDir(".") == 0)
		LOG_COMMENT("File System error while creating write dir: %s\n", PHYSFS_getLastError());

	// Make sure standard paths exist
	//Put all the different folder paths here
	const char* dirs[] = {
		SETTINGS_FOLDER
	};

	for (uint i = 0; i < sizeof(dirs) / sizeof(const char*); ++i)
	{
		if (PHYSFS_exists(dirs[i]) == 0)
			PHYSFS_mkdir(dirs[i]);
	}

	texExtension = { ".png", ".jpg", ".dds", ".tga"};
	modelExtension = { ".obj", ".fbx", ".3DS", ".FBX"};
}

ModuleFileSystem::~ModuleFileSystem()
{
	aiDetachAllLogStreams();
	PHYSFS_deinit();
}

bool ModuleFileSystem::Init(JsonParsing& node)
{
	LOG_COMMENT("Loading File System");
	bool ret = true;

	// Ask SDL for a write dir
	char* writePath = SDL_GetPrefPath(App->GetOrganizationName(), App->GetAppName());

	// Trun this on while in game mode
	//if(PHYSFS_setWriteDir(write_path) == 0)
		//LOG("File System error while creating write dir: %s\n", PHYSFS_getLastError());


	SDL_free(writePath);

	return ret;
}

bool ModuleFileSystem::CleanUp()
{
	return true;
}

bool ModuleFileSystem::AddPath(const char* path)
{
	bool ret = false;

	if (PHYSFS_mount(path, nullptr, 1) == 0)
	{
		LOG_COMMENT("File System error while adding a path or zip: %s\n", PHYSFS_getLastError());
	}
		
	else
	{
		ret = true;
	}
	
	return ret;
}

uint ModuleFileSystem::Load(const char* file, char** buffer)
{
	uint ret = 0;

	PHYSFS_file* fsFile = PHYSFS_openRead(file);

	if (fsFile != nullptr)
	{
		PHYSFS_sint32 size = (PHYSFS_sint32)PHYSFS_fileLength(fsFile);

		if (size > 0)
		{
			*buffer = new char[size];
			uint readed = (uint)PHYSFS_read(fsFile, *buffer, 1, size);
			if (readed != size)
			{
				LOG_COMMENT("File System error while reading from file %s: %s\n", file, PHYSFS_getLastError());
				RELEASE_ARRAY(buffer);
			}
			else
				ret = readed;
		}

		if (PHYSFS_close(fsFile) == 0)
			LOG_COMMENT("File System error while closing file %s: %s\n", file, PHYSFS_getLastError());
	}
	else
		LOG_COMMENT("File System error while opening file %s: %s\n", file, PHYSFS_getLastError());

	return ret;
}

uint ModuleFileSystem::Save(const char* file, const void* buffer, unsigned int size, bool append)
{
	unsigned int ret = 0;

	bool overWrite = PHYSFS_exists(file) != 0;
	PHYSFS_file* fsFile = (append) ? PHYSFS_openAppend(file) : PHYSFS_openWrite(file);

	if (fsFile != nullptr)
	{
		uint written = (uint)PHYSFS_write(fsFile, (const void*)buffer, 1, size);
		if (written != size)
		{
			LOG_COMMENT("File System error while writing to file %s: %s", file, PHYSFS_getLastError());
		}
			
		else
		{
			if (append == true)
			{
				LOG_COMMENT("Added %u data to [%s%s]", size, PHYSFS_getWriteDir(), file);
			}
				
			else if (overWrite == false)
			{
				LOG_COMMENT("New file created [%s%s] of %u bytes", PHYSFS_getWriteDir(), file, size);
			}
				
			ret = written;
		}

		if (PHYSFS_close(fsFile) == 0)
		{
			LOG_COMMENT("File System error while closing file %s: %s", file, PHYSFS_getLastError());
		}
			
	}
	else
	{
		LOG_COMMENT("File System error while opening file %s: %s", file, PHYSFS_getLastError());
	}
		
	return ret;
}

const char* ModuleFileSystem::GetReadPaths() const
{
	static char paths[512];

	paths[0] = '\0';

	char** path;
	for (path = PHYSFS_getSearchPath(); *path != nullptr; ++path)
	{
		strcat_s(paths, 512, *path);
		strcat_s(paths, 512, "\n");
	}

	return paths;
}

std::string ModuleFileSystem::GetFileExtension(const char* path)
{
	std::string full_path = path;
	std::string extension = "";

	size_t dot_position = full_path.find_last_of(".");

	extension = full_path.substr(dot_position + 1);

	return extension;
}


std::string ModuleFileSystem::FixPath(const char* path) const
{
	std::string normalized_path(path);

	for (uint i = 0; i < normalized_path.size(); ++i)
	{
		if (normalized_path[i] == '\\')
		{
			normalized_path[i] = '/';
		}
	}

	return normalized_path;
}

void ModuleFileSystem::DiscoverFilesAndDirs(const char* directory, std::vector<std::string>& fileList, std::vector<std::string>& dirList)
{
	char** rc = PHYSFS_enumerateFiles(directory);
	char** i;

	std::string dir(directory);

	for (i = rc; *i != nullptr; ++i)
	{
		if (PHYSFS_isDirectory((dir + *i).c_str()))
			dirList.push_back(dir + *i + "/");
		else
			fileList.push_back(dir + *i);
	}

	PHYSFS_freeList(rc);
}

void ModuleFileSystem::DiscoverFiles(const char* directory, std::vector<std::string>& file_list) 
{
	char** rc = PHYSFS_enumerateFiles(directory);
	char** i;

	std::string dir(directory);

	for (i = rc; *i != nullptr; ++i)
	{
		if (!PHYSFS_isDirectory((dir + *i).c_str()))
			file_list.push_back(*i);
	}

	PHYSFS_freeList(rc);

}


bool ModuleFileSystem::IsDirectory(const char* file) const
{
	return PHYSFS_isDirectory(file);														
}

const char* ModuleFileSystem::GetValidPath(const char* path)
{
	std::string norm_path = FixPath(path);

	uint dir_path_start = norm_path.find("Assets");
	if (dir_path_start != std::string::npos)
	{
		norm_path = norm_path.substr(dir_path_start, norm_path.size());
		path = _strdup(norm_path.c_str());
	}
	else
	{
		path = nullptr;
	}

	norm_path.clear();

	return path;
}

void ModuleFileSystem::GetFilenameWithExtension(std::string& path)
{
	FixPath(path.c_str());

	if (path.find("/") != std::string::npos)
		path = path.substr(path.find_last_of("/") + 1, path.length());
}

void ModuleFileSystem::GetFilenameWithoutExtension(std::string& path)
{
	FixPath(path.c_str());

	if (path.find("/") != std::string::npos)
	{
		path = path.substr(path.find_last_of("/") + 1, path.length());
	}
	path = path.substr(0, path.find_last_of("."));
}

void ModuleFileSystem::DiscoverDirs(const char* directory, std::vector<std::string>& dirList)
{
	char** rc = PHYSFS_enumerateFiles(directory);
	char** i;

	std::string dir(directory);

	for (i = rc; *i != nullptr; ++i)
	{
		if (PHYSFS_isDirectory((dir + *i).c_str()))
			dirList.push_back(dir + *i + "/");
	}

	PHYSFS_freeList(rc);
}
bool ModuleFileSystem::RemoveFile(const char* file)
{
	bool ret = false;

	if (file != nullptr)
	{
		if (PHYSFS_delete(file) == 0)
		{
			LOG_COMMENT("File deleted: [%s]", file);
			ret = true;
		}
		else
			LOG_COMMENT("File System error while trying to delete [%s]: ", file, PHYSFS_getLastError());
	}

	return ret;
}