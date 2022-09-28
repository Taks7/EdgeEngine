#include "Globals.h"
#include "Application.h"
#include "ModuleUI.h"
#include "glew/glew-2.2.0/include/GL/glew.h"
#include "ModuleRenderer3D.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_sdl.h"
#include "ImGui/imgui_impl_opengl2.h"
#include "ModuleInput.h"
#include "ModuleWindow.h"
// Just a test to add a basic cube onto the map and test the wireframe option
#include "Primitive.h"
//
#include "UI.h"
#include "AboutMenu.h"

#pragma comment (lib, "glew/glew-2.2.0/lib/Release/Win32/glew32.lib")

ModuleUI::ModuleUI(bool start_enabled) : Module(start_enabled)
{
	
}

// Destructor
ModuleUI::~ModuleUI()
{
}

// Called before render is available
bool ModuleUI::Init()
{
	//IMGUI start
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	//io.ConfigFlags |= ImGuiConfigFlags_
	//io.BackendFlags 
	ImGui::StyleColorsDark();


	ImGuiStyle& style = ImGui::GetStyle();

	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer3D->context);
	ImGui_ImplOpenGL2_Init();

	//Way to add menus to the menu list
	menus.push_back(aboutMenu = new AboutMenu());

	screenBrightness = 1.0f;
	screenHeight = App->window->screen_surface->h;
	screenWidth = App->window->screen_surface->w;
	return true;
}

bool ModuleUI::PreUpdate(float dt)
{
	
	
	return true;
}

bool ModuleUI::Update(float dt)
{
	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	MainMenu();
	for (std::vector<UI*>::iterator it = menus.begin(); it != menus.end(); ++it)
	{
		if ((*it)->IsActive())
		{
			(*it)->Draw();
		}
	}
	ImGui::Render();
	ImGui::EndFrame();
	ImGui::UpdatePlatformWindows();

	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

	
	return true;
}

bool ModuleUI::PostUpdate(float dt)
{
	

	return true;
}

// Called before quitting
bool ModuleUI::CleanUp()
{

	for (int i = 0; i < menus.size(); i++)
	{
		menus[i]->CleanUp();
		delete menus[i];
	}
	menus.clear();

	ImGui_ImplOpenGL2_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	return true;
}

void ModuleUI::MainMenu()
{
	//MENUS 
	ImGui::BeginMainMenuBar();
	{
		if (ImGui::BeginMenu("GeneralStuff"))
		{
			if (ImGui::Checkbox("AboutMenu", &enableAboutMenu))
			{
				aboutMenu->active = enableAboutMenu;
			}
			ImGui::EndMenu();

		}
		if (ImGui::BeginMenu("Configuration"))
		{
			
			ImGui::EndMenu();

		}
		if (ImGui::BeginMenu("Application"))
		{
			ImGui::EndMenu();

		}
		if (ImGui::BeginMenu("Window"))
		{
			
			if(ImGui::Checkbox("Fullscreen",&fullscreen))
			{
				App->window->SetFullscreen(fullscreen);
			}
			if (ImGui::Checkbox("Vsync", &Vsync))
			{
				App->window->Vsync(Vsync);
			}
			if (ImGui::SliderInt("Width", &screenWidth, 640, 1920))
			{
				App->window->ModifyWidth(screenWidth);
				
			}
			if (ImGui::SliderInt("Height", &screenHeight, 480, 1080))
			{
				App->window->ModifyHeight(screenHeight);
			}
			if (ImGui::SliderFloat("Brightness", &screenBrightness, 0.0001f, 1.0001f))
			{
				App->window->ModifyBrightness(screenBrightness);
			}

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("File System"))
		{
			ImGui::EndMenu();

		}
		if (ImGui::BeginMenu("Input"))
		{
			ImGui::EndMenu();

		}
		if (ImGui::BeginMenu("Hardware"))
		{
			ImGui::EndMenu();

		}
		if (ImGui::BeginMenu("Render options"))
		{
			if (ImGui::Checkbox("Wireframe Mode", &App->renderer3D->atributes.Wireframe))
			{

			}
			if (ImGui::Checkbox("GL_Depth_test", &App->renderer3D->atributes.Depth_test))
			{
		
			}
			if (ImGui::Checkbox("GL_Cull_face", &App->renderer3D->atributes.Cull_Face))
			{
				

			}
			if (ImGui::Checkbox("GL_Lightning", &App->renderer3D->atributes.Lightning))
			{
				

			}
			if (ImGui::Checkbox("GL_Color_material", &App->renderer3D->atributes.Color_Materials))
			{

			}
			if (ImGui::Checkbox("GL_Front", &App->renderer3D->atributes.Front))
			{

			}
			if (ImGui::Checkbox("GL_AmbientOclussion", &App->renderer3D->atributes.AmbientOclussion))
			{

			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Primitives_test"))
		{
			if (ImGui::Checkbox("CreateTestCube", &testCube))
			{
			}
			ImGui::EndMenu();

		}
	}
	ImGui::EndMainMenuBar();
}