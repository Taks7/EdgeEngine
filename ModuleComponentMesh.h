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

	VertexData mesh;

	const char* meshPath;

};

#endif 