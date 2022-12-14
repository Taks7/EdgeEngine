
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "MathGeo/src/Math/TransformOps.h"
#include "Frustum.h"
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"
#include "ModuleInput.h"
#include "EditorPanel.h"
#include "ModuleGameObject.h"
#include "ModuleComponent.h"
#include "ModuleComponentTransform.h"
#include "ModuleComponentCamera.h"

#define IMGUI_DEFINE_MATH_OPERATORS
#include "ImGui/imgui_internal.h"

#include "SceneView.h"

SceneView::SceneView() : UI(),
tex_origin			({ 0.0f, 0.0f }),
cursor_pos			({ 0.0f, 0.0f }),
guizmo_operation	(ImGuizmo::OPERATION::TRANSLATE),
guizmo_mode			(ImGuizmo::MODE::WORLD)
{
	active = true;
}

SceneView::~SceneView()
{

}

void SceneView::Draw()
{
	bool ret = true;

	ImGui::Begin("Game");
	
	CheckSceneIsClicked();
	
	AdaptTextureToWindowSize();
	DrawSceneTexture();

	
	
	ImGui::End();

	ImGui::Begin("Scene");

	/*SetIsHovered();*/

	CheckSceneIsClicked();

	AdaptTextureToWindowSize();
	DrawSceneTexture();

	HandleGuizmos();

	ImGui::End();

	//--------------------

	//ImGui::Begin("CameraView");
	//{
	//	ImGui::SetNextWindowBgAlpha(0.1f);
	//	/*ImGui::SetWindowPos({ float(App->window->GetWidht() - 300),400 });
	//	ImGui::SetWindowSize({ 300,550 });*/


	//	/*if (selectedGameObject != nullptr && selectedGameObject != App->scene_intro->rootObject)
	//	{
	//		DrawInfoOfGameObject(selectedGameObject);
	//		DrawGameObjectComponents(selectedGameObject);
	//	}*/

	//	/*ImGui::Image((ImTextureID)App->renderer3D->scene_render_texture, ImVec2(1,1), ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));*/

	//	if (ImGui::Button("See Game Camera"))
	//	{
	//		/*App->camera->SetCurrentCamera(camera);*/
	//		/*App->ui->seeEditor = true;*/
	//		if (App->ui->seeEditor == false)
	//		{
	//			App->camera->SetCurrentCamera(App->camera->game_current_camera);
	//		}
	//	}

	//	ImGui::SameLine();

	//	if (ImGui::Button("See Editor View Camera"))
	//	{
	//		/*App->ui->seeEditor = false;*/
	//		App->camera->SetMasterCameraAsCurrentCamera();
	//	}
	//}
	//ImGui::End();
}

bool SceneView::CleanUp()
{
	bool ret = true;



	return ret;
}

// --- E_SCENE METHODS ---
float2 SceneView::GetWorldMousePosition()
{	
	float win_width		= (float)App->window->GetWidht();
	float win_height	= (float)App->window->GetHeight();
	
	float tex_width		= App->ui->tex_size.x;
	float tex_height	= App->ui->tex_size.y;

	float mouse_X		= (float)App->input->GetMouseX();
	float mouse_Y		= (float)App->input->GetMouseY();

	float2 screen_mouse_pos = float2(mouse_X, win_height - mouse_Y) - float2(tex_origin.x, tex_origin.y + 22.5f);
	//float2 screen_mouse_pos = GetScreenMousePosition();
	float2 norm_screen_pos	= float2(screen_mouse_pos.x / tex_width, screen_mouse_pos.y / tex_height);
	float2 world_mouse_pos	= float2(norm_screen_pos.x * win_width, norm_screen_pos.y * win_height);

	return world_mouse_pos;
}

float2 SceneView::GetScreenMousePosition()
{
	float win_width		= (float)App->window->GetWidht();
	float win_height	= (float)App->window->GetHeight();

	float tex_width		= App->ui->tex_size.x;
	float tex_height	= App->ui->tex_size.y;

	float mouse_X		= (float)App->input->GetMouseX();
	float mouse_Y		= (float)App->input->GetMouseY();

	float2 world_mouse_pos	= float2(mouse_X, mouse_Y);
	float2 norm_world_pos	= float2(world_mouse_pos.x / win_width, world_mouse_pos.y / win_height);
	float2 screen_mouse_pos = float2(norm_world_pos.x * tex_width, norm_world_pos.y * tex_height);

	screen_mouse_pos		+= float2(tex_origin.x, tex_origin.y);

	return screen_mouse_pos;
}

float2 SceneView::GetWorldMouseMotion()
{
	float2 win_mouse_motion	= float2((float)App->input->GetMouseXMotion(), (float)App->input->GetMouseYMotion());
	float2 win_size			= float2((float)App->window->GetWidht(), (float)App->window->GetHeight());
	float2 tex_size			= float2(App->ui->tex_size.x, App->ui->tex_size.y);

	float2 local_motion			= float2(win_mouse_motion.x / tex_size.x, win_mouse_motion.y / tex_size.y);
	float2 world_mouse_motion	= float2(local_motion.x * win_size.x, local_motion.y * win_size.y);

	return world_mouse_motion;
}

float2 SceneView::GetSceneTextureSize()
{
	return float2(App->ui->tex_size.x, App->ui->tex_size.y);
}

bool SceneView::UsingGuizmo()
{
	return ImGuizmo::IsUsing();
}

