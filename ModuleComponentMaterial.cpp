
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

	
	
	textures.clear();

	return ret;
}

Texture* ModuleComponentMaterial::GetTexture()
{
	return materialUsed;
}

void ModuleComponentMaterial::SetTexture(Texture* texture)
{
	materialUsed = texture;
}