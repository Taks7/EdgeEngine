#pragma once
#include "Module.h"
#include "Globals.h"
#include "ImGui/imgui.h"

class ModuleUI : public Module
{
public:
	ModuleUI(bool start_enabled = true);
	~ModuleUI();

	bool Start();
	update_status Update(float dt) override;
	update_status PostUpdate(float dt)override;
	bool CleanUp();
};