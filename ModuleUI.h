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
	bool PreUpdate(float dt);
	bool Update(float dt);
	bool PostUpdate(float dt);
	bool CleanUp();

	

public:
	void MainMenu();



	bool fullscreen;
	int screenWidth;
	int screenHeight;
};

#endif // __ModuleWindow_H__