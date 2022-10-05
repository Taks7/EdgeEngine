#ifndef __ModuleFBXLoader_H__
#define __ModuleFBXLoader_H__

#include "Module.h"
#include "SDL/include/SDL.h"

class Application;

class ModuleFBXLoader : public Module
{
public:

	ModuleFBXLoader(bool start_enabled = true);

	// Destructor
	virtual ~ModuleFBXLoader();

	bool Init();
	bool CleanUp();

	bool LoadConfig(JsonParsing& node) override;
	bool SaveConfig(JsonParsing& node) const override;

public:
	
};

#endif // __ModuleWindow_H__