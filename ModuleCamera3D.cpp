#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "ModuleCamera3D.h"
#include "Frustum.h"
#include "ModuleComponentCamera.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_sdl.h"
#include "ImGui/imgui_impl_opengl2.h"

#include <vector>
#include "Frustum.h"
#include "MathGeoLib.h"
#pragma comment (lib, "MathGeo/lib/MathGeoLib.lib")

#define ZOOM_SPEED 2.0f
#define ROTATION_SPEED 0.2f


ModuleCamera3D::ModuleCamera3D(bool start_enabled) : Module(start_enabled)
{
	/*CreateGameCamera();*/
	name = "Camera";

	CalculateViewMatrix();

	X = vec3(1.0f, 0.0f, 0.0f);
	Y = vec3(0.0f, 1.0f, 0.0f);
	Z = vec3(0.0f, 0.0f, 1.0f);

	Position = vec3(0.0f, 0.0f, 5.0f);
	Reference = vec3(0.0f, 0.0f, 0.0f);

	zoomSpeed = ZOOM_SPEED;

	
}

ModuleCamera3D::~ModuleCamera3D()
{}

// -----------------------------------------------------------------
bool ModuleCamera3D::Start()
{
	LOG_COMMENT("Setting up the camera");
	bool ret = true;

	return ret;
}

// -----------------------------------------------------------------
bool ModuleCamera3D::CleanUp()
{
	LOG_COMMENT("Cleaning camera");

	return true;
}