void SceneView::CheckSceneIsClicked()
{
	if (ImGui::IsWindowHovered())
	{
		ImGui::FocusWindow(ImGui::GetCurrentWindow());
	}
	else
	{
		if (ImGui::IsWindowFocused())
		{
			if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) 
				|| ImGui::IsMouseClicked(ImGuiMouseButton_Right) 
				|| ImGui::IsMouseClicked(ImGuiMouseButton_Middle))
			{
				ImGui::FocusWindow(nullptr);
			}
		}
	}

	/*if (ImGui::IsWindowFocused())
	{
		SetIsClicked(true);
	}
	else
	{
		SetIsClicked(false);
	}*/
}

void SceneView::AdaptTextureToWindowSize()
{	
	App->ui->tex_size = ImVec2((float)App->window->GetWidht(), (float)App->window->GetHeight());
	ImVec2 win_size = ImGui::GetWindowSize() * 0.975f;

	float width_ratio	= (App->ui->tex_size.x / win_size.x);
	float height_ratio	= (App->ui->tex_size.y / win_size.y);

	if (App->ui->tex_size.x > win_size.x)
	{
		App->ui->tex_size = App->ui->tex_size / width_ratio;			//des														
	}

	if (App->ui->tex_size.y > win_size.y)
	{
		App->ui->tex_size = App->ui->tex_size / height_ratio;				 //des													
	}
}

void SceneView::DrawSceneTexture()
{
	cursor_pos = (ImGui::GetWindowSize() - App->ui->tex_size) * 0.5f;  //des
	ImGui::SetCursorPos(cursor_pos);

	ImVec2 screen_cursor_pos = ImGui::GetCursorScreenPos();

	if (screen_cursor_pos.x > 1920)																			
	{
		screen_cursor_pos.x = screen_cursor_pos.x - 1920;
	}

	tex_origin		= screen_cursor_pos + ImVec2(0, App->ui->tex_size.y);		 //des									
	tex_origin.y	= (float)App->window->GetHeight() - tex_origin.y;										

	ImGui::Image((ImTextureID)App->renderer3D->GetSceneRenderTexture(), App->ui->tex_size, ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
	

	//ImGui::Image((ImTextureID)App->renderer->GetDepthBufferTexture(), tex_size, ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
}

void SceneView::HandleGuizmos()
{	
	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_STATE::KEY_DOWN)
	{
		guizmo_operation	= ImGuizmo::OPERATION::TRANSLATE;
		guizmo_mode			= ImGuizmo::MODE::WORLD;
	}
	if (App->input->GetKey(SDL_SCANCODE_E) == KEY_STATE::KEY_DOWN)
	{
		guizmo_operation	= ImGuizmo::OPERATION::ROTATE;
		guizmo_mode			= ImGuizmo::MODE::WORLD;
	}
	if (App->input->GetKey(SDL_SCANCODE_R) == KEY_STATE::KEY_DOWN)
	{
		guizmo_operation	= ImGuizmo::OPERATION::SCALE;
		guizmo_mode			= ImGuizmo::MODE::LOCAL;
	}
	
	/*if (App->editor->GetSelectedGameObjectThroughEditor() == nullptr)
	{
		return;
	}*/

	if (App->scene_intro->GetSelectedGameObject() == nullptr)
	{
		return;
	}

	if (App->camera->GetCurrentCamera() == nullptr)
	{
		return;
	}

	/*if (App->editor->GetCurrentCameraThroughEditor() == nullptr)
	{
		return;
	}*/
	
	ModuleGameObject* selected		= App->scene_intro->GetSelectedGameObject();
	ModuleComponentCamera* current_camera	= App->camera->GetCurrentCamera();

	float4x4 view_matrix		= current_camera->GetFrustum().ViewMatrix();
	float4x4 projection_matrix	= current_camera->GetFrustum().ProjectionMatrix();
	ModuleComponentsTransform* selectWorldTransform = (ModuleComponentsTransform*)selected->GetComponent(COMPONENT_TYPES::TRANSFORM);
	float4x4 world_transform = selectWorldTransform->GetWorldTransform();

	view_matrix.Transpose();																				// MathGeoLib works with Row-Major matrices and ImGuizmo works with
	projection_matrix.Transpose();																			// Column-Major matrices. Hence the need to transpose them.
	world_transform.Transpose();																			// ----------------------------------------------------------------

	ImGuizmo::SetDrawlist();

	float win_height	= (float)App->window->GetHeight();
	float tex_width		= App->ui->tex_size.y;
	ImVec2 origin_pos	= ImVec2(tex_origin.x, win_height - tex_origin.y - tex_width);

	ImGuizmo::SetRect(tex_origin.x, origin_pos.y, App->ui->tex_size.x, App->ui->tex_size.y);

	ImGuizmo::Manipulate(view_matrix.ptr(), projection_matrix.ptr(), guizmo_operation, guizmo_mode, world_transform.ptr());

	if (ImGuizmo::IsUsing())
	{
		world_transform = world_transform.Transposed();
		ModuleComponentsTransform* setWorldTransform = (ModuleComponentsTransform*)selected->GetComponent(COMPONENT_TYPES::TRANSFORM);
		setWorldTransform->SetWorldTransform(world_transform);
	}
}

//void SceneView::SetIsHovered()
//{
//	if (ImGui::IsWindowHovered())
//	{
//		is_hovered = true;
//	}
//	else
//	{
//		is_hovered = false;
//	}
//}
//
//void SceneView::SetIsClicked(const bool& set_to)
//{
//	is_clicked = set_to;
//}
