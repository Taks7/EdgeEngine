
#include "Globals.h"
#include "Module.h"
#include "ModuleComponent.h"
#include "ModuleGameObject.h"
#include "ModuleComponentMaterial.h"
#include "ModuleFBXLoader.h"

ModuleComponentMaterial::ModuleComponentMaterial(ModuleGameObject* owner) : ModuleComponents(owner, COMPONENT_TYPES::MATERIAL, "Material"),
materialUsed(nullptr),
defaultTexture(false)
{

}

ModuleComponentMaterial::~ModuleComponentMaterial()
{

}

bool ModuleComponentMaterial::Update()
{
	bool ret = true;

	return ret;
}

bool ModuleComponentMaterial::CleanUp()
{
	bool ret = true;

	if (materialUsed != nullptr)
	{
		materialUsed->CleanUp();
	}

	for (uint i = 0; i < textures.size(); ++i)
	{
		textures[i]->CleanUp();

		delete textures[i];
		textures[i] = nullptr;
	}

	textures.clear();

	return ret;
}