// -----------------------------------------------------------------
bool ModuleCamera3D::Update(float dt)
{
	// Implement a debug camera with keys and mouse
	// Now we can make this movememnt frame rate independant!

	vec3 newPos(0,0,0);
	float speed = 3.0f * dt;

	App->renderer3D->DrawRaycast();

	if (App->input->GetKey(SDL_SCANCODE_Z) == KEY_DOWN)
	{
		CastRay();
	}
	

	if(App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		speed = 8.0f * dt;

	if(App->input->GetKey(SDL_SCANCODE_R) == KEY_REPEAT) newPos.y += speed;

	if(App->input->GetKey(SDL_SCANCODE_C) == KEY_REPEAT) newPos.y -= speed;

	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_REPEAT)
	{
		LookAt(0);
	}

	if(App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) newPos -= Z * speed;
	if(App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) newPos += Z * speed;


	if(App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) newPos -= X * speed;
	if(App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) newPos += X * speed;

	// Aqui con poner Look at(gameObjectSelected position ya estaria)if (App->input->GetKey(SDL_SCANCODE_F) == KEY_REPEAT) LookAt();

	Position += newPos;
	Reference += newPos;
	zoomSpeed = App->camera->GetZoomSpeed();
	App->camera->SetZoomSpeed(zoomSpeed);

	// Mouse motion ----------------
	
	if(App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
	{
		int dx = -App->input->GetMouseXMotion();
		int dy = -App->input->GetMouseYMotion();

		float Sensitivity = 0.25f;

		if(dx != 0)
		{
			float DeltaX = (float)dx * Sensitivity;

			X = rotate(X, DeltaX, vec3(0.0f, 1.0f, 0.0f));
			Y = rotate(Y, DeltaX, vec3(0.0f, 1.0f, 0.0f));
			Z = rotate(Z, DeltaX, vec3(0.0f, 1.0f, 0.0f));
		}

		if(dy != 0)
		{
			float DeltaY = (float)dy * Sensitivity;

			Y = rotate(Y, DeltaY, X);
			Z = rotate(Z, DeltaY, X);

			if(Y.y < 0.0f)
			{
				Z = vec3(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
				Y = cross(Z, X);
			}
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT)
	{
		if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT)
		{
			int dx = -App->input->GetMouseXMotion();
			int dy = -App->input->GetMouseYMotion();

			float sensitivity = ROTATION_SPEED;

			Position -= Reference;

			if (dx != 0)
			{
				float delta_X = (float)dx * sensitivity;

				X = rotate(X, delta_X, vec3(0.0f, 1.0f, 0.0f));
				Y = rotate(Y, delta_X, vec3(0.0f, 1.0f, 0.0f));
				Z = rotate(Z, delta_X, vec3(0.0f, 1.0f, 0.0f));
			}

			if (dy != 0)
			{
				float delta_Y = (float)dy * sensitivity;

				Y = rotate(Y, delta_Y, X);
				Z = rotate(Z, delta_Y, X);

				if (Y.y < 0.0f)
				{
					Z = vec3(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
					Y = cross(Z, X);
				}
			}

			Position = Reference + Z * length(Position);
		}
	}

	if (App->input->GetMouseZ() != 0)
	{
		Zoom();
	}

	//Recalculate Matrix -------------

	CalculateViewMatrix();

	return true;
}

void ModuleCamera3D::CreateGameCamera()
{
	game_camera = App->scene_intro->CreateEmptyGameObject("GameCamera", nullptr);
	//game_camera->SetName("GameCamera");
	game_camera->CreateComponent(COMPONENT_TYPES::CAMERA);
	/*game_camera->GetCameraComponent()->SetFarPlaneDistance(1000.0f);*/
	ModuleComponentCamera* cameraCreated = (ModuleComponentCamera*)game_camera->GetComponent(COMPONENT_TYPES::CAMERA);
	cameraCreated->SetFarPlaneDistance(1000.0f);

	SetCurrentCamera(cameraCreated);

	if (App != nullptr)
	{
		float win_width = (float)App->window->GetWidht();
		float win_height = (float)App->window->GetHeight();

		cameraCreated->SetAspectRatio(win_width / win_height);
	}

}

ModuleComponentCamera* ModuleCamera3D::GetCurrentCamera() const
{
	return current_camera;
}

void ModuleCamera3D::SetCurrentCamera(ModuleComponentCamera* module_component_camera)
{
	if (module_component_camera == nullptr)
	{
		LOG_COMMENT("[ERROR] Camera: Could not set a new current camera! Error: Given Camera Component was nullptr.");
		return;
	}

	if (module_component_camera->GetOwner() == nullptr)																						
	{
		LOG_COMMENT("[ERROR] Camera: Could not set a new current camera! Error: Given Camera Component's owner was nullptr.");
		return;
	}
	//Error Frustum
	/*if (current_camera != nullptr)
	{
		current_camera->SetFrustumIsHidden(false);
	}*/
	
	module_component_camera->SetFrustumIsHidden(true);
	current_camera = module_component_camera;
	current_camera->SetUpdateProjectionMatrix(true);
	
	if (App != nullptr)																										
	{
		current_camera->SetAspectRatio(((float)App->window->GetHeight()) / ((float)App->window->GetHeight()));
	}
}

void ModuleCamera3D::SetMasterCameraAsCurrentCamera()
{
	current_camera->SetFrustumIsHidden(false);
	
	if (master_camera == nullptr)
	{
		CreateGameCamera();
	}

	ModuleComponentCamera* masterCamera = (ModuleComponentCamera*)master_camera->GetComponent(COMPONENT_TYPES::CAMERA);
	if (masterCamera == nullptr)
	{
		master_camera->CreateComponent(COMPONENT_TYPES::CAMERA);
		/*ModuleComponentCamera* masterCamera = (ModuleComponentCamera*)master_camera;*/
	}

	current_camera = (ModuleComponentCamera*)master_camera->GetComponent(COMPONENT_TYPES::CAMERA);
	current_camera->SetUpdateProjectionMatrix(true);
}

void ModuleCamera3D::CastRay()
{
	
	ModuleComponentCamera* currentCam = (ModuleComponentCamera*)game_camera->GetComponent(COMPONENT_TYPES::CAMERA);
	float2 mouse_pos = App->scene_intro->getWorldMosuePosition();

	float norm_mouse_X = mouse_pos.x / (float)App->window->GetWidht();
	float norm_mouse_Y = mouse_pos.y / (float)App->window->GetHeight();

	float ray_origin_X = (norm_mouse_X - 0.5f) * 2;
	float ray_origin_Y = (norm_mouse_Y - 0.5f) * 2;

	//float x = ((ImGui::GetMousePos().x / (float)App->window->GetWidht()) * 2.0f)/* - 1.0f*/;
	//float y = 1 - ((ImGui::GetMousePos().y / (float)App->window->GetHeight()) * 2.0f);

	last_raycast = currentCam->GetFrustum().UnProjectLineSegment(ray_origin_X, ray_origin_Y);

	/*std::map<float, ModuleGameObject*> hits;
	App->scene_intro->getRaycastHits(helperRay, hits);*/

	App->scene_intro->RaycastSelection(last_raycast);
	
	
}
// -----------------------------------------------------------------
void ModuleCamera3D::Look(const vec3 &Position, const vec3 &Reference, bool RotateAroundReference)
{
	this->Position = Position;
	this->Reference = Reference;

	Z = normalize(Position - Reference);
	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
	Y = cross(Z, X);

	if(!RotateAroundReference)
	{
		this->Reference = this->Position;
		this->Position += Z * 0.05f;
	}

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
void ModuleCamera3D::LookAt( const vec3 &Spot)
{
	Reference = Spot;

	Z = normalize(Position - Reference);
	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
	Y = cross(Z, X);

	CalculateViewMatrix();
}


// -----------------------------------------------------------------
void ModuleCamera3D::Move(const vec3 &Movement)
{
	Position += Movement;
	Reference += Movement;

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
float* ModuleCamera3D::GetViewMatrix()
{
	return &ViewMatrix;
}

// -----------------------------------------------------------------
void ModuleCamera3D::CalculateViewMatrix()
{
	ViewMatrix = mat4x4(X.x, Y.x, Z.x, 0.0f, X.y, Y.y, Z.y, 0.0f, X.z, Y.z, Z.z, 0.0f, -dot(X, Position), -dot(Y, Position), -dot(Z, Position), 1.0f);
	ViewMatrixInverse = inverse(ViewMatrix);
}

//------------------------------------------------------------------
void ModuleCamera3D::Zoom()
{
	Position -= Z * App->input->GetMouseZ() * zoomSpeed;
	/*Z = normalize(Position - Reference);*/
}

void ModuleCamera3D::SetZoomSpeed(const float& zoom_speed)
{
	this->zoomSpeed = zoom_speed;
}

float ModuleCamera3D::GetZoomSpeed() const
{
	return zoomSpeed;
}

float3 ModuleCamera3D::ToFloat3(vec3 vec) {
	return float3(vec.x, vec.y, vec.z);
}
vec3 ModuleCamera3D::ToVec3(float3 vec) {
	return vec3(vec.x, vec.y, vec.z);
}

