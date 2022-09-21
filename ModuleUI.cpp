#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"
#include "ModuleUI.h"
#include "SDL\include\SDL_opengl.h"
#include "ImGui/imgui_impl_opengl2.h"
#include "ImGui/imgui_impl_sdl.h"


ModuleUI::ModuleUI(bool start_enabled) : Module(start_enabled)
{
}

// Destructor
ModuleUI::~ModuleUI()
{}

bool ModuleUI::Start()
{
	LOG("Loading start UI")

    // Setup Dear ImGui context
/*
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    // Setup Platform/Renderer bindings
    ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer3D->context);
    ImGui_ImplOpenGL3_Init("version #420");
    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
*/
	return true;
}
update_status ModuleUI::Update(float dt)
{
    /*
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();
    */
	return UPDATE_CONTINUE;
}
update_status ModuleUI::PostUpdate(float dt)
{
    //ImGui::Render();
   // ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    return UPDATE_CONTINUE;
}


bool ModuleUI::CleanUp()
{

	return true;
}
