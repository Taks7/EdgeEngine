#include "ModuleComponentBillboard.h"
#include "Resource.h"
#include "Application.h"
#include "ModuleGameObject.h"
#include "ModuleCamera3D.h"
#include "ModuleComponentParticles.h"
//#include "Libraries\glew\glew.h"
//#include "Libraries\SDL\include\SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>
ModuleComponentBillBoard::ModuleComponentBillBoard(ModuleGameObject* owner) : ModuleComponents(owner, COMPONENT_TYPES::BILLBOARD, "BillBoard")
{
	this->owner = owner;
	billboard_alignment = SCREEN_ALIGNED;
	//res_mesh = App->resources->GetBillboard();
	res_texture = nullptr;
	transform = (ModuleComponentsTransform*)owner->GetComponent(COMPONENT_TYPES::TRANSFORM);
}

ModuleComponentBillBoard::~ModuleComponentBillBoard()
{
}

bool ModuleComponentBillBoard::Update()
{
	return true;
}

void ModuleComponentBillBoard::Draw(Color color)
{
	FaceCamera();
	ModuleComponentParticles* ownerParticleSystem = (ModuleComponentParticles*)owner->GetComponent(COMPONENT_TYPES::PARTICLES);
	if (ownerParticleSystem != nullptr)
	{
		if (ownerParticleSystem->particle_material != nullptr)
		{
			glColor4f(color.r, color.g, color.b, color.a);

			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			if (ownerParticleSystem->particle_material->materialUsed != nullptr && ownerParticleSystem->particle_material->defaultTexture)
				glBindTexture(GL_TEXTURE_2D, ownerParticleSystem->particle_material->materialUsed->id);
			//else
				//glColor4f(mat->color.r, mat->color.g, mat->color.b, mat->color.a);
		}
	}

	transform->UpdateLocalTransform();

	glPushMatrix();
	glMultMatrixf(transform->GetLocalTransform().Transposed().ptr());

	//res_mesh->Draw();

	glPopMatrix();
}

void ModuleComponentBillBoard::FaceCamera()
{
	switch (billboard_alignment)
	{
	case Billboarding_Alignment::SCREEN_ALIGNED:
		ScreenAlign();
		break;
	case Billboarding_Alignment::WORLD_ALIGNED:
		WorldAlign();
		break;
	case Billboarding_Alignment::AXIS_ALIGNED:
		AxisAlign();
		break;
	default:
		break;
	}
}

void ModuleComponentBillBoard::SetAlignment(Billboarding_Alignment new_alignment)
{
	billboard_alignment = new_alignment;
}

std::string ModuleComponentBillBoard::GetAlignmentText()
{
	std::string ret_word = "";
	switch (billboard_alignment)
	{
	case Billboarding_Alignment::SCREEN_ALIGNED:
		ret_word = "Screen Aligned";
		return ret_word;
		break;
	case Billboarding_Alignment::WORLD_ALIGNED:
		ret_word = "World Aligned";
		return ret_word;
		break;
	case Billboarding_Alignment::AXIS_ALIGNED:
		ret_word = "Axis Aligned";
		return ret_word;
		break;
	default:
		ret_word = "None";
		return ret_word;
		break;
	}
}

void ModuleComponentBillBoard::ScreenAlign()
{
	float3 normal = (App->camera->GetPosition() - this->transform->GetPosition()).Normalized();
	float3 up = App->camera->GetCurrentCamera()->GetFrustum().Up();
	float3 right = normal.Cross(up);

	float3x3 mat = float3x3::identity;
	mat.Set(-right.x, -right.y, -right.z, up.x, up.y, up.z, normal.x, normal.y, normal.z);

	transform->SetRotation(mat.Inverted().ToEulerXYZ());
}

void ModuleComponentBillBoard::WorldAlign()
{
}

void ModuleComponentBillBoard::AxisAlign()
{
}