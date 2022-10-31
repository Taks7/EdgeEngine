#ifndef __ModuleConsole_H__
#define __ModuleConsole_H__

#include "Module.h"
#include <string>
#include <vector>
#include "ImGui/imgui.h"
#include "Globals.h"

class ModuleConsole : public Module
{
public:

	ModuleConsole(bool start_enabled = true);
	~ModuleConsole();

	bool DrawConsole(ImGuiIO& io);
	
	bool CleanUp();

public:

	void AddLog(const char* log);
	const char* GetName() const;

private:

	void ClearLog();
	void ConsoleMenu();
	void ConsoleOutput();
	void ConsoleScroll();

private:

	std::vector<char*> logs;
	bool scrollBottom;
};

#endif // __ModuleConsole_H__