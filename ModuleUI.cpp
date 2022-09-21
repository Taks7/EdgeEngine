#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"
#include "ModuleUI.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_glfw.h"
#include "ImGui/imgui_impl_sdl.h"
#include "Imgui/imgui_impl_opengl3.h"
#include <glew.h>
#include <glfw3.h>




ModuleUI::ModuleUI(bool start_enabled) : Module(start_enabled)
{
}

// Destructor
ModuleUI::~ModuleUI()
{}

bool ModuleUI::Start()
{
	LOG("Loading start UI")
	IMGUI_CHECKVERSION();

	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui::StyleColorsDark();
	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer3D->context);
	ImGui_ImplOpenGL3_Init();
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
