#ifndef _INSPECTOR_MENU_H
#define _INSPECTOR_MENU_H

#include "UI.h"

class InspectorMenu : public UI
{
public:
	InspectorMenu();
	virtual ~InspectorMenu();

	ModuleGameObject* GetSelectedGameObject();
	void DrawInfoOfGameObject(ModuleGameObject* game_object);
	void DrawGameObjectComponents();
	void DrawTransformComponent();
	void DrawMeshComponent();
	void DrawMaterialComponent();
	void Draw() override;

	ModuleGameObject* selectedGameObject;

};
#endif // !_ABOUT_MENU_H

