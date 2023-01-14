#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "ModuleUI.h"
#include "AABB.h"
#include "ParticlesEmitter.h"
ModuleSceneIntro::ModuleSceneIntro(bool start_enabled) : Module(start_enabled)
{
	name = "scene";

}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG_COMMENT("Loading Intro assets");
	bool ret = true;

	App->camera->LookAtFloat3(float3::zero);
	
	CreateSceneCamera();

	
	/*App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));*/

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG_COMMENT("Unloading Intro scene");

	for (int n = 0; n < primitives.size(); n++)
	{
		delete primitives[n];
	}
	primitives.clear();

	return true;
}

// Update
bool ModuleSceneIntro::Update(float dt)
{
	NormalPlane p(0, 1, 0, 0);
	p.axis = true;
	p.Render();


	if (App->ui->testCube)
	{
		Cube c(0.5, 0.5, 0.5);		
		//SpherePrimitive s(1, 12, 24);

		//CylinderPrimitive k(10, 10);
	}

	if (App->ui->testCylinder)
	{
		CylinderPrimitive k(20, 10);
	}

	if (App->ui->testSphere)
	{
		SpherePrimitive s(12, 12, 24);
	}
	
	//TODO 3: Nothing to do here. But it's good to know where all primitives are being updated
	for (uint n = 0; n < primitives.size(); n++)
	{
		primitives[n]->Update();
	}

	for (uint j = 0; j < game_objects.size(); j++)
	{
		game_objects.at(j)->Render();

		ModuleComponentCamera* cameraobject = (ModuleComponentCamera*)game_objects.at(j)->GetComponent(COMPONENT_TYPES::CAMERA);
		if (cameraobject != nullptr)
		{
			if (cameraobject->IsCulling())
			{
				culling_camera = cameraobject;
			}
		}
	}
	
	return true;
}

bool ModuleSceneIntro::PostUpdate()
{
	//TODO 3: Nothing to do here. But it's good to know where all primitives are being rendered
	for (uint n = 0; n < primitives.size(); n++)
	{
		primitives[n]->Render();
	}

	for (uint i = 0; i < game_objects.size(); i++)
	{
		if (game_objects[i]->IsActive())
		{
			game_objects[i]->Update();
			
		}
	}
	
	return true;
}

ModuleGameObject* ModuleSceneIntro::CreateEmptyGameObject(const char* name, ModuleGameObject* parent)
{
	if (game_objects.empty())
	{
		CreateMasterGameObject();

		return rootObject;
	}
	else if (!game_objects.empty())
	{
		if (parent == nullptr)
		{
			std::string gameObjName = name;

			if (!game_objects.empty())
			{
				gameObjName += std::to_string(game_objects.size());
			}
			ModuleGameObject* gameObject = new ModuleGameObject(game_objects.size(), gameObjName, true, false);
			gameObject->CreateComponent(COMPONENT_TYPES::MATERIAL);
			gameObject->CreateComponent(COMPONENT_TYPES::MESH);
			gameObject->CreateComponent(COMPONENT_TYPES::TRANSFORM);
			//gameObject->CreateComponent(COMPONENT_TYPES::CAMERA);
			game_objects.push_back(gameObject);
			rootObject->childs.push_back(gameObject);
			return gameObject;
		}


		if (parent != nullptr)
		{
			ModuleGameObject* gameObject_child = new ModuleGameObject(parent->childs.size(), name, true, false);
			gameObject_child->CreateComponent(COMPONENT_TYPES::MATERIAL);
			gameObject_child->CreateComponent(COMPONENT_TYPES::MESH);
			gameObject_child->CreateComponent(COMPONENT_TYPES::TRANSFORM);
			//gameObject_child->CreateComponent(COMPONENT_TYPES::CAMERA);
			parent->childs.push_back(gameObject_child);
			return gameObject_child;

		}
	}
	
	
}
void ModuleSceneIntro::getRaycastHits(const LineSegment& ray, std::map<float, ModuleGameObject*>& hits)
{
	for (uint i = 0; i < game_objects.size(); ++i)
	{
		for (int j = 0; j < game_objects.at(i)->childs.size(); j++)
		{
			if (ray.Intersects(game_objects.at(i)->childs.at(j)->GetAABB()))
			{
				ModuleComponentsTransform* gameObjTransform = (ModuleComponentsTransform*)game_objects.at(i)->childs.at(j)->GetComponent(COMPONENT_TYPES::TRANSFORM);
				float3 position = gameObjTransform->GetGlobalPosition();
				hits.emplace(ray.Distance(position), game_objects.at(i)->childs.at(j));
			}
		}
		if (ray.Intersects(game_objects[i]->GetAABB()))
		{
			ModuleComponentsTransform* gameObjTransform = (ModuleComponentsTransform*)game_objects[i]->GetComponent(COMPONENT_TYPES::TRANSFORM);
			float3 position = gameObjTransform->GetGlobalPosition();
			hits.emplace(ray.Distance(position), game_objects[i]);
		}
	}

	
}

