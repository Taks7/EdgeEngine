#ifndef __ModuleComponentMaterial_H__
#define __ModuleComponentMaterial_H__

#include "Module.h"
#include "ModuleGameObject.h"
#include <string>

class ModuleGameObject;
class ModuleMaterial;
class ModuleFBXLoader;

struct Texture;

class ModuleComponentMaterial : public ModuleComponents
{
public:
	ModuleComponentMaterial(ModuleGameObject* owner);
	~ModuleComponentMaterial();

	bool	Update();
	bool	CleanUp();


public:
	std::vector<ModuleFBXLoader*> textures;

	bool defaultTexture;

private:
	ModuleFBXLoader* materialUsed;
};

#endif // !__ComponentMesh_H__