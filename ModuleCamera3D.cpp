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

#define ZOOM_SPEED 200.0f
#define ROTATION_SPEED 0.00009f
#define MOVEMENT_SPEED 8.0f



ModuleCamera3D::ModuleCamera3D(bool start_enabled) : Module(start_enabled),
game_camera(nullptr),
current_camera(nullptr)
{
	name = "Camera";

	/*CreateGameCamera();*/

	/*CalculateViewMatrix();*/

	X = vec3(1.0f, 0.0f, 0.0f);
	Y = vec3(0.0f, 1.0f, 0.0f);
	Z = vec3(0.0f, 0.0f, 1.0f);

	Position = vec3(0.0f, 0.0f, 5.0f);
	Reference = vec3(0.0f, 0.0f, 0.0f);


	movementSpeed = MOVEMENT_SPEED;
	rotationSpeed = ROTATION_SPEED;
	zoomSpeed = ZOOM_SPEED;
}

ModuleCamera3D::~ModuleCamera3D()
{
	current_camera = nullptr;

	game_camera->CleanUp();
	RELEASE(game_camera);
}

bool ModuleCamera3D::Init()
{
	CreateGameCamera();

	ModuleComponentsTransform* masterCameraTransform = (ModuleComponentsTransform*)game_camera->GetComponent(COMPONENT_TYPES::TRANSFORM);
	masterCameraTransform->SetPosition(float3(60.0f, 40.0f, 60.0f));
	LookAt(Reference);

	return true;
}

