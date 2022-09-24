#ifndef __ModuleUI_H__
#define __ModuleUI_H__

#include "Module.h"
#include "SDL/include/SDL.h"

class Application;

class ModuleUI : public Module
{
public:

	ModuleUI(bool start_enabled = true);

	// Destructor
	virtual ~ModuleUI();

	bool Init();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	

public:
	
};

#endif // __ModuleWindow_H__