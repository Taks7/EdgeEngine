
#include "Globals.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "ModuleMaterials.h"
#include "ModuleComponentMesh.h"
#include "ModuleComponentTransform.h"
#include "ModuleComponentCamera.h"
#include "SDL\include\SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */
#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */

#define WORLD_GRID_SIZE 64

ModuleRenderer3D::ModuleRenderer3D(bool start_enabled) : Module(start_enabled)
{
	name = "Renderer"; 

	 atributes.Depth_test = true;
	 atributes.Cull_Face = true;
	 atributes.Lightning = true;
	 atributes.Color_Materials = true;
	 atributes.Texture_2D = true;
	 atributes.Front = true;
	 atributes.AmbientOclussion = true;
	 atributes.Wireframe = false;

	 ckeckerTextureid = 0;
}

// Destructor
ModuleRenderer3D::~ModuleRenderer3D()
{}

// Called before render is available
bool ModuleRenderer3D::Init()
{
	LOG_COMMENT("Creating 3D Renderer context");
	bool ret = true;
	
	//Create context
	context = SDL_GL_CreateContext(App->window->window);
	if(context == NULL)
	{
		LOG_COMMENT("OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	
	if(ret == true)
	{
		//Use Vsync
		if(VSYNC && SDL_GL_SetSwapInterval(1) < 0)
			LOG_COMMENT("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());

		//Initialize Projection Matrix
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		//Check for error
		GLenum error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG_COMMENT("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}

		//Initialize Modelview Matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG_COMMENT("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}
		
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glClearDepth(1.0f);
		
		//Initialize clear color
		glClearColor(0.f, 0.f, 0.f, 1.f);

		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG_COMMENT("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}
		
		GLfloat LightModelAmbient[] = {0.0f, 0.0f, 0.0f, 1.0f};
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LightModelAmbient);
		
		lights[0].ref = GL_LIGHT0;
		lights[0].ambient.Set(0.25f, 0.25f, 0.25f, 1.0f);
		lights[0].diffuse.Set(0.75f, 0.75f, 0.75f, 1.0f);
		lights[0].SetPos(0.0f, 0.0f, 2.5f);
		lights[0].Init();
		
		GLfloat MaterialAmbient[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);

		GLfloat MaterialDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);
	
		lights[0].Active(true);
	}
	

	// Projection matrix for
	OnResize(SCREEN_WIDTH, SCREEN_HEIGHT);


	GLenum error = glewInit();
	if (GLEW_OK != error)
	{
		LOG_COMMENT("Glew failed error %s\n", glewGetErrorString(error));
	}
	LOG_COMMENT("Glew version: %s\n", glewGetString(GLEW_VERSION));

	App->materialImport->Init();



	return ret;
}

// PreUpdate: clear buffer
bool ModuleRenderer3D::PreUpdate(float dt)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	/*glLoadMatrixf(App->camera->GetViewMatrix());*/

	ModuleComponentCamera* camera = App->camera->GetCurrentCamera();

	if (camera != nullptr)
	{
		if (camera->GetUpdateProjectionMatrix())
		{
			RecalculateProjectionMatrix();
			camera->SetUpdateProjectionMatrix(false);
		}
	}

	/*glLoadMatrixf(App->camera->GetViewMatrix());*/
	glLoadMatrixf((GLfloat*)camera->GetOGLViewMatrix());

	float3 camera_position = float3::zero;
	if (App->camera->GetCurrentCamera() != nullptr)
	{
		camera_position = App->camera->GetPosition();
	}
	else
	{
		camera_position = float3(0.0f, 20.0f, 0.0f);
	}

	if (atributes.Depth_test == true)
	{
		glEnable(GL_DEPTH_TEST);
	}
	if (atributes.Depth_test == false)
	{
		glDisable(GL_DEPTH_TEST);
	}
	if (atributes.Cull_Face == true)
	{
		glEnable(GL_CULL_FACE);
	}
	if (atributes.Cull_Face == false)
	{
		glDisable(GL_CULL_FACE);
	}
	if (atributes.Lightning == true)
	{
		glEnable(GL_LIGHTING);
	}
	if (atributes.Lightning == false)
	{
		glDisable(GL_LIGHTING);
	}
	if (atributes.Color_Materials == true)
	{
		glEnable(GL_COLOR_MATERIAL);
	}
	if (atributes.Color_Materials == false)
	{
		glDisable(GL_COLOR_MATERIAL);
	}
	if (atributes.Front == true)
	{
		glEnable(GL_FRONT);
	}
	if (atributes.Front == false)
	{
		glDisable(GL_FRONT);
	}
	if (atributes.AmbientOclussion == true)
	{
		glEnable(GL_AMBIENT);
	}
	if (atributes.AmbientOclussion == false)
	{
		glDisable(GL_AMBIENT);
	}
	if (atributes.Wireframe == true)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	if (atributes.Wireframe == false)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}



	// light 0 on cam pos
	lights[0].SetPos(App->camera->Position.x, App->camera->Position.y, App->camera->Position.z);

	for(uint i = 0; i < MAX_LIGHTS; ++i)
		lights[i].Render();

	

	return true;
}

// PostUpdate present buffer to screen
bool ModuleRenderer3D::PostUpdate()
{
	/*Render();*/

	SDL_GL_SwapWindow(App->window->window);
	return true;
}

// Called before quitting
bool ModuleRenderer3D::CleanUp()
{
	LOG_COMMENT("Destroying 3D Renderer");

	SDL_GL_DeleteContext(context);

	return true;
}

void ModuleRenderer3D::DrawGameObjects(ModuleGameObject GameObject,float4x4 transform)
{
	if (GameObject.IsActive())
	{	
		ModuleComponentsMesh* NewMesh = (ModuleComponentsMesh*)GameObject.GetComponent(COMPONENT_TYPES::MESH);
		ModuleComponentMaterial* NewMaterial = (ModuleComponentMaterial*)GameObject.GetComponent(COMPONENT_TYPES::MATERIAL);
		if (NewMesh->IsActive())
		{
			if (NewMesh != nullptr)
			{
				glPushMatrix();
				glMultMatrixf((GLfloat*)&transform.Transposed());
				
				glEnableClientState(GL_VERTEX_ARRAY);
				// Render things in Element mode
				glBindBuffer(GL_ARRAY_BUFFER, NewMesh->mesh.id_vertex);
				glVertexPointer(3, GL_FLOAT, 0, nullptr);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NewMesh->mesh.id_index);


				glEnableClientState(GL_TEXTURE_COORD_ARRAY);
				glBindBuffer(GL_ARRAY_BUFFER, NewMesh->mesh.id_uvs);

				if (GameObject.bouindingBoxes)
				{
					glBegin(GL_LINES);

					glVertex3f(NewMesh->mesh.aabb.maxPoint.x, NewMesh->mesh.aabb.maxPoint.y, NewMesh->mesh.aabb.maxPoint.z);
					glVertex3f(NewMesh->mesh.aabb.maxPoint.x, NewMesh->mesh.aabb.minPoint.y, NewMesh->mesh.aabb.maxPoint.z);

					glVertex3f(NewMesh->mesh.aabb.maxPoint.x, NewMesh->mesh.aabb.maxPoint.y, NewMesh->mesh.aabb.maxPoint.z);
					glVertex3f(NewMesh->mesh.aabb.minPoint.x, NewMesh->mesh.aabb.maxPoint.y, NewMesh->mesh.aabb.maxPoint.z);

					glVertex3f(NewMesh->mesh.aabb.maxPoint.x, NewMesh->mesh.aabb.maxPoint.y, NewMesh->mesh.aabb.maxPoint.z);
					glVertex3f(NewMesh->mesh.aabb.maxPoint.x, NewMesh->mesh.aabb.maxPoint.y, NewMesh->mesh.aabb.minPoint.z);

					glVertex3f(NewMesh->mesh.aabb.maxPoint.x, NewMesh->mesh.aabb.minPoint.y, NewMesh->mesh.aabb.minPoint.z);
					glVertex3f(NewMesh->mesh.aabb.maxPoint.x, NewMesh->mesh.aabb.maxPoint.y, NewMesh->mesh.aabb.minPoint.z);

					glVertex3f(NewMesh->mesh.aabb.maxPoint.x, NewMesh->mesh.aabb.minPoint.y, NewMesh->mesh.aabb.minPoint.z);
					glVertex3f(NewMesh->mesh.aabb.maxPoint.x, NewMesh->mesh.aabb.minPoint.y, NewMesh->mesh.aabb.maxPoint.z);

					glVertex3f(NewMesh->mesh.aabb.maxPoint.x, NewMesh->mesh.aabb.minPoint.y, NewMesh->mesh.aabb.minPoint.z);
					glVertex3f(NewMesh->mesh.aabb.minPoint.x, NewMesh->mesh.aabb.minPoint.y, NewMesh->mesh.aabb.minPoint.z);

					glVertex3f(NewMesh->mesh.aabb.minPoint.x, NewMesh->mesh.aabb.maxPoint.y, NewMesh->mesh.aabb.minPoint.z);
					glVertex3f(NewMesh->mesh.aabb.minPoint.x, NewMesh->mesh.aabb.minPoint.y, NewMesh->mesh.aabb.minPoint.z);

					glVertex3f(NewMesh->mesh.aabb.minPoint.x, NewMesh->mesh.aabb.maxPoint.y, NewMesh->mesh.aabb.minPoint.z);
					glVertex3f(NewMesh->mesh.aabb.minPoint.x, NewMesh->mesh.aabb.maxPoint.y, NewMesh->mesh.aabb.maxPoint.z);

					glVertex3f(NewMesh->mesh.aabb.minPoint.x, NewMesh->mesh.aabb.maxPoint.y, NewMesh->mesh.aabb.minPoint.z);
					glVertex3f(NewMesh->mesh.aabb.maxPoint.x, NewMesh->mesh.aabb.maxPoint.y, NewMesh->mesh.aabb.minPoint.z);

					glVertex3f(NewMesh->mesh.aabb.minPoint.x, NewMesh->mesh.aabb.minPoint.y, NewMesh->mesh.aabb.maxPoint.z);
					glVertex3f(NewMesh->mesh.aabb.minPoint.x, NewMesh->mesh.aabb.maxPoint.y, NewMesh->mesh.aabb.maxPoint.z);

					glVertex3f(NewMesh->mesh.aabb.minPoint.x, NewMesh->mesh.aabb.minPoint.y, NewMesh->mesh.aabb.maxPoint.z);
					glVertex3f(NewMesh->mesh.aabb.maxPoint.x, NewMesh->mesh.aabb.minPoint.y, NewMesh->mesh.aabb.maxPoint.z);

					glVertex3f(NewMesh->mesh.aabb.minPoint.x, NewMesh->mesh.aabb.minPoint.y, NewMesh->mesh.aabb.maxPoint.z);
					glVertex3f(NewMesh->mesh.aabb.minPoint.x, NewMesh->mesh.aabb.minPoint.y, NewMesh->mesh.aabb.minPoint.z);


					glEnd();
				}
				if (NewMaterial->IsActive())
				{
					if (NewMaterial->materialUsed != nullptr)
					{
						glTexCoordPointer(3, GL_FLOAT, 0, nullptr);
						glBindTexture(GL_TEXTURE_2D, NewMaterial->materialUsed->id);
					}
				}

				if (App->renderer3D->checkerTextureApplied)
				{
					glBindTexture(GL_TEXTURE_2D, ckeckerTextureid);
				}



				glDrawElements(GL_TRIANGLES, NewMesh->mesh.num_index, GL_UNSIGNED_INT, nullptr);

				glBindTexture(GL_TEXTURE_2D, 0);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
				glBindBuffer(GL_ARRAY_BUFFER, 0);
				glDisableClientState(GL_TEXTURE_COORD_ARRAY);
				glDisableClientState(GL_VERTEX_ARRAY);


				glPopMatrix();
			}

		}
	}

}

