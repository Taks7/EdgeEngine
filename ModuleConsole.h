#ifndef __ModuleConsole_H__
#define __ModuleConsole_H__

#include <string>
#include <vector>
#include "Module.h"
#include "ImGui/imgui.h"
#include "Globals.h"
#include "UI.h"

class ModuleConsole : public UI
{
public:

	ModuleConsole();
	virtual ~ModuleConsole();

	void Draw();

public:

	void AddLog(const char* log);

private:

	void ClearLog();
	void ConsoleMenu();
	void ConsoleOutput();
	void ConsoleScroll();

private:

	std::vector<char*> logsConsole;
	bool scrollBottom;
};

#endif // __ModuleConsole_H__