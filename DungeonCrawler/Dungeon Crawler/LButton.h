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
	void handleEvent(SDL_Event* e, int index);

	//Allows you to set button sprite
	void setSprite(LButtonSprite newsprite);

	//Shows button sprite
	void render();

	//set button clicked handler
	void setHandler(void(*new_handler)(int index));

	//set size
	void setConstraints(int w, int h);
	
	//setters
	void setSST(LTexture *SST_);
	void setClips(std::vector<SDL_Rect>  vec_);

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
	void(*handler)(int index);

	//disabled
	bool disabled;

	//clip vector ptr
	std::vector<SDL_Rect> vec;

	//SST
	LTexture *SST;

};

extern int quest_index;

void newGame(int index);
void loadGame(int index);
void tutorial(int index);
void credits(int index);

void char1Clicked(int index);
void char2Clicked(int index);
void char3Clicked(int index);

void gotoQuest(int index);
void questInfo(int index);
void questAccept(int index);
void questReject(int index);
void gotoCharUp(int index);
void gotoPartyUp(int index);
void gotoShop(int index);
void returnToTown(int index);

void wellHandler(int index);
void obHandler(int index);
void chestHandler(int index);
void tomeHandler(int index);
void strHandler(int index);
void dexHandler(int index);
void conHandler(int index);
void intHandler(int index);
void wisHandler(int index);
void chaHandler(int index);

void peek(int index);
void scout(int index);
void rest(int index);

void endTurnHandler(int index); 
void moveButton(int index);
void greatAxeButton(int index);

void emptyHandler(int index);

extern std::vector<LButton> Buttons;
extern std::vector<LButton> menuButtons;
extern std::vector<LButton> charButtons;
extern std::vector<LButton> questButtons;
extern std::vector<LButton> townButtons;
extern std::vector<LButton> acceptrejectButtons;
extern std::vector<LButton> dungeonButtons;
extern std::vector<LButton> choiceButtons;
extern LButton endTurnButton;

#endif