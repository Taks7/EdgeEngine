#include "Project.h"
#include "ImGui/imgui.h"
#include "ModuleSceneIntro.h"
#include "ModuleComponentMaterial.h"
#include "ModuleComponentMesh.h"

#define MAX_FILE_SIZE 500

Project::Project()
{
	selected_file = new char[MAX_FILE_SIZE];
	selected_file[0] = '\0';
	active = true;
}

Project::~Project()
{
	//https://www.youtube.com/watch?v=9Gognvp8BvQ&ab_channel=OneShot mirar para repasar cosas de ImGuI
}

void Project::Draw()
{
	ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);

	ImGui::Begin("Project");
	{
		if(ImGui::TreeNodeEx(ASSETS_PATH, ImGuiTreeNodeFlags_DefaultOpen))
		{
		
			DrawDirectoriesTree(ASSETS_PATH, META_EXTENSION);
			ImGui::TreePop();
		}
	}
	ImGui::PopStyleVar();
	ImGui::End();
}


void Project::DrawDirectoriesTree(const char* root_directory, const char* extension_to_filter)
{
	std::vector<std::string> directories;
	std::vector<std::string> files;

	std::string root_dir = root_directory;

	App->fs->DiscoverFiles(root_dir.c_str(), files, directories);

	for (uint i = 0; i < directories.size(); ++i)
	{
		std::string path = root_dir + directories[i] + "/";														// Ex: root_dir + directories[i] = "Assets/Models/"

		if (ImGui::TreeNodeEx(path.c_str(), 0, "%s/", directories[i].c_str()))
		{
			DrawDirectoriesTree(path.c_str(), extension_to_filter);
			path.clear();
			ImGui::TreePop();
		}
	}

	std::sort(files.begin(), files.end());

	for (uint i = 0; i < files.size(); ++i)
	{
		if (ImGui::TreeNodeEx(files[i].c_str(), ImGuiTreeNodeFlags_Leaf))
		{
			if (ImGui::IsItemClicked())
			{
				sprintf_s(selected_file, MAX_FILE_SIZE, "%s%s", root_dir.c_str(), files[i].c_str());

				if (ImGui::IsMouseDoubleClicked(0))
				{
					ready_to_load = true;
					HandleSelectedItem(selected_file);
					ImGui::TreePop();
					return;
				}
			}

			ImGui::TreePop();
		}
	}

	directories.clear();
	files.clear();
	root_dir.clear();
}

void Project::HandleSelectedItem(const char* file_path)
{
	std::string path = App->fs->GetValidPath(file_path);

	if (App->fs->GetFileExtension(file_path) == "fbx" || App->fs->GetFileExtension(file_path) == "FBX")
	{
		App->loaderModels->LoadMeshToGameObject(App->scene_intro->CreateEmptyGameObject("Dropped"), path.c_str(), nullptr);
	}
	if (App->fs->GetFileExtension(file_path) == "png" || App->fs->GetFileExtension(file_path) == "PNG" || App->fs->GetFileExtension(file_path) == "dds" || App->fs->GetFileExtension(file_path) == "DDS")
	{
		//Add here function to change texture for the object
		for (int i = 0; i < App->scene_intro->game_objects.size(); i++)
		{
			if (App->scene_intro->game_objects[i]->IsSelected())
			{
				ModuleComponentMaterial* material = (ModuleComponentMaterial*)App->scene_intro->game_objects[i]->GetComponent(COMPONENT_TYPES::MATERIAL);

				if (App->scene_intro->game_objects[i]->childs.size() > 0)
				{
					for (int j = 0; j < App->scene_intro->game_objects[i]->childs.size(); j++)
					{
						ModuleComponentMaterial* materialChild = (ModuleComponentMaterial*)App->scene_intro->game_objects[i]->childs[j]->GetComponent(COMPONENT_TYPES::MATERIAL);
						Texture* newTexture = new Texture();
						App->materialImport->Import(path.c_str(), newTexture);
						if (materialChild->materialUsed != nullptr) materialChild->materialUsed = nullptr;
						materialChild->materialUsed = newTexture;
					}
				}
				Texture* newTexture = new Texture();
				App->materialImport->Import(path.c_str(), newTexture);
				if (material->materialUsed != nullptr) material->materialUsed = nullptr;
				
				
				//material->materialUsed->Import(path.c_str(), material->materialUsed);
			}
		}
	}
}