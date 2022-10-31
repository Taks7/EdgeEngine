#include "Application.h"
#include "Globals.h"
#include "Application.h"
#include "ModuleConsole.h"
#include "ModuleUI.h"

//#pragma comment( lib, "SDL_mixer/libx86/SDL2_mixer.lib" )

ModuleConsole::ModuleConsole()
{
	active = true;
	scrollBottom = true;
}

ModuleConsole::~ModuleConsole()
{

}

void ModuleConsole::Draw()
{
	ImGui::Begin("Console", NULL, ImGuiWindowFlags_MenuBar);
	{
		ImGui::SetWindowPos({ float(App->window->screen_surface->h -800),660 });
		ImGui::SetWindowSize({ 800,140 });
		ConsoleMenu();
		ConsoleOutput();
		/*ConsoleScroll();*/	
	}
	ImGui::End();
}

void ModuleConsole::AddLog(const char* log)
{
	char* tmp = strdup(log);											

	logsConsole.push_back(tmp);

	scrollBottom = true;
}

void ModuleConsole::ClearLog()
{
	for (uint i = 0; i < logsConsole.size(); ++i)
	{
		free(logsConsole[i]);
	}

	logsConsole.clear();

	scrollBottom = true;
}

void ModuleConsole::ConsoleMenu()
{
	ImGui::BeginMenuBar();

	if (ImGui::BeginMenu("Console Options"))											
	{
		if (ImGui::MenuItem("Clear Console"))									
		{
			ClearLog();
		}

		ImGui::EndMenu();														
	}

	ImGui::EndMenuBar();														
}

void ModuleConsole::ConsoleOutput()
{
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 6));

	for (int i = 0; i < logsConsole.size(); ++i)
	{
		ImVec4 textColor = { 1.0f, 1.0f, 1.0f, 1.0f };

		if (strstr(logsConsole[i], "[ERROR]") != nullptr)
		{
			textColor = { 1.0f, 0.0f, 0.0f, 1.0f };
		}

		if (strstr(logsConsole[i], "[WARNING]") != nullptr)
		{
			textColor = { 1.0f, 1.0f, 0.0f, 1.0f };
		}

		if (strstr(logsConsole[i], "[STATUS]") != nullptr)
		{
			textColor = { 1.0f, 0.0f, 1.0f, 1.0f };
		}

		if (strstr(logsConsole[i], "[SCENE]") != nullptr)
		{
			textColor = { 0.0f, 1.0f, 1.0f, 1.0f };
		}

		ImGui::PushStyleColor(ImGuiCol_Text, textColor);
		ImGui::TextUnformatted(logsConsole[i]);
		ImGui::PopStyleColor();
	}

	ImGui::PopStyleVar();
}

void ModuleConsole::ConsoleScroll()
{
	if (scrollBottom)
	{
		ImGui::SetScrollHereY(1.0f);											
		scrollBottom = false;
	}
}