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
	const char*     GetName() const;
	void			SetName(const char* new_name);

	bool			IsActive() const;
	void			SetIsActive(const bool& is_active);

public:
	std::vector<ModuleFBXLoader*> textures;

	bool defaultTexture;

private:
	ModuleFBXLoader* materialUsed;
};

#endif // !__ComponentMesh_H__