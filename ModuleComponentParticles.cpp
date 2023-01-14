#include "ModuleComponentParticles.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "ModuleFBXLoader.h"
#include "ModuleInput.h"
#include "pcg-c-basic-0.9/pcg_basic.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_opengl3.h"
#include "ImGui/imgui_impl_sdl.h"
#include "ParticlesEmitter.h"


ModuleComponentParticles::ModuleComponentParticles(ModuleGameObject* owner) : ModuleComponents(owner, COMPONENT_TYPES::PARTICLES, "Particles")
{
	maxParticles = 200;

	particleReference = new Particles();
	particleReference->lifetime = 2.f;
	particleReference->direction = { 0,1,0 };
	particleReference->size = 1.f;
	particleReference->dirVariation = 180.0f;
	particleReference->speed = 2.f;
	particleReference->color = Blue;

	particle_material = nullptr;
	res_mesh = App->loaderModels->getBillboard();
}

ModuleComponentParticles::~ModuleComponentParticles()
{
}

bool ModuleComponentParticles::Init()
{
	for (size_t i = 0; i < emitters.size(); i++)
	{
		emitters[i].Init();
	}

	return true;
}

bool ModuleComponentParticles::Update()
{

	for (size_t i = 0; i < emitters.size(); i++)
	{
		emitters[i].UpdateModules();
	}

	for (size_t i = 0; i < emitters.size(); i++)
	{
		emitters[i].Draw();
	}

	return true;
}

void ModuleComponentParticles::Reset()
{
	for (size_t i = 0; i < emitters.size(); i++)
	{
		emitters[i].Reset();
	}
}

// 
//float ModuleComponentParticles::GetRandomFloat(range<float> number)
//{
//	return (ldexp(pcg32_random(), -32) * (number.max - number.min)) + number.min;
//}

//uint ModuleComponentParticles::GetRandomUint(range<uint> number)
//{
//	return (ldexp(pcg32_random(), -32) * (number.max - number.min)) + number.min;
//}


//TODO: Acabar de posar materials a cada particle, metode fbxloader?
void ModuleComponentParticles::AddMaterial(std::map<uint, Resource*> resources)
{
	uint flags = 0;
	flags |= ImGuiTreeNodeFlags_Leaf;

	for (std::map<uint, Resource*>::iterator goIterator = resources.begin(); goIterator != resources.end(); goIterator++)
	{
		Resource* res = (*goIterator).second;

		std::string name = res->GetName();

		if (App->input->GetFileType(res->GetAssetsPath()) == FileType::PNG)
		{
			if (ImGui::TreeNodeEx(name.c_str(), flags)) {

				ImGui::TreePop();

				if (ImGui::IsItemClicked())
				{
					ModuleComponentMaterial* particle_material;
					particle_material = (ModuleComponentMaterial*)this->GetOwner()->GetComponent(COMPONENT_TYPES::MATERIAL);
					particle_material->SetTexture(res->GetUID());
				}
			}
		}

		res = nullptr;
	}
}

void ModuleComponentParticles::UpdateParticleGUI(Particles* newParticleReference)
{
	particleReference = newParticleReference;
}

