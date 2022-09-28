#ifndef _UI_H_
#define _UI_H_

#include "Globals.h"
#include "Application.h"
#include <string>

//BASE CLASS FOR ALL UI ELEMENTS ALONGSIDE MODULEUI (something like entities in the other projects)
class UI
{
public:
	UI();
	virtual ~UI();

	virtual void Draw() {};
	bool IsActive() const;
	void switchActive();
	virtual bool CleanUp() { return true; }

	bool active = false;
private:
	
};


#endif // _UI_H_