//TODO 9: And change the color of the colliding bodies, so we can visualize it working!
bool ModuleSceneIntro::RaycastSelection(const LineSegment& ray)
{
	LOG_COMMENT("[SCENE] RayCast selection");
	std::map<float, ModuleGameObject*> hits;
	getRaycastHits(ray, hits);

	if (hits.size() == 0)
	{
		hits.clear();
		return true;
	}

	std::map<float, ModuleGameObject*>::iterator item;
	for (item = hits.begin(); item != hits.end(); ++item)
	{
		
		    LineSegment local_ray = ray;
			ModuleComponentsTransform* transform = (ModuleComponentsTransform*)item->second->GetComponent(COMPONENT_TYPES::TRANSFORM);
			local_ray.Transform(transform->GetGlobalMatrix().Inverted());
			SelectItem(item->second);
			hits.clear();
			return true;
	}

	SelectItem(nullptr);

	hits.clear();
	
	return true;
		
}

void ModuleSceneIntro::SelectItem(ModuleGameObject* game_object)
{
	for (int i = 0; i < App->scene_intro->game_objects.size(); i++)
	{
		if (App->scene_intro->game_objects.at(i)->IsActive() && game_object != App->scene_intro->game_objects.at(i))
		{
			App->scene_intro->game_objects.at(i)->selectedForInspector = false;
			
		}
	}
	
	game_object->selectedForInspector = true;

	selectedGameObject = game_object;
	//App->scene_intro->rootObject = game_object;
}


float2 ModuleSceneIntro::getWorldMosuePosition()
{
	float win_width = (float)App->window->GetWidht();
	float win_height = (float)App->window->GetHeight();

	/*float tex_width = tex_size.x;
	float tex_height = tex_size.y;*/

	float mouse_X = (float)App->input->GetMouseX();
	float mouse_Y = (float)App->input->GetMouseY();

	float2 screen_mouse_pos = float2(mouse_X, win_height - mouse_Y); /*- float2(tex_origin.x, tex_origin.y + 22.5f);	*/			
	//float2 screen_mouse_pos = GetScreenMousePosition();
	float2 norm_screen_pos = float2(screen_mouse_pos.x , screen_mouse_pos.y );
	float2 world_mouse_pos = float2(norm_screen_pos.x * win_width, norm_screen_pos.y * win_height);

	//return world_mouse_pos;
	return screen_mouse_pos;
	//-------------------------------------------------------
	/*float2 win_mouse_motion = float2((float)App->input->GetMouseXMotion(), (float)App->input->GetMouseYMotion());
	float2 win_size = float2((float)App->window->GetWidht(), (float)App->window->GetHeight());
	float2 tex_size = float2(this->tex_size.x, this->tex_size.y);

	float2 local_motion = float2(win_mouse_motion.x / tex_size.x, win_mouse_motion.y / tex_size.y);
	float2 world_mouse_motion = float2(local_motion.x * win_size.x, local_motion.y * win_size.y);

	return world_mouse_motion;*/
}

void ModuleSceneIntro::CreateSceneCamera()
{
	ModuleGameObject* scene_camera;

	scene_camera = CreateEmptyGameObject("SceneCamera", nullptr);
	scene_camera->CreateComponent(COMPONENT_TYPES::CAMERA);
	ModuleComponentCamera* cameraSceneCreated = (ModuleComponentCamera*)scene_camera->GetComponent(COMPONENT_TYPES::CAMERA);
	cameraSceneCreated->SetAspectRatio(App->window->GetWidht() / App->window->GetHeight());

	ModuleComponentsTransform* setLocalPos = (ModuleComponentsTransform*)scene_camera->GetComponent(COMPONENT_TYPES::TRANSFORM);
	setLocalPos->SetPosition(float3(0.0f, 5.0f, 25.0f));
}

//void ModuleCamera3D::CreateGameCamera()
//{
//	game_camera = App->scene_intro->CreateEmptyGameObject("SceneCamera", nullptr);
//	//game_camera->SetName("GameCamera");
//	game_camera->CreateComponent(COMPONENT_TYPES::CAMERA);
//	/*game_camera->GetCameraComponent()->SetFarPlaneDistance(1000.0f);*/
//	ModuleComponentCamera* cameraCreated = (ModuleComponentCamera*)game_camera->GetComponent(COMPONENT_TYPES::CAMERA);
//	cameraCreated->SetFarPlaneDistance(1000.0f);
//
//	SetCurrentCamera(cameraCreated);
//
//	if (App != nullptr)
//	{
//		float win_width = (float)App->window->GetWidht();
//		float win_height = (float)App->window->GetHeight();
//
//		cameraCreated->SetAspectRatio(win_width / win_height);
//	}
//
//}


