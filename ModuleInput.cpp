#include "Globals.h"
#include "Application.h"
#include "ModuleUI.h"
#include "ModuleInput.h"
#include "ModuleFBXLoader.h"
#include "ImGui/imgui_impl_sdl.h"
#include "MathGeo/src/MathGeoLib.h"
#include "Assimp/include/assimp/cimport.h"

#define MAX_KEYS 300

ModuleInput::ModuleInput(bool start_enabled) : Module(start_enabled)
{
	name = "Input";
	keyboard = new KEY_STATE[MAX_KEYS];
	memset(keyboard, KEY_IDLE, sizeof(KEY_STATE) * MAX_KEYS);
}

// Destructor
ModuleInput::~ModuleInput()
{
	delete[] keyboard;
}

// Called before render is available
bool ModuleInput::Init()
{
	LOG_COMMENT("Init SDL input event system");
	bool ret = true;
	SDL_Init(0);

	if(SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		LOG_COMMENT("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	SDL_EventState(SDL_DROPFILE, SDL_ENABLE);
	return ret;
}

// Called every draw update
bool ModuleInput::PreUpdate(float dt)
{
	
	SDL_PumpEvents();
	const Uint8* keys = SDL_GetKeyboardState(NULL);
	//math::Clock::Day();
	for(int i = 0; i < MAX_KEYS; ++i)
	{
		if(keys[i] == 1)
		{
			if(keyboard[i] == KEY_IDLE)
				keyboard[i] = KEY_DOWN;
			else
				keyboard[i] = KEY_REPEAT;
		}
		else
		{
			if(keyboard[i] == KEY_REPEAT || keyboard[i] == KEY_DOWN)
				keyboard[i] = KEY_UP;
			else
				keyboard[i] = KEY_IDLE;
		}
	}

	Uint32 buttons = SDL_GetMouseState(&mouse_x, &mouse_y);

	mouse_x /= SCREEN_SIZE;
	mouse_y /= SCREEN_SIZE;
	mouse_z = 0;

	for(int i = 0; i < 5; ++i)
	{
		if(buttons & SDL_BUTTON(i))
		{
			if(mouse_buttons[i] == KEY_IDLE)
				mouse_buttons[i] = KEY_DOWN;
			else
				mouse_buttons[i] = KEY_REPEAT;
		}
		else
		{
			if(mouse_buttons[i] == KEY_REPEAT || mouse_buttons[i] == KEY_DOWN)
				mouse_buttons[i] = KEY_UP;
			else
				mouse_buttons[i] = KEY_IDLE;
		}
	}

	mouse_x_motion = mouse_y_motion = 0;

	bool quit = false;
	SDL_Event e;
	while(SDL_PollEvent(&e))
	{
		ImGui_ImplSDL2_ProcessEvent(&e);
		
		switch(e.type)
		{
			case SDL_MOUSEWHEEL:
			mouse_z = e.wheel.y;
			break;

			case SDL_MOUSEMOTION:
			mouse_x = e.motion.x / SCREEN_SIZE;
			mouse_y = e.motion.y / SCREEN_SIZE;

			mouse_x_motion = e.motion.xrel / SCREEN_SIZE;
			mouse_y_motion = e.motion.yrel / SCREEN_SIZE;
			break;

			case SDL_DROPFILE:
			{

				const char* dropped_filedir = e.drop.file;

				std::string path = App->fs->GetValidPath(dropped_filedir);

				if (App->fs->GetFileExtension(dropped_filedir) == "fbx" || App->fs->GetFileExtension(dropped_filedir) == "FBX")
				{
					App->loaderModels->LoadMeshToGameObject(App->scene_intro->CreateEmptyGameObject("Dropped"), path.c_str(), nullptr);
				}
				if (App->fs->GetFileExtension(dropped_filedir) == "png" || App->fs->GetFileExtension(dropped_filedir) == "PNG" || App->fs->GetFileExtension(dropped_filedir) == "dds" || App->fs->GetFileExtension(dropped_filedir) == "DDS")
				{
					//Add here function to change texture for the object
					for (int i = 0; i < App->scene_intro->game_objects.size(); i++)
					{
						if (App->scene_intro->game_objects[i]->IsSelected())
						{
							ModuleComponentMaterial* material = (ModuleComponentMaterial*)App->scene_intro->game_objects[i]->GetComponent(COMPONENT_TYPES::MATERIAL);
							
							if(App->scene_intro->game_objects[i]->childs.size() > 0)
							{
								for (int j = 0; j < App->scene_intro->game_objects[i]->childs.size(); j++)
								{
									ModuleComponentMaterial* materialChild = (ModuleComponentMaterial*)App->scene_intro->game_objects[i]->childs[j]->GetComponent(COMPONENT_TYPES::MATERIAL);
									Texture* newTexture = new Texture();
									App->materialImport->Import(path.c_str(), newTexture);
									if (materialChild->materialUsed != nullptr) materialChild->materialUsed = nullptr;
									materialChild->materialUsed = newTexture;
								}
							}
							Texture* newTexture = new Texture();
							App->materialImport->Import(path.c_str(), newTexture);
							if (material->materialUsed != nullptr) material->materialUsed = nullptr;
							material->materialUsed = newTexture;
						}
					}
				}

				SDL_free(&dropped_filedir);
			}
			break;

			case SDL_QUIT:
			quit = true;
			break;

			case SDL_WINDOWEVENT:
			{
				if(e.window.event == SDL_WINDOWEVENT_RESIZED)
					App->renderer3D->OnResize(e.window.data1, e.window.data2);
			}
			
			

		}
	}

	if (keyboard[SDL_SCANCODE_ESCAPE] == KEY_UP)
	{
		App->ui->exitPopUp = true;
	}

	if (keyboard[SDL_SCANCODE_1] == KEY_UP)
	{
		App->scene_intro->CreateCustomParticleSystem(3, { 0,0,0 }, App->scene_intro->CreateEmptyGameObject("FireworkParticle"));
	}
	
	return true;
}

FileType ModuleInput::GetFileType(std::string file)
{
	std::string dFile = file;
	if (dFile.length() > 4) {
		std::string formatStr = dFile.substr(dFile.length() - 4);
		if (formatStr == FBX_FORMAT || formatStr == FBX_FORMAT_CAP)
			return FileType::FBX;
		else if (formatStr == PNG_FORMAT || formatStr == PNG_FORMAT_CAP)
			return FileType::PNG;
		else if (formatStr == JPG_FORMAT || formatStr == JPG_FORMAT_CAP || formatStr == JPEG_FORMAT || formatStr == JPEG_FORMAT_CAP)
			return FileType::JPG;
		else if (formatStr == DDS_FORMAT || formatStr == DDS_FORMAT_CAP)
			return FileType::DDS;
		else if (formatStr == TGA_FORMAT || formatStr == TGA_FORMAT_CAP)
			return FileType::TGA;
	}
	else
		LOG_COMMENT("Cannot load %s file.  Format not recognized", file)
		return FileType::UNDEFINED;

}

bool ModuleInput::Update(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		App->debug = !App->debug;

	return true;
}

// Called before quitting
bool ModuleInput::CleanUp()
{
	LOG_COMMENT("Quitting SDL input event subsystem");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	return true;
}