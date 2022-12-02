#include "Project.h"
#include "ImGui/imgui.h"
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

	}
	ImGui::End();
}