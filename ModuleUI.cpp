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
		if (ImGui::BeginMenu("Help"))
		{
			ImGui::SetNextWindowBgAlpha(1.0f);
			ImGui::Begin("About");
			{

				ImGui::SetWindowSize({ 600,700 });
				ImGui::Text("Edge Engine");
				ImGui::Text("Next world revolution engine!");
				ImGui::Text("By Albert Ramisa and Alejandro Martin");
				ImGui::NewLine();
				ImGui::Text("3rd parties libraries used:");
				//AQUI PONER TODAS LAS LIBRERIAS (hay que mirar)
				ImGui::BulletText("SDL");
				//
				ImGui::NewLine();
				ImGui::Text("MIT License");
				ImGui::NewLine();
				ImGui::Text("Copyright (c) 2012 - 2021 Krzysztof Gabis");
				ImGui::NewLine();
				ImGui::Text("Permission is hereby granted, free of charge, to any person obtaining a");
				ImGui::Text("copy of this software and associated documentation files(the 'Software'), to dealin");
				ImGui::Text("the Software without restriction, including without limitation the rights");
				ImGui::Text("to use, copy, modify, merge, publish, distribute, sublicense, and /or sell");
				ImGui::Text("copies of the Software, and to permit persons to whom the Software is ");
				ImGui::Text("furnished to do so, subject to the following conditions : ");
				ImGui::NewLine();
				ImGui::Text("The above copyright notice and this permission notice shall be included in");
				ImGui::Text("all copies or substantial portions of the Software.");
				ImGui::NewLine();
				ImGui::Text("THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND, EXPRESS OR");
				ImGui::Text("IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,");
				ImGui::Text("FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE");
				ImGui::Text("AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER");
				ImGui::Text("LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,");
				ImGui::Text("OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN");
				ImGui::Text("THE SOFTWARE.");
			}
			ImGui::End();

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