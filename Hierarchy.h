#ifndef _HIERARCHY_MENU_H
#define _HIERARCHY_MENU_H

#include "UI.h"

class Hierarchy : public UI
{
public:
	Hierarchy();
	virtual ~Hierarchy();

	void Draw() override;

	void PopUpOptions(ModuleGameObject* gameObject);

	void ProcessGameObject(ModuleGameObject* GameObject);

	
public:

	bool showPopUp;
};
#endif // !_ABOUT_MENU_H

