
#include "Application.h"
#include "Module.h"
#include "ModuleComponentMesh.h"

#include "ModuleGameObject.h"

#include "ModuleFBXLoader.h"

ModuleComponentsMesh::ModuleComponentsMesh(ModuleGameObject* owner) : ModuleComponents(owner,COMPONENT_TYPES::MESH,"Mesh")
{
	
}

ModuleComponentsMesh::~ModuleComponentsMesh()
{

}

bool ModuleComponentsMesh::Update()
{
	bool ret = true;
	return ret;
}

bool ModuleComponentsMesh::CleanUp()
{
	bool ret = true;

	return ret;
}

void ModuleComponentsMesh::SetMesh(VertexData mesh)
{
	this->mesh = mesh;
}

VertexData ModuleComponentsMesh::GetMesh()
{
	return mesh;
}
