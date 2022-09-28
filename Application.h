#pragma once
#include "Module.h"
#include <list>
#include <string>
#include "Globals.h"
#include "Timer.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleSceneIntro.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModuleUI.h"
#include "JsonParsing.h"

// USAR PARA STL: https://github.com/karansaxena/STL_Cheat_Sheets

class Application
{
public:
	ModuleWindow* window;
	ModuleInput* input;
	ModuleAudio* audio;
	ModuleSceneIntro* scene_intro;
	ModuleRenderer3D* renderer3D;
	ModuleCamera3D* camera;
	ModuleUI* ui;

	bool debug;
private:

	std::list<Module*> list_modules;
	Timer	ms_timer;
	float	dt;
	bool saveRequest;
	bool loadRequest;
	
public:

	Application();
	~Application();

	bool Init();
	bool Update();
	bool CleanUp();

	void RequestBrowser(const char* string);
private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();

	void SaveConfig();
	void LoadConfig();
};