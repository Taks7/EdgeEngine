#pragma once

#include "JsonParsing.h"

#include "PhysFS/include/physfs.h"
#include "Assimp/include/assimp/cfileio.h"

#include <list>
#include <string>

typedef unsigned int uint;
enum class ResourceType;

class ModuleFileSystem
{
public:
	ModuleFileSystem() {}
	ModuleFileSystem(const char* assetsPath);

	~ModuleFileSystem();

	bool Init(JsonParsing& node);

	bool CleanUp();

	bool AddPath(const char* path);

	uint Load(const char* file, char** buffer);
	uint Save(const char* file, const void* buffer, unsigned int size, bool append = false);

	inline aiFileIO* GetAssimpIO() const { return assimpIO; }

	inline const char* GetBasePath() const { return PHYSFS_getBaseDir(); }
	inline const char* GetWritePath() const { return PHYSFS_getWriteDir(); }
	const char* GetReadPaths() const;
	std::string GetFileExtension(const char* path);

private:
	const char* name;
	std::list<std::string> texExtension;
	std::list<std::string> modelExtension;

	aiFileIO* assimpIO;
};