#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "ModuleUI.h"
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
		game_objects.push_back(gameObject);
		return gameObject;
	}
	

	if (parent != nullptr)
	{
		ModuleGameObject* gameObject_child = new ModuleGameObject(parent->childs.size(), name, true, false);
		gameObject_child->CreateComponent(COMPONENT_TYPES::MATERIAL);
		gameObject_child->CreateComponent(COMPONENT_TYPES::MESH);
		gameObject_child->CreateComponent(COMPONENT_TYPES::TRANSFORM);
		parent->childs.push_back(gameObject_child);
		return gameObject_child;

	}
	
}
void ModuleSceneIntro::getRaycastHits(const LineSegment& ray, std::map<float, ModuleGameObject*>& hits)
{
	for (uint i = 0; i < game_objects.size(); ++i)
	{
		ModuleComponentsMesh* gameObjMesh = (ModuleComponentsMesh*)game_objects[i]->GetComponent(COMPONENT_TYPES::MESH);
		if (ray.Intersects(gameObjMesh->mesh.aabb))
		{
			ModuleComponentsTransform* gameObjTransform = (ModuleComponentsTransform*)game_objects[i]->GetComponent(COMPONENT_TYPES::TRANSFORM);
			float3 position = gameObjTransform->GetGlobalPosition();
			hits.emplace(ray.Distance(position), game_objects[i]);
		}
	}
}
/*
//TODO 9: And change the color of the colliding bodies, so we can visualize it working!
void ModuleSceneIntro::SelectThroughRaycast(const LineSegment& ray)
{
	std::map<float, ModuleGameObject*> hits;
	getRaycastHits(ray, hits);

	if (hits.size() == 0)
	{
		hits.clear();
		return;
	}

	std::map<float, ModuleGameObject*>::iterator item;
	for (item = hits.begin(); item != hits.end(); ++item)
	{
		item->second->GetAllMeshComponents(c_meshes);

		std::vector<Triangle> faces;
		for (uint m = 0; m < c_meshes.size(); ++m)
		{
			R_Mesh* r_mesh = c_meshes[m]->GetMesh();

			if (r_mesh == nullptr)
			{
				continue;
			}

			LineSegment local_ray = ray;
			local_ray.Transform(item->second->GetTransformComponent()->GetWorldTransform().Inverted());

			GetFaces(r_mesh->vertices, faces);
			for (uint f = 0; f < faces.size(); ++f)
			{
				if (local_ray.Intersects(faces[f], nullptr, nullptr))
				{
					SetSelectedGameObject(item->second);

					faces.clear();
					c_meshes.clear();
					hits.clear();

					return;
				}
			}

			faces.clear();
		}

		c_meshes.clear();
	}

	// If no GameObject was hit
	SetSelectedGameObject(nullptr);

	hits.clear();
}
*/