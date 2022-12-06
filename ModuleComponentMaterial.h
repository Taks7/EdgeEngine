#ifndef __ModuleComponentMaterial_H__
#define __ModuleComponentMaterial_H__

#include "Module.h"
#include "ModuleGameObject.h"
#include <string>
#include "ModuleImporter.h"
class ModuleGameObject;
class ModuleMaterial;

struct TextureData;

class ModuleComponentMaterial : public ModuleComponents
{
public:
	ModuleComponentMaterial(ModuleGameObject* owner);
	~ModuleComponentMaterial();

	bool	Update();
	bool	CleanUp();

	TextureData* GetTexture();

	void SetTexture(TextureData* texture);


public:
	std::vector<TextureData*> textures;

	bool defaultTexture;

	TextureData* materialUsed;
	
};

#endif // !__ComponentMesh_H__