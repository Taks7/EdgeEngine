#ifndef __GameView_H__
#define __GameView_H__

#include "EditorPanel.h"

class GameView : public EditorPanel
{
public:
	GameView();
	~GameView();

	void Draw();
	bool CleanUp	() override;

private:


};

#endif // !__E_GAME_H__