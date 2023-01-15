#include "ModuleParticles.h"
#include "MathGeoLib.h"
#include "ModuleCamera3D.h"
#include "ModuleFBXLoader.h"
#include "ModuleSceneIntro.h"
#include "scene.h"
#include "Color.h"

ModuleParticles::ModuleParticles()
{

	existing_particles = 0;
	activeParticles = 0;
	lastUsedParticle = 0;
	particleReference = new Particles(); //TODO: Duplicate data. particleReference in EmitterInstance and C_ParticleSystem should only be initialized once
	particleReference->position = { 0,0,0 };
	particleReference->lifetime = 0;
	particleReference->billboard = nullptr;
}

void ModuleParticles::Init(EmitterInstance* emitterInstance)
{
	existing_particles = 0;
	activeParticles = 0;
	lastUsedParticle = 0;
}

void ModuleParticles::Spawn(EmitterInstance* emitterInstance)
{
	if (existing_particles < emitterInstance->owner->maxParticles) {
		//Create new particles until the vector is full
		CreateParticle(emitterInstance);
	}
	else if (GetFirstUnusedParticle() != -1 && activeParticles < emitterInstance->owner->maxParticles)
	{
		ModuleComponentsTransform* emmiterTransform = (ModuleComponentsTransform*)emitterInstance->owner->owner->GetComponent(COMPONENT_TYPES::TRANSFORM);
		uint index = GetFirstUnusedParticle();
		particles_vector[index].active = true; activeParticles++; //Reactivate particle     
		particleReference->position = emmiterTransform->GetPosition(); //Get position from C_Transform

		//We update values from Particle Reference
		particles_vector[index].position = particleReference->position;
		particles_vector[index].lifetime = particleReference->lifetime;
		particles_vector[index].direction = particleReference->direction + SetRandomDirection();
		particles_vector[index].size = particleReference->size;
		particles_vector[index].speed = particleReference->speed;
		particles_vector[index].color = particleReference->color;
	}
}

void ModuleParticles::Update(EmitterInstance* emitterInstance)
{
	Spawn(emitterInstance);

	for (size_t i = 0; i < particles_vector.size(); i++)
	{
		if (particles_vector[i].active)
		{
			particles_vector[i].position += particles_vector[i].speed * particles_vector[i].direction;
			particles_vector[i].distanceToCamera = CalculateParticleDistanceToCamera(&particles_vector[i]);
		}
	}

	SortParticles(particles_vector);

	DrawParticles();
	DeActivateParticles();
}

void ModuleParticles::Reset()
{
	CleanUp();
}

void ModuleParticles::CleanUp()
{
}

void ModuleParticles::DrawParticles()
{
	for (size_t i = 0; i < particles_vector.size(); i++)
	{
		if (particles_vector[i].active)

		{
			particles_vector[i].billboard->transform->GetPosition() = particles_vector[i].position;
			particles_vector[i].billboard->transform->GetScale() = float3(particles_vector[i].size);
			particles_vector[i].billboard->Draw(particles_vector[i].color);

			//Particula dibujada con GL_POINTS, no esta claro si funciona bieen
			//particles_vector[i].position += particles_vector[i].speed * particles_vector[i].direction * App->Dt();
			////particles_vector[i].lifetime -= App->Dt();
			//glColor4f(particles_vector[i].color.r, particles_vector[i].color.g, particles_vector[i].color.b, particles_vector[i].color.a);
			//glPointSize(particles_vector[i].size);
			//glBegin(GL_POINTS);
			//glVertex3f(particles_vector[i].position.x, particles_vector[i].position.y, particles_vector[i].position.z);
			//glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
			//glEnd();

		}
	}
}

void ModuleParticles::DeActivateParticles()
{
	for (size_t i = 0; i < particles_vector.size(); i++)
	{
		if (particles_vector[i].lifetime <= 0)
		{
			if (particles_vector[i].active == true) {
				activeParticles--;
				particles_vector[i].active = false;
			}
		}
	}
}

void ModuleParticles::Save()
{

}

void ModuleParticles::Load()
{
}

float ModuleParticles::CalculateParticleDistanceToCamera(Particles* p)
{
	return  sqrt(pow(App->camera->Position.x - p->position.x, 2) +
		pow(App->camera->Position.y - p->position.y, 2) +
		pow(App->camera->Position.z - p->position.z, 2) * 1.0);
}

unsigned int ModuleParticles::GetFirstUnusedParticle()
{
	// first search from last used particle, this will usually return almost instantly
	for (unsigned int i = lastUsedParticle; i < particles_vector.size(); ++i) {
		if (particles_vector[i].lifetime <= 0.0f) {
			lastUsedParticle = i;
			return i;
		}
	}
	// otherwise, do a linear search
	for (unsigned int i = 0; i < lastUsedParticle; ++i) {
		if (particles_vector[i].lifetime <= 0.0f) {
			lastUsedParticle = i;
			return i;
		}
	}
	return -1; //Return -1 if there are not unused particles
}