ModuleGameObject* ModuleSceneIntro::CreateMasterGameObject()
{
	rootObject = new ModuleGameObject(game_objects.size(), "Main scene", true, true);
	rootObject->CreateComponent(COMPONENT_TYPES::MATERIAL);
	rootObject->CreateComponent(COMPONENT_TYPES::MESH);
	rootObject->CreateComponent(COMPONENT_TYPES::TRANSFORM);
	//gameObject->CreateComponent(COMPONENT_TYPES::CAMERA);
	game_objects.push_back(rootObject);
	return rootObject;
}

ModuleGameObject* ModuleSceneIntro::GetSelectedGameObject()
{
	return selectedGameObject;
}

ModuleComponentCamera* ModuleSceneIntro::GetCullingCamera() const
{
	return culling_camera;
}

void ModuleSceneIntro::SetCullingCamera(ModuleComponentCamera* culling_camera)
{
	ModuleComponentCamera* prev_cull_cam = this->culling_camera;

	this->culling_camera = culling_camera;

	if (prev_cull_cam != nullptr)
	{
		if (prev_cull_cam != culling_camera)
		{
			prev_cull_cam->SetIsCulling(false);
		}
	}
}

void ModuleSceneIntro::DestroyGameObject(ModuleGameObject* GO)
{
	GO->components.clear();

	for (int i = 0; i < rootObject->childs.size(); i++)
	{
		if (rootObject->childs[i]->childs.empty() == false)
		{
			for (size_t j = 0; j < rootObject->childs[i]->childs.size(); j++)
			{
				if (rootObject->childs[i]->childs[j] == GO)
				{
					rootObject->childs[i]->childs.erase(rootObject->childs[i]->childs.begin() + j);
				}
			}
		}
		if (rootObject->childs[i] == GO)
		{
			rootObject->childs.erase(rootObject->childs.begin() + i);
		}
	}

	if (selectedGameObject == GO)
	{
		selectedGameObject = nullptr;
	}
}

void ModuleSceneIntro::CreateCustomParticleSystem(int type, float3 position, ModuleGameObject* owner)
{
	float3 defaultPos = { -1, -1, -1 };
	ModuleComponentsTransform* ownerTransform = (ModuleComponentsTransform*)owner->GetComponent(COMPONENT_TYPES::TRANSFORM);
	if (position.x != defaultPos.x && position.y != defaultPos.y && position.z != defaultPos.z)
		ownerTransform->SetPosition(position);

	//We make sure that particle_system is valid, or else we create it
	ModuleComponentParticles* ownerParticles = (ModuleComponentParticles*)owner->GetComponent(COMPONENT_TYPES::PARTICLES);
	if (ownerParticles == nullptr)
		owner->CreateComponent(COMPONENT_TYPES::PARTICLES);

	switch (type)
	{
	case ModuleParticles::None:
		LOG_COMMENT("Error. You are trying to create a <None Particle System>");
		break;
	case ModuleParticles::Custom:
	{
		//TODO: Create emitters elsewhere
		ownerParticles->emitters.push_back(EmitterInstance(new ParticleEmitter()));
		ownerParticles->emitters.back().owner = ownerParticles;	//Set EmitterInstance's owner
		ownerParticles->emitters.back().Init();
		CustomParticle* defaultParticle = new CustomParticle(owner);
		defaultParticle->name = "defaultParticle";
		ownerParticles->emitters[0].emitter->modules.push_back(defaultParticle);
		ownerParticles->emitters[0].UpdateParticleReference();
		//delete emitterReference;
		break;
	}
	case ModuleParticles::Smoke:
	{
		ownerParticles->emitters.push_back(EmitterInstance(new ParticleEmitter));

		Smoke* newSmoke = new Smoke(owner);
		ownerParticles->emitters[0].emitter->modules.push_back(newSmoke);

		ownerParticles->emitters.back().owner = ownerParticles;
		ownerParticles->emitters.back().Init();
		break;
	}
	case ModuleParticles::Firework:
	{
		ownerParticles->emitters.push_back(EmitterInstance(new ParticleEmitter()));
		ownerParticles->emitters.back().owner = ownerParticles;
		ownerParticles->emitters.back().Init();
		ownerParticles->emitters.back().UpdateParticleReference();
		Firework* firework = new Firework(owner);
		firework->name = "firework";
		ownerParticles->emitters.back().emitter->modules.push_back(firework);
		break;
	}
	default:
		break;
	}
}
