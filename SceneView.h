#ifndef __SceneView_H__
#define __SceneView_H__

#include "MathGeo/src/Math/float2.h"
#include "ImGui/imgui.h"
#include "ImGuizmo/ImGuizmo.h"
#include "EditorPanel.h"
#include "Globals.h"
#include "UI.h"
#include "ModuleUI.h"
#include "ModuleGameObject.h"

class ModuleGameObject;

class SceneView : public UI
{
public:
	SceneView();
	virtual ~SceneView();

	void Draw();

	virtual bool CleanUp();

private:
	void CheckSceneIsClicked();

	void AdaptTextureToWindowSize();
	void DrawSceneTexture();

	void HandleGuizmos();

public:
	float2 GetWorldMousePosition();
	float2 GetScreenMousePosition();
	float2 GetWorldMouseMotion();
	float2 GetSceneTextureSize();

	bool UsingGuizmo();
	/*void SetIsHovered();
	void SetIsClicked(const bool& set_to);*/

private:
	ImVec2	tex_origin;
	ImVec2	cursor_pos;

	ImGuizmo::OPERATION	guizmo_operation;
	ImGuizmo::MODE guizmo_mode;
};

#endif // !__SceneView_H__