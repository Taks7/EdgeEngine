#ifndef __ModuleComponentBillboard_H__
#define __ModuleComponentBillboard_H__

#include "ModuleComponent.h"
#include "ModuleFBXLoader.h"
#include "ModuleComponentCamera.h"
#include "ModuleComponentTransform.h"
#include "Color.h"
#include <string>

class ModuleGameObject;

enum Billboarding_Alignment {
	SCREEN_ALIGNED = 0,
	WORLD_ALIGNED,
	AXIS_ALIGNED
};

class ModuleComponentBillBoard : public ModuleComponents {
public:
	ModuleComponentBillBoard(ModuleGameObject* owner);
	~ModuleComponentBillBoard();

	bool Update() override;
	void Draw(Color color);
	void FaceCamera();

	void SetAlignment(Billboarding_Alignment new_alignment);
	std::string GetAlignmentText();
private:
	void ScreenAlign();
	void WorldAlign();
	void AxisAlign();
public:
	ModuleComponentsTransform* transform;
	VertexData* res_mesh;

	float3 pos;
private:
	Billboarding_Alignment billboard_alignment;
	Texture* res_texture;

};

#endif