void ModuleParticles::UpdateParticleReference(EmitterInstance* emitterInstance)
{
	ModuleComponentsTransform* emitterInstanceTransform = (ModuleComponentsTransform*)emitterInstance->owner->owner->GetComponent(COMPONENT_TYPES::TRANSFORM);

	particleReference->position = emitterInstanceTransform->GetPosition();
	particleReference->lifetime = emitterInstance->owner->particleReference->lifetime;
	particleReference->color = emitterInstance->owner->particleReference->color;
	particleReference->direction = emitterInstance->owner->particleReference->direction;
	particleReference->dirVariation = emitterInstance->owner->particleReference->dirVariation;
	particleReference->size = emitterInstance->owner->particleReference->size;
	particleReference->speed = emitterInstance->owner->particleReference->speed;
}

void ModuleParticles::SortParticles(std::vector<Particles>& particles)
{
	std::sort(particles.begin(), particles.end(), ReOrderParticles());
}

void ModuleParticles::CreateParticle(EmitterInstance* emitterInstance)
{
	Particles* newParticle = new Particles(particleReference);
	
	if (emitterInstance->owner->owner->GetComponent(COMPONENT_TYPES::BILLBOARD) == false)
	{
		newParticle->billboard = (ModuleComponentBillBoard*)emitterInstance->owner->owner->CreateComponent(COMPONENT_TYPES::BILLBOARD);
	}
	else
	{
		newParticle->billboard = (ModuleComponentBillBoard*)emitterInstance->owner->owner->GetComponent(COMPONENT_TYPES::BILLBOARD);
	}
	
	
	//newParticle->billboard = emitterInstance->owner->owner->GetComponent(COMPONENT_TYPES::BILLBOARD);
	
	if (newParticle != nullptr)
	{
		particles_vector.push_back(newParticle);
		existing_particles++;
		activeParticles++;
		//particles_vector[particles_vector.size()-1].color = owner->GetRandomColor(owner->color);
		particles_vector[particles_vector.size() - 1].direction = particleReference->direction + SetRandomDirection();
	}
	else LOG_COMMENT("Error creating particles in the Particle Emitter Instance. newParticle was nulltr.")
}

float3 ModuleParticles::SetRandomDirection()
{
	LCG lcg;
	float3 randomInSphere = float3::RandomSphere(lcg, { 0,0,0 }, 1);
	float3 variation = randomInSphere.Normalized();
	variation.x = variation.x * particleReference->dirVariation * DEGTORAD;
	variation.y = variation.y * particleReference->dirVariation * DEGTORAD;
	variation.z = variation.z * particleReference->dirVariation * DEGTORAD;

	float3 dir = particleReference->direction.Normalized() + variation;

	return dir.Normalized();
}

Firework::Firework(ModuleGameObject* owner)
{
	ModuleComponentsTransform* fireworkOwnerTransform = (ModuleComponentsTransform*)fireworkOwner->GetComponent(COMPONENT_TYPES::TRANSFORM);

	fireworkOwner = owner;
	currentTime = 0.0f;
	lifeTime = 0.7f; //Firework goes up for x seconds
	particleReference->active = false;
	particleReference->billboard = nullptr;
	//particleReference->color = Green; //Color not initialized because it's latter overwritten by the rangeColor
	rangeColor = { {0,0,0,1}, {1,1,1,1} };
	particleReference->direction = { 0,1,0 };
	particleReference->dirVariation = 0.0f;
	particleReference->distanceToCamera = NULL;
	particleReference->lifetime = 1.0; //Particle lifetime
	particleReference->position = fireworkOwnerTransform->GetPosition();
	particleReference->size = 2.0f;
	particleReference->speed = 2.0f;

	ModuleComponentParticles* particle_system = (ModuleComponentParticles*)owner->GetComponent(COMPONENT_TYPES::PARTICLES);
	particle_system->UpdateParticleGUI(particleReference);

	//Set Resource
	particle_system->particle_material = (ModuleComponentMaterial*)owner->GetComponent(COMPONENT_TYPES::MATERIAL);
	std::string resourceName = "fire";
	/*Resource* resourceFireWork = App->resources->GetResourceByName(&resourceName);
	if (resourceFireWork != nullptr)
		particle_system->particle_material->SetTexture(resourceFireWork->GetUID());*/
}

Firework::~Firework()
{

}

