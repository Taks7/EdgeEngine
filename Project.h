#ifndef __Project_H__
#define __Project_H__


#include "Globals.h"
#include "UI.h"
#include "FSdefinitions.h"

class Project : public UI
{
public:

	Project();
	virtual ~Project();

	void Draw();

	void DrawDirectoriesTree(const char* root_directory, const char* extension_to_filter);

	void HandleSelectedItem(const char* file_path);
private:

	char* selected_file;																				
	bool	ready_to_load;

};

#endif