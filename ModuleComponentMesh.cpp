
#include "Application.h"
#include "Module.h"
#include "ModuleComponentMesh.h"

#include "ModuleGameObject.h"

#include "ModuleFBXLoader.h"

ModuleComponentsMesh::ModuleComponentsMesh(ModuleGameObject* owner) : ModuleComponents(owner,COMPONENT_TYPES::MESH,"Mesh")
{
	//App->loaderModels->LoadMeshToGameObject(owner, meshPath, texturePath);
}

ModuleComponentsMesh::~ModuleComponentsMesh()
{

}

bool ModuleComponentsMesh::Update()
{
	bool ret = true;

	//here we should draw the mesh I believe, 
	
	App->renderer3D->DrawGameObjects(mesh);
	
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