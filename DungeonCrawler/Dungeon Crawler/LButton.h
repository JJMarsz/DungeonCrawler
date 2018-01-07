#ifndef LBUTTON_H
#define LBUTTON_H

#include "LTexture.h"
#include "lib.h"

//The mouse button
class LButton
{
public:
	//Initializes internal variables
	LButton();

	//Sets top left position
	void setPosition(int x, int y);

	//Handles mouse event
	void handleEvent(SDL_Event* e);

	//Allows you to set button sprite
	void setSprite(LButtonSprite newsprite);

	//Shows button sprite
	void render();

	//set button clicked handler
	void setHandler(void(*new_handler)(void));

	//set size
	void setConstraints(int w, int h);

	//getters
	int getWidth();
	int getHeight();

private:
	//Top left position
	SDL_Point mPosition;

	//contraints
	int width, height;

	//Currently used global sprite
	LButtonSprite mCurrentSprite;

	//Button Clicked
	bool MouseDown, MouseUp;

	//Button Clicked function handler
	void(*handler)(void);

	//disabled
	bool disabled;

};

void menuClicked(void);

void emptyHandler(void);

extern std::vector<LButton> Buttons;
extern std::vector<LButton> menuButtons;

#endif