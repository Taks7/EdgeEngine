#ifndef __EditorPanel_H__
#define __EditorPanel_H__

#include <string>
#include <vector>

#include "ImGui/imgui.h"
#include "Globals.h"
#include "UI.h"
#include "ModuleUI.h"

class EditorPanel : public UI
{
public:
	EditorPanel();
	virtual ~EditorPanel();

	/*void Start();*/
	void Draw();
	virtual bool CleanUp();

public:
	void Enable();
	void Disable();

	const char*	GetName() const;
	bool IsActive() const;
	bool IsHovered() const;
	bool IsClicked() const;

	void SetIsHovered();
	void SetIsClicked(const bool& set_to);

	void HelpMarker(const char* description);

	bool BeginRootWindow(ImGuiIO& io, const char* window_id, bool docking, ImGuiWindowFlags window_flags);
	void BeginDockspace(ImGuiIO& io, const char* dockspace_id, ImGuiDockNodeFlags docking_flags, ImVec2 size);

	bool InitializeImGui() const;

private:
	const char*		name;
	bool			is_active;
	bool			is_hovered;
	bool			is_clicked;
	std::vector<EditorPanel*>	editor_panels;
};

#endif // !__EditorPanel_H__