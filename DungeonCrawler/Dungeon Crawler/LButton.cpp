#include "LButton.h"
#include "party.h"


std::vector<LButton> Buttons;
std::vector<LButton> menuButtons;
std::vector<LButton> charButtons;

/* Button class defenitions */
LButton::LButton()
{
	mPosition.x = 0;
	mPosition.y = 0;
	MouseDown = false;
	MouseUp = false;
	disabled = false;
	mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
	handler = emptyHandler;
	//buttonText = "";
}

void LButton::setPosition(int x, int y)
{
	mPosition.x = x;
	mPosition.y = y;
}

void LButton::handleEvent(SDL_Event* e, int index) {
	//If mouse event happened
	if (e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP)
	{
		//Get mouse position
		int x, y;
		SDL_GetMouseState(&x, &y);
		//Check if mouse is in button
		bool inside = true;

		//Mouse is outside of the button
		if (x < mPosition.x || x > mPosition.x + width || y < mPosition.y || y > mPosition.y + height) {
			inside = false;
		}//Mouse is outside button
		if (!inside) {
			mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
			MouseUp = false;
			MouseDown = false;
		}
		//Mouse is inside button
		else {
			//Set mouse over sprite
			switch (e->type) {
			case SDL_MOUSEMOTION:
				if (mCurrentSprite != BUTTON_SPRITE_MOUSE_DOWN)
					mCurrentSprite = BUTTON_SPRITE_MOUSE_OVER_MOTION;
				break;

			case SDL_MOUSEBUTTONDOWN:
				mCurrentSprite = BUTTON_SPRITE_MOUSE_DOWN;
				MouseDown = true;
				break;

			case SDL_MOUSEBUTTONUP:
				mCurrentSprite = BUTTON_SPRITE_MOUSE_UP;
				MouseUp = true;
				break;
			}
		}
		//button was clicked
		if (MouseDown == true && MouseUp == true) {
			MouseDown = false;
			MouseUp = false;
			handler(index);
		}
	}
}

void LButton::setSprite(LButtonSprite newsprite) {
	mCurrentSprite = newsprite;
}

void LButton::render() {
	//Show current button sprite
	if(state == ROOM_MAIN)
		buttonSpriteSheetTexture.render(mPosition.x, mPosition.y, &buttonSpriteClips[mCurrentSprite]);
	else if(state == MAIN_MENU)
		buttonSpriteSheetTexture.render(mPosition.x, mPosition.y, &buttonSpriteClips[mCurrentSprite+4]);
	else
		buttonSpriteSheetTexture.render(mPosition.x, mPosition.y, &buttonSpriteClips[mCurrentSprite + 8]);

}

void LButton::setHandler(void(*new_handler)(int index)) {
	handler = new_handler;
}

void LButton::setConstraints(int w, int h) {
	width = w;
	height = h;
}

int LButton::getWidth() {
	return width;
}

int LButton::getHeight() {
	return height;
}

//different button clicked function handlers
void menuClicked(int index) {
	charButtons[0].setHandler(char1Clicked);
	charButtons[1].setHandler(char1Clicked);
	charButtons[2].setHandler(char1Clicked);
	//generate chars
	chars.unshowChar();
	chars.getChars();
	state = PICK_CHAR1;
}

void char1Clicked(int index) {
	charButtons[0].setHandler(char2Clicked);
	charButtons[1].setHandler(char2Clicked);
	charButtons[2].setHandler(char2Clicked);
	chars.unshowChar();
	chars.pickChar(chars.getIndex(displayList[index].getName()));
	//generate chars
	chars.getChars();
	state = PICK_CHAR2;
}

void char2Clicked(int index) {
	charButtons[0].setHandler(char3Clicked);
	charButtons[1].setHandler(char3Clicked);
	charButtons[2].setHandler(char3Clicked);
	chars.unshowChar();
	chars.pickChar(chars.getIndex(displayList[index].getName()));
	//generate chars
	chars.getChars();
	state = PICK_CHAR3;
}

void char3Clicked(int index) {
	chars.pickChar(chars.getIndex(displayList[index].getName()));
	state = ROOM_MAIN;
}

void emptyHandler(int index) {
	return;
}
