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
#include "Primitive.h"
#include "UI.h"
#include "AboutMenu.h"
#include "Hierarchy.h"
#include "Inspector.h"
#include "ModuleConsole.h"

#pragma comment (lib, "glew/glew-2.2.0/libGlew/Release/Win32/glew32.lib")

ModuleUI::ModuleUI(bool start_enabled) : Module(start_enabled)
{
	name = "UI";

	menus.push_back(aboutMenu = new AboutMenu());
	menus.push_back(hierarchy = new Hierarchy());
	menus.push_back(inspector = new InspectorMenu());
	menus.push_back(consoleMenu = new ModuleConsole());
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
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	ImGui::StyleColorsDark();
	


	ImGuiStyle& style = ImGui::GetStyle();

	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer3D->context);
	ImGui_ImplOpenGL2_Init();

	//Way to add menus to the menu list
	

	screenBrightness = 1.0f;
	screenHeight = App->window->screen_surface->h;
	screenWidth = App->window->screen_surface->w;

	GPUSeller = (const char*)glGetString(GL_VENDOR);
	GPUSpecs = (const char*)glGetString(GL_RENDERER);
	GPUDrivers = (const char*)glGetString(GL_VERSION);

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
			if(ImGui::MenuItem("About..."))
			{
				aboutMenu->switchActive();
			}
			if (ImGui::MenuItem("Documentation"))
			{
				App->RequestBrowser("https://github.com/Taks7/EdgeEngine/wiki");
			}
			if (ImGui::MenuItem("Download latest"))
			{
				App->RequestBrowser("https://github.com/Taks7/EdgeEngine/releases");
			}
			if (ImGui::MenuItem("Report a bug"))
			{
				App->RequestBrowser("https://github.com/Taks7/EdgeEngine/issues");
			}
			if (ImGui::MenuItem("Exit engine"))
			{
				exit(0);
			}

			ImGui::EndMenu();

		}
		if (ImGui::BeginMenu("Configuration"))
		{
			if (ImGui::MenuItem("Save Configuration"))
			{
				App->SaveConfigRequest();
			}
			if (ImGui::MenuItem("Load Configuration"))
			{
				App->LoadConfigRequest();
			}
			ImGui::EndMenu();

		}
		if (ImGui::BeginMenu("Window"))
		{
			if(ImGui::Checkbox("Fullscreen",&fullscreen))
			{
				App->window->SetFullscreen(fullscreen);
			}
			ImGui::SameLine();
			if (ImGui::Checkbox("Vsync", &Vsync))
			{
				App->window->Vsync(Vsync);
			}
			if (ImGui::Checkbox("Resizable", &resizable))
			{
				//not done yet
			}
			ImGui::SameLine();
			if (ImGui::Checkbox("Full Desktop", &FullDesktop))
			{
				//not done yet
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
			if (ImGui::MenuItem("ImGui Settings"))
			{
				showGUIPreferences = !showGUIPreferences;
			}

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Hardware"))
		{
			if (frames.size() >= 100) 
			{
				for (size_t i = 1; i < frames.size(); i++)
				{
					frames[i - 1] = frames[i];
				}
				frames[frames.size() - 1] = float(ImGui::GetIO().Framerate);
			}
			else
			{
				frames.push_back(float(ImGui::GetIO().Framerate));
			}
			ImGui::Text("Frame rate = %f:", ImGui::GetIO().Framerate);
			ImGui::PlotHistogram("Framerate", &frames[0], frames.size(), 0, NULL, 0.0f, 100.0f, ImVec2(300, 100));

			if (miliseconds.size() >= 100)
			{
				for (size_t i = 1; i < miliseconds.size(); i++)
				{
					miliseconds[i - 1] = miliseconds[i];
				}
				miliseconds[miliseconds.size() - 1] = float(ImGui::GetIO().DeltaTime);
			}
			else
			{
				miliseconds.push_back(float(ImGui::GetIO().DeltaTime));
			}
			ImGui::Text("miliseconds = %f:", ImGui::GetIO().DeltaTime);
			ImGui::PlotHistogram("miliseconds", &miliseconds[0], miliseconds.size(), 0, NULL, 0.0f, 0.5f, ImVec2(300, 100));
			ImGui::NewLine();
			ImGui::Text("Hardware information:");
			ImGui::NewLine();
			int count, size;
			App->GetCPU(count, size);
			ImGui::Text("CPUs: %d (%dKb)", count, size);
			ImGui::Separator();
			float ram = App->GetRAM();
			ImGui::Text("RAM: %.2fGb", ram);
			ImGui::Separator();
			ImGui::Text("GPU Seller: %s\nGPU Specifications: %s\nGPU Drivers: %s", GPUSeller.c_str(), GPUSpecs.c_str(), GPUDrivers.c_str());
			ImGui::Separator();
			int major, minor, patch;
		    App->GetSDLVersion(major, minor, patch);
		    ImGui::Text("SDL Version: %d.%d.%d", major, minor, patch);
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
		if (ImGui::BeginMenu("GameObjects"))
		{
			if (ImGui::MenuItem("CreateEmpty"))
			{
				App->scene_intro->CreateEmptyGameObject("empty", nullptr);
				LOG_COMMENT("[SCENE] Created an Empty Game Object")
			}
			if (ImGui::MenuItem("Create Baker House With Mesh"))
			{
				App->loaderModels->LoadMeshToGameObject(App->scene_intro->CreateEmptyGameObject("house", nullptr), "Assets/BakerHouse.fbx", "Assets/Resources/Baker_House.png");
				LOG_COMMENT("[SCENE] Created a Game Object with Mesh and Textures")
			}
			if (ImGui::MenuItem("Create Weapon 1911"))
			{
				App->loaderModels->LoadMeshToGameObject(App->scene_intro->CreateEmptyGameObject("1911", nullptr), "Assets/1911.fbx", nullptr);
				LOG_COMMENT("[SCENE] Created a Weapon 1911 with Mesh")
			}
			if (ImGui::MenuItem("Create Weapon AK47"))
			{
				App->loaderModels->LoadMeshToGameObject(App->scene_intro->CreateEmptyGameObject("AK47", nullptr), "Assets/AK47.fbx", nullptr);
				LOG_COMMENT("[SCENE] Created a Weapon AK47 with Mesh")
			}
			if (ImGui::MenuItem("Create Weapon MP7"))
			{
				App->loaderModels->LoadMeshToGameObject(App->scene_intro->CreateEmptyGameObject("MP7", nullptr), "Assets/MP7.fbx", nullptr);
				LOG_COMMENT("[SCENE] Created a Weapon MP7 with Mesh")
			}
			ImGui::EndMenu();

		}

		if (ImGui::BeginMenu("Basic Shapes"))
		{
			if (ImGui::MenuItem("Load Cube"))
			{
				App->loaderModels->LoadMeshToGameObject(App->scene_intro->CreateEmptyGameObject("BSCube", nullptr), "Assets/BSCube.fbx", nullptr);
				LOG_COMMENT("[SCENE] Created a Cube Basic Shape")
			}
			if (ImGui::MenuItem("Load Triangle"))
			{
				App->loaderModels->LoadMeshToGameObject(App->scene_intro->CreateEmptyGameObject("BSTriangle", nullptr), "Assets/BSTriangle.fbx", nullptr);
				LOG_COMMENT("[SCENE] Created a Triangle Basic Shape")
			}

			if (ImGui::MenuItem("Load Sphere"))
			{
				App->loaderModels->LoadMeshToGameObject(App->scene_intro->CreateEmptyGameObject("BSSphere", nullptr), "Assets/BSSphere.fbx", nullptr);
				LOG_COMMENT("[SCENE] Created a Sphere Basic Shape")
			}

			ImGui::EndMenu();

		}
	}

	if (showGUIPreferences)
	{
		ImGui::Begin("ImGui Settings", &showGUIPreferences);
		ImGui::ShowStyleEditor();
		ImGui::End();
	}

	ImGui::EndMainMenuBar();
}

void ModuleUI::ConsoleLogs(const char* log)
{	
	if (consoleMenu != nullptr)
	{
		consoleMenu->AddLog(log);
	}
	
}