// -----------------------------------------------------------------
bool ModuleCamera3D::Start()
{
	LOG_COMMENT("Setting up the camera");
	bool ret = true;

	/*CreateGameCamera();*/

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

	//vec3 newPos(0,0,0);
	//float speed = 3.0f * dt;

	//
	//
	//

	//if(App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
	//	speed = 8.0f * dt;

	//if(App->input->GetKey(SDL_SCANCODE_R) == KEY_REPEAT) newPos.y += speed;

	//if(App->input->GetKey(SDL_SCANCODE_C) == KEY_REPEAT) newPos.y -= speed;

	//if (App->input->GetKey(SDL_SCANCODE_F) == KEY_REPEAT)
	//{
	//	LookAt(0);
	//}

	//if(App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) newPos -= Z * speed;
	//if(App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) newPos += Z * speed;


	//if(App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) newPos -= X * speed;
	//if(App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) newPos += X * speed;

	//// Aqui con poner Look at(gameObjectSelected position ya estaria)if (App->input->GetKey(SDL_SCANCODE_F) == KEY_REPEAT) LookAt();

	//Position += newPos;
	//Reference += newPos;
	//zoomSpeed = App->camera->GetZoomSpeed();
	//App->camera->SetZoomSpeed(zoomSpeed);

	//// Mouse motion ----------------
	//
	//if(App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
	//{
	//	int dx = -App->input->GetMouseXMotion();
	//	int dy = -App->input->GetMouseYMotion();

	//	float Sensitivity = 0.25f;

	//	if(dx != 0)
	//	{
	//		float DeltaX = (float)dx * Sensitivity;

	//		X = rotate(X, DeltaX, vec3(0.0f, 1.0f, 0.0f));
	//		Y = rotate(Y, DeltaX, vec3(0.0f, 1.0f, 0.0f));
	//		Z = rotate(Z, DeltaX, vec3(0.0f, 1.0f, 0.0f));
	//	}

	//	if(dy != 0)
	//	{
	//		float DeltaY = (float)dy * Sensitivity;

	//		Y = rotate(Y, DeltaY, X);
	//		Z = rotate(Z, DeltaY, X);

	//		if(Y.y < 0.0f)
	//		{
	//			Z = vec3(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
	//			Y = cross(Z, X);
	//		}
	//	}
	//}

	//if (App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT)
	//{
	//	if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT)
	//	{
	//		int dx = -App->input->GetMouseXMotion();
	//		int dy = -App->input->GetMouseYMotion();

	//		float sensitivity = ROTATION_SPEED;

	//		Position -= Reference;

	//		if (dx != 0)
	//		{
	//			float delta_X = (float)dx * sensitivity;

	//			X = rotate(X, delta_X, vec3(0.0f, 1.0f, 0.0f));
	//			Y = rotate(Y, delta_X, vec3(0.0f, 1.0f, 0.0f));
	//			Z = rotate(Z, delta_X, vec3(0.0f, 1.0f, 0.0f));
	//		}

	//		if (dy != 0)
	//		{
	//			float delta_Y = (float)dy * sensitivity;

	//			Y = rotate(Y, delta_Y, X);
	//			Z = rotate(Z, delta_Y, X);

	//			if (Y.y < 0.0f)
	//			{
	//				Z = vec3(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
	//				Y = cross(Z, X);
	//			}
	//		}

	//		Position = Reference + Z * length(Position);
	//	}
	//}

	//if (App->input->GetMouseZ() != 0)
	//{
	//	Zoom();
	//}

	////Recalculate Matrix -------------

	//CalculateViewMatrix();

    FreeMovement();

	App->renderer3D->DrawRaycast();

	if (App->input->GetKey(SDL_SCANCODE_Z) == KEY_DOWN)
	{
		CastRay();
	}

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

	float3 initialPos = float3(10, 5, 5);
	cameraCreated->SetPosition(initialPos);

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

	if (current_camera != nullptr)
	{
		current_camera->SetFrustumIsHidden(false);
	}
	
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
	
	if (game_camera == nullptr)
	{
		CreateGameCamera();
	}

	ModuleComponentCamera* masterCamera = (ModuleComponentCamera*)game_camera->GetComponent(COMPONENT_TYPES::CAMERA);
	if (masterCamera == nullptr)
	{
		game_camera->CreateComponent(COMPONENT_TYPES::CAMERA);
		/*ModuleComponentCamera* masterCamera = (ModuleComponentCamera*)game_camera;*/
	}

	current_camera = (ModuleComponentCamera*)game_camera->GetComponent(COMPONENT_TYPES::CAMERA);
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

float3 ModuleCamera3D::GetPosition() const
{
	return current_camera->GetFrustum().Pos();
}

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

void ModuleCamera3D::LookAtFloat3(const float3& spot)											
{
	current_camera->LookAt(spot);
	reference = spot;
}


// -----------------------------------------------------------------
void ModuleCamera3D::FreeMovement()
{
	float3 new_position = float3::zero;
	Frustum frustum = current_camera->GetFrustum();
	float mov_speed = movementSpeed;

	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_STATE::KEY_REPEAT)								
	{																									 
		mov_speed = movementSpeed * 2 * App->Dt();										
	}																									


	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_STATE::KEY_REPEAT)									
	{																									 
		new_position += frustum.Front() * mov_speed * App->Dt();
	}																									 
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_STATE::KEY_REPEAT)									 
	{																									 
		new_position -= frustum.Front() * mov_speed * App->Dt();
	}																									


	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_STATE::KEY_REPEAT)									
	{																									 										
		new_position -= frustum.WorldRight() * mov_speed * App->Dt();
	}																									 										
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_STATE::KEY_REPEAT)									 										
	{																									 										
		new_position += frustum.WorldRight() * mov_speed * App->Dt();
	}																									


	if (App->input->GetKey(SDL_SCANCODE_R) == KEY_STATE::KEY_REPEAT)
	{																									
		new_position += frustum.Up() * mov_speed * App->Dt();
	}																									
	if (App->input->GetKey(SDL_SCANCODE_C) == KEY_STATE::KEY_REPEAT)
	{																									
		new_position -= frustum.Up() * mov_speed * App->Dt();
	}	

	if (App->input->GetMouseZ() != 0)
	{
		Frustum frustum = current_camera->GetFrustum();
		float3 new_Z = frustum.Front() * (float)App->input->GetMouseZ() * zoomSpeed * App->Dt();

		MoveCameras(new_Z);
	}

	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_STATE::KEY_DOWN)
	{
		float3 zero = float3::zero;
		float3 five = float3(0, 5, -10);
		float3 ten = float3(0, -10, 15);
		PointAt(five, ten, false);
	}

	//error al mover la camara
	/*if (App->input->GetMouseButton(SDL_BUTTON_MIDDLE) == KEY_STATE::KEY_REPEAT)
	{
		float3 new_X = float3::zero;
		float3 new_Y = float3::zero;
		float3 new_position = float3::zero;

		Frustum frustum = current_camera->GetFrustum();
		float2 mouse_motion = App->scene_intro->getWorldMosuePosition();

		if (mouse_motion.x != 0)
		{
			new_X = -mouse_motion.x * frustum.WorldRight();
		}

		if (mouse_motion.y != 0)
		{
			new_Y = mouse_motion.y * frustum.Up();
		}

		new_position = new_X + new_Y;

		MoveCameras(new_position);
	}*/

	if (App->input->GetKey(SDL_SCANCODE_LALT) == KEY_STATE::KEY_REPEAT)
	{
		if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_STATE::KEY_REPEAT)
		{
			if (App->scene_intro->GetSelectedGameObject() != nullptr)
			{
				if (App->scene_intro->GetSelectedGameObject()->GetComponent(COMPONENT_TYPES::CAMERA) != current_camera)
				{
					ModuleComponentsTransform* getWorldPos;
					getWorldPos = (ModuleComponentsTransform*)App->scene_intro->GetSelectedGameObject()->GetComponent(COMPONENT_TYPES::TRANSFORM);
					reference = getWorldPos->GetWorldPosition();
				}
			}
			else
			{
				reference = float3::zero;
			}

			Frustum frustum = current_camera->GetFrustum();
			float2 mouse_motion = App->scene_intro->getWorldMosuePosition();
			float sensitivity = rotationSpeed * App->Dt();

			float3 new_Z = frustum.Pos() - reference;

			if (mouse_motion.x != 0.0f)
			{
				Quat new_X = Quat(frustum.Up(), -mouse_motion.x * sensitivity);
				new_Z = new_X.Transform(new_Z);
			}

			if (mouse_motion.y != 0.0f)
			{
				Quat new_Y = Quat(frustum.WorldRight(), -mouse_motion.y * sensitivity);
				new_Z = new_Y.Transform(new_Z);
			}

			float3 new_position = new_Z + reference;

			PointAt(new_position, reference, true);
		}
	}

	MoveCameras(new_position);
}

void ModuleCamera3D::Move(const vec3 &Movement)
{
	Position += Movement;
	Reference += Movement;

	CalculateViewMatrix();
}

void ModuleCamera3D::MoveCameras(const float3& velocity)
{
	current_camera->Move(velocity);
	reference += velocity;
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

void ModuleCamera3D::PointAt(const float3& position, const float3& target, bool RotateAroundReference)
{
	current_camera->PointAt(position, target);
	reference = target;

	if (!RotateAroundReference)
	{
		reference = position;

		Frustum frustum = current_camera->GetFrustum();
		current_camera->SetPosition(frustum.Pos() + frustum.Front() * 0.05f);
	}
}

void ModuleCamera3D::Focus(const float3& target, const float& distance_from_target)
{
	current_camera->Focus(target, distance_from_target);
	reference = target;
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

