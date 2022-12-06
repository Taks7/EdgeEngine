#ifndef _ModuleComponentMesh_H_
#define _ModuleComponentMesh_H_

#include "Module.h"
#include "ModuleGameObject.h"
#include "ModuleImporter.h"
#include <string>

//struct MeshVertexData;
class ModuleGameObject;

class ModuleComponentsMesh : public ModuleComponents
{
public:
	ModuleComponentsMesh(ModuleGameObject* owner);
	~ModuleComponentsMesh();

	virtual bool	Update();
	virtual bool	CleanUp();

public:
	//void SetMesh(MeshVertexData mesh);

	//MeshVertexData GetMesh();

	MeshVertexData mesh;

	const char* meshPath;

};

#endif 