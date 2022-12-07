#include "Application.h"
#include "ModuleSceneIntro.h"
#include "ModuleWindow.h"
#include "EditorPanel.h"
#include "ModuleUI.h"
#include "UI.h"
#include "ImGui/imgui_impl_opengl3.h"
#include "ImGui/imgui_impl_sdl.h"

EditorPanel::EditorPanel()
{
	active = true;
}

EditorPanel::~EditorPanel()
{

}

void EditorPanel::Draw()
{
	bool ret = true;

	

	////Start Dear ImGui's frame
	//ImGui_ImplOpenGL3_NewFrame();
	//ImGui_ImplSDL2_NewFrame(App->window->GetWindow());
	//ImGui::NewFrame();

	//if (BeginRootWindow(io, "Root window", true, ImGuiWindowFlags_MenuBar))
	//{
	//	bool draw = true;
	//	for (uint i = 0; i < editor_panels.size(); ++i)
	//	{
	//		if (editor_panels[i]->IsActive())
	//		{
	//			draw = editor_panels[i]->Draw(io);

	//			if (!draw)
	//			{
	//				LOG_COMMENT("[EDITOR] Exited through %s Panel", editor_panels[i]->GetName());
	//				break;
	//			}
	//		}
	//	}

	//	ImGui::End();
	//}

	//---------------------------

	ImGui::Begin("CameraView");
	{
		ImGui::SetNextWindowBgAlpha(0.1f);
		/*ImGui::SetWindowPos({ float(App->window->GetWidht() - 300),400 });
		ImGui::SetWindowSize({ 300,550 });*/


		/*if (selectedGameObject != nullptr && selectedGameObject != App->scene_intro->rootObject)
		{
			DrawInfoOfGameObject(selectedGameObject);
			DrawGameObjectComponents(selectedGameObject);
		}*/

		/*ImGui::Image((ImTextureID)App->renderer3D->scene_render_texture, ImVec2(1,1), ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));*/

		if (ImGui::Button("See Game Camera"))
		{
			/*App->camera->SetCurrentCamera(camera);*/
			/*App->ui->seeEditor = true;*/
			if (App->ui->seeEditor == false)
			{
				App->camera->SetCurrentCamera(App->camera->game_current_camera);
			}
		}

		ImGui::SameLine();

		if (ImGui::Button("See Editor View Camera"))
		{
			/*App->ui->seeEditor = false;*/
			App->camera->SetMasterCameraAsCurrentCamera();
		}
	}
	ImGui::End();
}

bool EditorPanel::CleanUp()
{
	bool ret = true;

	return ret;
}

void EditorPanel::Enable()
{
	if (!is_active)
	{
		is_active = true;
	}
}

void EditorPanel::Disable()
{
	if (is_active)
	{
		is_active = false;
	}
}

bool EditorPanel::IsActive() const
{
	return is_active;
}

bool EditorPanel::IsHovered() const
{
	return is_hovered;
}

bool EditorPanel::IsClicked() const
{
	return is_clicked;
}

void EditorPanel::SetIsHovered()
{	
	if (ImGui::IsWindowHovered())												
	{
		is_hovered = true;
	}
	else
	{
		is_hovered = false;
	}
}

void EditorPanel::SetIsClicked(const bool& set_to)
{
	is_clicked = set_to;
}

const char* EditorPanel::GetName() const
{
	return name;
}

void EditorPanel::HelpMarker(const char* description)
{
	ImGui::TextDisabled("(?)");

	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();

		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(description);
		ImGui::PopTextWrapPos();

		ImGui::EndTooltip();
	}
}

bool EditorPanel::BeginRootWindow(ImGuiIO& io, const char* window_id, bool docking, ImGuiWindowFlags window_flags)
{
	bool ret = true;

	ImGuiViewport* viewport = ImGui::GetWindowViewport();

	ImGui::SetNextWindowPos(viewport->Pos);
	ImGui::SetNextWindowSize(viewport->Size);
	ImGui::SetNextWindowViewport(viewport->ID);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

	window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove
		| ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus
		| ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoBackground;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ret = ImGui::Begin(window_id, &ret, window_flags);
	ImGui::PopStyleVar(3);

	if (docking)
	{
		BeginDockspace(io, window_id, ImGuiDockNodeFlags_PassthruCentralNode, ImVec2(10.0f, 1.0f));
	}

	return ret;
}

void EditorPanel::BeginDockspace(ImGuiIO& io, const char* dockspace_id, ImGuiDockNodeFlags docking_flags, ImVec2 size)
{
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dckspace_id = ImGui::GetID(dockspace_id);
		ImGui::DockSpace(dckspace_id, size, docking_flags);
	}
}