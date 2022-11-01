#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"
#include "SDL/include/SDL.h"

class Application;

class ModuleWindow : public Module
{
public:

	ModuleWindow(bool start_enabled = true);

	// Destructor
	virtual ~ModuleWindow();

	bool Init();
	bool CleanUp();

	void SetTitle(const char* title);

	void SetFullscreen(bool fullscreen);

	void ModifyWidth(int x);
	void ModifyHeight(int y);

	float GetWidht();
	float GetHeight();
	void Vsync(bool vsync);
	void SetResizable(bool resizable);
	void SetFullDesktop(bool fullDesktop);
	void ModifyBrightness(float brightness);

	bool LoadConfig(JsonParsing& node) override;
	bool SaveConfig(JsonParsing& node) const override;

public:
	//The window we'll be rendering to
	SDL_Window* window;

	//The surface contained by the window
	SDL_Surface* screen_surface;

	bool vsync = false;
};

#endif // __ModuleWindow_H__