bool ModuleRenderer3D::LoadConfig(JsonParsing& node)
{	
	atributes.Depth_test = node.GetJsonBool("depth test");
	atributes.Cull_Face = node.GetJsonBool("cull face");
	atributes.Lightning = node.GetJsonBool("lightning");
	atributes.Color_Materials = node.GetJsonBool("color materials");
	atributes.Texture_2D = node.GetJsonBool("texture 2D");
	atributes.Front = node.GetJsonBool("front");
	atributes.AmbientOclussion = node.GetJsonBool("ambient oclussion");
	atributes.Wireframe = node.GetJsonBool("wireframe");
	
	return true;
}

bool ModuleRenderer3D::SaveConfig(JsonParsing& node) const
{
	node.SetNewJsonBool(node.ValueToObject(node.GetRootValue()), "depth test", atributes.Depth_test);
	node.SetNewJsonBool(node.ValueToObject(node.GetRootValue()), "cull face", atributes.Cull_Face);
	node.SetNewJsonBool(node.ValueToObject(node.GetRootValue()), "lightning", atributes.Lightning);
	node.SetNewJsonBool(node.ValueToObject(node.GetRootValue()), "color materials", atributes.Color_Materials);
	node.SetNewJsonBool(node.ValueToObject(node.GetRootValue()), "texture 2D", atributes.Texture_2D);
	node.SetNewJsonBool(node.ValueToObject(node.GetRootValue()), "front", atributes.Front);
	node.SetNewJsonBool(node.ValueToObject(node.GetRootValue()), "ambient oclussion", atributes.AmbientOclussion);
	node.SetNewJsonBool(node.ValueToObject(node.GetRootValue()), "wireframe", atributes.Wireframe);

	return true;
}


