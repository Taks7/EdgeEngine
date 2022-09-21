#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"
#include "ModuleUI.h"
#include "SDL\include\SDL_opengl.h"



ModuleUI::ModuleUI(bool start_enabled) : Module(start_enabled)
{
}

// Destructor
ModuleUI::~ModuleUI()
{}

bool ModuleUI::Start()
{
	LOG("Loading start UI")


	return true;
}
update_status ModuleUI::Update(float dt)
{
  
	return UPDATE_CONTINUE;
}
update_status ModuleUI::PostUpdate(float dt)
{
    
    return UPDATE_CONTINUE;
}


bool ModuleUI::CleanUp()
{

	return true;
}
