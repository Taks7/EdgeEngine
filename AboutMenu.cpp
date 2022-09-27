#include "AboutMenu.h"
#include "ImGui/imgui.h"
AboutMenu::AboutMenu()
{
	active = false;
}

AboutMenu::~AboutMenu()
{

}

void AboutMenu::Draw()
{
	ImGui::Begin("hello");
	{
		ImGui::Text("testing");
	}
	ImGui::End();

}