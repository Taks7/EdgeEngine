#ifndef __PARTICLES_H__
#define __PARTICLES_H__

#include "MathGeo/src/Math/float3.h"
#include "MathGeo/src/Math/float4.h"
#include "MathGeo/src/Math/Quat.h"

//#include "Component_Billboard.h"  //TODO:ModuleComponentBillboard
#include "Color.h"

class Particles {
public:
	Particles();
	Particles(Particles* particleReference);
	~Particles();
public:
	float3 position;
	float lifetime;
	float speed;
	float size;
	float dirVariation;
	Color color;
	float3 direction;
	bool active;
	float distanceToCamera;

	/*ModuleComponentBillboard* billboard;*/ //TODO:ModuleComponentBillboard
};

#endif // !__PARTICLES_H__