#ifndef _INSPECTOR_MENU_H
#define _INSPECTOR_MENU_H

#include "UI.h"

class InspectorMenu : public UI
{
public:
	InspectorMenu();
	virtual ~InspectorMenu();

	void DrawInfoOfGameObject(ModuleGameObject* game_object);
	void DrawGameObjectComponents(ModuleGameObject* selectedGameObject);
	void DrawTransformComponent(ModuleGameObject* selectedGameObject);
	void DrawMeshComponent(ModuleGameObject* selectedGameObject);
	void DrawMaterialComponent(ModuleGameObject* selectedGameObject);
	void DrawCameraComponent(ModuleGameObject* selectedGameObject);
	void DrawParticlesComponent(ModuleGameObject* selectedGameObject);
	void Draw() override;

	

};
#endif // !_ABOUT_MENU_H

