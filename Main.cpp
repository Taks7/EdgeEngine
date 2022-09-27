#include <stdlib.h>
#include "Application.h"
#include "Globals.h"

#include "SDL/include/SDL.h"
#pragma comment( lib, "SDL/libx86/SDL2.lib" )
#pragma comment( lib, "SDL/libx86/SDL2main.lib" )

enum main_states
{
	MAIN_CREATION,
	MAIN_START,
	MAIN_UPDATE,
	MAIN_FINISH,
	MAIN_EXIT
};

int main(int argc, char ** argv)
{
	LOG_COMMENT("Starting game '%s'...", TITLE);

	int main_return = EXIT_FAILURE;
	main_states state = MAIN_CREATION;
	Application* App = NULL;

	while (state != MAIN_EXIT)
	{
		switch (state)
		{
		case MAIN_CREATION:

			LOG_COMMENT("-------------- Application Creation --------------");
			App = new Application();
			state = MAIN_START;
			break;

		case MAIN_START:

			LOG_COMMENT("-------------- Application Init --------------");
			if (App->Init() == false)
			{
				LOG_COMMENT("Application Init exits with ERROR");
				state = MAIN_EXIT;
			}
			else
			{
				state = MAIN_UPDATE;
				LOG_COMMENT("-------------- Application Update --------------");
			}

			break;

		case MAIN_UPDATE:
		{
			int update_return = App->Update();

			if (update_return == NULL)
			{
				LOG_COMMENT("Application Update exits with ERROR");
				state = MAIN_EXIT;
			}
			
			if (update_return == false)
				state = MAIN_FINISH;
			
		}
			break;

		case MAIN_FINISH:

			LOG_COMMENT("-------------- Application CleanUp --------------");
			if (App->CleanUp() == false)
			{
				LOG_COMMENT("Application CleanUp exits with ERROR");
			}
			else
				main_return = EXIT_SUCCESS;

			state = MAIN_EXIT;

			break;

		}
	}

	delete App;
	LOG_COMMENT("Exiting game '%s'...\n", TITLE);
	return main_return;
}