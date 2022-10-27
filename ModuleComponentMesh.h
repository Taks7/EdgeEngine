#ifndef _ModuleComponentMesh_H_
#define _ModuleComponentMesh_H_

#include "Module.h"
#include "ModuleGameObject.h"
#include "ModuleFBXLoader.h"
#include <string>

class ModuleGameObject;
class ModuleFBXLoader;

class ModuleComponentsMesh : public ModuleComponents
{
public:
	ModuleComponentsMesh(ModuleGameObject* owner);
	~ModuleComponentsMesh();

	virtual bool	Update();
	virtual bool	CleanUp();

public:
	ModuleFBXLoader* GetMesh() const;
	void SetMesh(VertexData mesh);

	VertexData GetMesh();

	/*
	std::string GetMeshPath() const;
	void SetMeshPath(const char* path);

	void GetMeshData(uint& num_vertices, uint& num_normals, uint& num_tex_coords, uint& num_indices);
	*/
	//mesh needed here 
	VertexData mesh;

	const char* meshPath;

	const char* texturePath;
};

#endif 