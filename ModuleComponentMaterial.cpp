
#include "Globals.h"
#include "Module.h"
#include "ModuleComponent.h"
#include "ModuleGameObject.h"
#include "ModuleComponentMaterial.h"
#include "ModuleImporter.h"

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

TextureData* ModuleComponentMaterial::GetTexture()
{
	return materialUsed;
}

void ModuleComponentMaterial::SetTexture(TextureData* texture)
{
	materialUsed = texture;
}