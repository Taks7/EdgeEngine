#ifndef _ABOUT_MENU_H
#define _ABOUT_MENU_H

#include "UI.h"

class AboutMenu : public UI
{
public:
	AboutMenu();
	virtual ~AboutMenu();

	void Draw() override;
};
#endif // !_ABOUT_MENU_H

