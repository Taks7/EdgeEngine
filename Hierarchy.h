#ifndef _HIERARCHY_MENU_H
#define _HIERARCHY_MENU_H

#include "UI.h"

class Hierarchy : public UI
{
public:
	Hierarchy();
	virtual ~Hierarchy();

	void Draw() override;

	void PopUpOptions();

	void ProcessGameObject(ModuleGameObject* GameObject);

	void SetHovered();

	
public:

	bool showPopUp = false;
	bool hovered = false;
};
#endif // !_ABOUT_MENU_H