void ModuleRenderer3D::OnResize(int width, int height)
{
	glViewport(0, 0, width, height);

	if (App->camera->GetCurrentCamera() != nullptr)
	{
		App->camera->GetCurrentCamera()->SetAspectRatio(width / height);
	}
	else
	{
		LOG_COMMENT("[ERROR] Renderer 3D: Could not recalculate the aspect ratio! Error: Current Camera was nullptr.");
	}

	RecalculateProjectionMatrix();
}

void ModuleRenderer3D::LoadCheckerTexture()
{
	GLubyte checker[CHEIGHT][CWIDTH][4];					

	for (int i = 0; i < CHEIGHT; ++i)									
	{
		for (int j = 0; j < CWIDTH; ++j)								
		{
			int color = ((((i & 0x8) == 0) ^ ((j & 0x8) == 0))) * 255;			

			checker[i][j][0] = (GLubyte)color;
			checker[i][j][1] = (GLubyte)color;
			checker[i][j][2] = (GLubyte)color;
			checker[i][j][3] = (GLubyte)255;

		}
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);																									
	glGenTextures(1, &ckeckerTextureid);
	glBindTexture(GL_TEXTURE_2D, ckeckerTextureid);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);				
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);				
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);			
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);	
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, CWIDTH, CHEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, checker);

	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void ModuleRenderer3D::RecalculateProjectionMatrix()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if (App->camera->GetCurrentCamera() != nullptr)
	{
		glLoadMatrixf((GLfloat*)App->camera->GetCurrentCamera()->GetOGLProjectionMatrix());
	}
	else
	{
		LOG_COMMENT("[ERROR] The Camera was nullptr.");
	}

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

uint ModuleRenderer3D::GetSceneRenderTexture() const
{
	return scene_render_texture;
}

void ModuleRenderer3D::DrawRaycast()
{
	glBegin(GL_LINES);

	glVertex3d(App->camera->last_raycast.a.x, App->camera->last_raycast.a.y, App->camera->last_raycast.a.z);
	glVertex3d(App->camera->last_raycast.b.x, App->camera->last_raycast.b.y, App->camera->last_raycast.b.z);

	glEnd();

}