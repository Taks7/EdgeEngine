#include "UI.h"

UI::UI()
{
}

UI::~UI()
{

}

bool UI::IsActive() const
{
	return active;
}
void UI::switchActive()
{
	active = !active;
}
