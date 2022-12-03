#include "Project.h"
#include "ImGui/imgui.h"
#include "ModuleSceneIntro.h"
#include "ModuleComponentMaterial.h"
#include "ModuleComponentMesh.h"

Project::Project()
{
	active = true;
}

Project::~Project()
{
	//https://www.youtube.com/watch?v=9Gognvp8BvQ&ab_channel=OneShot mirar para repasar cosas de ImGuI
}

void Project::Draw()
{
	ImGui::Begin("Project");
	{
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
		if(ImGui::TreeNodeEx("Assets", flags))
		{
		
			if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
			{

			}		
			ImGui::TreePop();
		}
	}
	ImGui::End();
}