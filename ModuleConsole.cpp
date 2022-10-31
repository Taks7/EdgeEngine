#include "Application.h"
#include "Globals.h"
#include "Application.h"
#include "ModuleConsole.h"
#include "ModuleUI.h"

//#pragma comment( lib, "SDL_mixer/libx86/SDL2_mixer.lib" )

ModuleConsole::ModuleConsole(bool start_enabled) : Module(start_enabled)
{
	name = "Console";
	scrollBottom = true;
}

ModuleConsole::~ModuleConsole()
{

}

bool ModuleConsole::DrawConsole(ImGuiIO& io)
{
	bool ret = true;

	ImGui::Begin(GetName(), NULL, ImGuiWindowFlags_MenuBar);

	ConsoleMenu();
	ConsoleOutput();
	ConsoleScroll();

	ImGui::End();

	return ret;
}

bool ModuleConsole::CleanUp()
{
	bool ret = true;

	ClearLog();

	return ret;
}

void ModuleConsole::AddLog(const char* log)
{
	char* tmp = strdup(log);											

	logs.push_back(tmp);

	scrollBottom = true;
}

void ModuleConsole::ClearLog()
{
	for (uint i = 0; i < logs.size(); ++i)										
	{
		free(logs[i]);
	}

	logs.clear();

	scrollBottom = true;
}

const char* ModuleConsole::GetName() const
{
	return name;
}

void ModuleConsole::ConsoleMenu()
{
	ImGui::BeginMenuBar();

	if (ImGui::BeginMenu("Options"))											
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

	for (int i = 0; i < logs.size(); ++i)
	{
		ImVec4 textColor = { 1.0f, 1.0f, 1.0f, 1.0f };

		if (strstr(logs[i], "[ERROR]") != nullptr)
		{
			textColor = { 1.0f, 1.0f, 0.0f, 1.0f };							
		}

		if (strstr(logs[i], "[WARNING]") != nullptr)
		{
			textColor = { 1.0f, 0.0f, 1.0f, 1.0f };							
		}

		if (strstr(logs[i], "[STATUS]") != nullptr)
		{
			textColor = { 1.0f, 0.0f, 1.0f, 1.0f };
		}

		if (strstr(logs[i], "[SCENE]") != nullptr)
		{
			textColor = { 0.0f, 1.0f, 1.0f, 1.0f };
		}

		ImGui::PushStyleColor(ImGuiCol_Text, textColor);
		ImGui::TextUnformatted(logs[i]);										
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