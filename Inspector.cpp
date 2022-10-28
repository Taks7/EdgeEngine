#include "Inspector.h"
#include "ImGui/imgui.h"
InspectorMenu::InspectorMenu()
{
	active = false;
}

InspectorMenu::~InspectorMenu()
{
	//https://www.youtube.com/watch?v=9Gognvp8BvQ&ab_channel=OneShot mirar para repasar cosas de ImGuI
}

void InspectorMenu::Draw()
{
	ImGui::Begin("Inspector");
	{
		
	}
	ImGui::End();

}