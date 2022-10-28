#ifndef _INSPECTOR_MENU_H
#define _INSPECTOR_MENU_H

#include "UI.h"

class InspectorMenu : public UI
{
public:
	InspectorMenu();
	virtual ~InspectorMenu();

	void Draw() override;
};
#endif // !_ABOUT_MENU_H

