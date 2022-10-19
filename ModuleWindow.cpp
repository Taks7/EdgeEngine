#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"
ModuleWindow::ModuleWindow(bool start_enabled) : Module(start_enabled)
{
	name = "Window";
	window = NULL;
	screen_surface = NULL;
}

// Destructor
ModuleWindow::~ModuleWindow()
{
}

// Called before render is available
bool ModuleWindow::Init()
{
	LOG_COMMENT("Init SDL window & surface");
	bool ret = true;

	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		LOG_COMMENT("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		//Create window
		int width = SCREEN_WIDTH * SCREEN_SIZE;
		int height = SCREEN_HEIGHT * SCREEN_SIZE;
		Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN ;

		//Use OpenGL 2.1
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

		if(WIN_FULLSCREEN == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN;
		}

		if(WIN_RESIZABLE == true)
		{
			flags |= SDL_WINDOW_RESIZABLE;
		}

		if(WIN_BORDERLESS == true)
		{
			flags |= SDL_WINDOW_BORDERLESS;
		}

		if(WIN_FULLSCREEN_DESKTOP == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		}

		window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);

		if(window == NULL)
		{
			LOG_COMMENT("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			ret = false;
		}
		else
		{
			//Get window surface
			screen_surface = SDL_GetWindowSurface(window);
		}
	}

	return ret;
}

// Called before quitting
bool ModuleWindow::CleanUp()
{
	LOG_COMMENT("Destroying SDL window and quitting all SDL systems");

	//Destroy window
	if(window != NULL)
	{
		SDL_DestroyWindow(window);
	}

	//Quit SDL subsystems
	SDL_Quit();
	return true;
}

void ModuleWindow::SetTitle(const char* title)
{
	SDL_SetWindowTitle(window, title);
}
void ModuleWindow::SetFullscreen(bool fullscreen)
{
	SDL_SetWindowFullscreen(window, fullscreen);
}
void ModuleWindow::ModifyWidth(int x)
{
	SDL_SetWindowSize(window, x, screen_surface->h);
	App->renderer3D->OnResize(x, screen_surface->h);
	screen_surface->w = x;
}
void ModuleWindow::ModifyHeight(int y)
{
	SDL_SetWindowSize(window, screen_surface->w, y);
	App->renderer3D->OnResize(screen_surface->w, y);
	screen_surface->h = y;
}
void ModuleWindow::Vsync(bool vsync)
{
	vsync = SDL_HINT_RENDER_VSYNC;
}
void ModuleWindow::ModifyBrightness(float brightness)
{
	int result = SDL_SetWindowBrightness(window, brightness);

	if (result != 0)
	{
		LOG_COMMENT("Setting Brightness Value");
	}
}
bool ModuleWindow::LoadConfig(JsonParsing& node)
{
	App->ui->fullscreen = node.GetJsonBool("fullscreen");
	App->ui->Vsync = node.GetJsonBool("vsync");
	App->ui->screenBrightness = node.GetJsonNumber("brightness");

	App->window->SetFullscreen(App->ui->fullscreen);
	App->window->Vsync(App->ui->Vsync);
	App->window->ModifyBrightness(App->ui->screenBrightness);
	
	return true;
}

bool ModuleWindow::SaveConfig(JsonParsing& node) const
{
	node.SetNewJsonBool(node.ValueToObject(node.GetRootValue()), "fullscreen", App->ui->fullscreen);
	node.SetNewJsonBool(node.ValueToObject(node.GetRootValue()), "vsync", App->ui->Vsync);
	node.SetNewJsonNumber(node.ValueToObject(node.GetRootValue()), "brightness", App->ui->screenBrightness);
	
	return true;
}
void ModuleWindow::SetResizable(bool resizable)
{
	
}
void ModuleWindow::SetFullDesktop(bool fullDesktop)
{

}