void Firework::Update(EmitterInstance* emitterInstance)
{
	if (currentTime < lifeTime) //This is when the firework should go up
	{
		ModuleComponentsTransform* fireworkOwnerTransform = (ModuleComponentsTransform*)fireworkOwner->GetComponent(COMPONENT_TYPES::TRANSFORM);

		float3 lastPos = fireworkOwnerTransform->GetPosition();

		fireworkOwnerTransform->SetPosition({ lastPos.x, lastPos.y + 2.0f, lastPos.z });
		particleReference->position = fireworkOwnerTransform->GetPosition();
		Spawn(emitterInstance);
	}
	else if (currentTime < lifeTime * 2) { //This is when the firework should explode
		particleReference->dirVariation = 360.0f;
		particleReference->speed = 100.0f;
		particleReference->size = 5.0f;
		Spawn(emitterInstance);
	}
	else if (currentTime > lifeTime * 4) { //This is when the firework should die
		CleanUp();
	}
	currentTime += App->Dt();

	for (size_t i = 0; i < particles_vector.size(); i++)
	{
		if (particles_vector[i].active)
		{
			particles_vector[i].lifetime -= App->Dt();
			particles_vector[i].position += particles_vector[i].speed * particles_vector[i].direction * App->Dt();
			particles_vector[i].distanceToCamera = CalculateParticleDistanceToCamera(&particles_vector[i]);
		}
	}

	SortParticles(particles_vector);

	DeActivateParticles();
}

void Firework::Spawn(EmitterInstance* emitterInstance)
{
	if (existing_particles < emitterInstance->owner->maxParticles) {
		//Create new particles until the vector is full
		particleReference->color = GetRandomColor(rangeColor); //Error al poner el color
		Particles* newParticle = new Particles(particleReference);

		newParticle->billboard = (ModuleComponentBillBoard*)emitterInstance->owner->owner->CreateComponent(COMPONENT_TYPES::BILLBOARD);

		if (newParticle != nullptr)
		{
			particles_vector.push_back(newParticle);
			existing_particles++;
			activeParticles++;
			//particles_vector[particles_vector.size()-1].color = owner->GetRandomColor(owner->color);
			particles_vector[particles_vector.size() - 1].direction = particleReference->direction + SetRandomDirection();
		}
		else LOG_COMMENT("Error creating particles in the Particle Emitter Instance. newParticle was nulltr.")
	}
}

void Firework::CleanUp()
{
	for (size_t i = 0; i < particles_vector.size(); i++)
	{
		particles_vector[i].~Particles();
		particles_vector.erase(particles_vector.begin(), particles_vector.end());
	}
	fireworkOwner->SetActive(false);
	App->scene_intro->DestroyGameObject(fireworkOwner);
	fireworkOwner = nullptr;
	delete fireworkOwner;
}

//Error al poner el color
Color Firework::GetRandomColor(range<Color> r)
{
	Color c;
	c.r = (ldexp(pcg32_random(), -32) * (r.max.r - r.min.r)) + r.min.r;
	c.g = (ldexp(pcg32_random(), -32) * (r.max.g - r.min.g)) + r.min.g;
	c.b = (ldexp(pcg32_random(), -32) * (r.max.b - r.min.b)) + r.min.b;
	c.a = (ldexp(pcg32_random(), -32) * (r.max.a - r.min.a)) + r.min.a;

	return c;
}

CustomParticle::CustomParticle(ModuleGameObject* owner)
{
}

CustomParticle::~CustomParticle()
{
}

void CustomParticle::CleanUp()
{
	for (size_t i = 0; i < particles_vector.size(); i++)
	{
		//particles_vector[i].~Particle();
		particles_vector.erase(particles_vector.begin(), particles_vector.end());
	}
}

Smoke::Smoke(ModuleGameObject* owner)
{
	//Set up particleReference
	ModuleComponentsTransform* owner_transform = (ModuleComponentsTransform*)owner->GetComponent(COMPONENT_TYPES::PARTICLES);

	particleReference->position = owner_transform->GetPosition();
	particleReference->speed = 0.3f;
	particleReference->color = { 0.3, 0.3, 0.3, 1.0 };
	particleReference->size = 0.5f;
	particleReference->dirVariation = 40;
	particleReference->direction = { 0, 1, 0 };
	particleReference->lifetime = 5.0f;

	ModuleComponentParticles* particle_system = (ModuleComponentParticles*)owner->GetComponent(COMPONENT_TYPES::PARTICLES);
	particle_system->UpdateParticleGUI(particleReference);

	//Set up material
	particle_system->particle_material = (ModuleComponentMaterial*)owner->GetComponent(COMPONENT_TYPES::MATERIAL);

	//Set Resource
	std::string resourceName1 = "smoke1";

	Texture* newTexture = new Texture();
	App->materialImport->Import("Assets/Textures/smoke1.png", newTexture);
	//particle_system->particle_material->materialUsed = newTexture;
	particle_system->particle_material->SetTexture(newTexture);
	/*Resource* resourceSmoke1 = App->fs->GetResourceByName(&resourceName1); 
	if (resourceSmoke1 != nullptr) particle_system->particle_material->SetTexture(resourceSmoke1->GetUID());*/
}

Smoke::~Smoke()
{
}

/*void Smoke::Update(EmitterInstance* emitterInstance)
{
}*/

void Smoke::CleanUp()
{
}

