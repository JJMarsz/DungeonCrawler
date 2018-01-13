#include "LButton.h"
#include "party.h"

SDL_Color textColor = { 255, 255, 255 };

std::vector<LButton> Buttons;
std::vector<LButton> menuButtons;
std::vector<LButton> charButtons;
std::vector<LButton> questButtons;
std::vector<LButton> acceptrejectButtons;
std::vector<LButton> townButtons;

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
	switch (state) {
	case ROOM_MAIN:
		buttonSpriteSheetTexture.render(mPosition.x, mPosition.y, &buttonSpriteClips[mCurrentSprite]);
		break;
	case MAIN_MENU:
		buttonSpriteSheetTexture.render(mPosition.x, mPosition.y, &buttonSpriteClips[mCurrentSprite + 4]);
		break;
	case SELECTED_QUEST_ACCEPT:
		acceptrejectSST.render(mPosition.x, mPosition.y, &acceptrejectClips[mCurrentSprite]);
		break;
	case SELECTED_QUEST_REJECT:
		acceptrejectSST.render(mPosition.x, mPosition.y, &acceptrejectClips[mCurrentSprite+4]);
		break;
	case SELECTED_QUEST:
	case TOWN_QUEST_BOARD:
		questSST.render(mPosition.x, mPosition.y, &questPageClips[mCurrentSprite]);
		break;
	case TOWN_CHAR_UP:

		break;
	case TOWN_PARTY_UP:

		break;
	case TOWN_SHOP:

		break;
	case TOWN_BUTTON_LEFT:
		townButtonSST.render(mPosition.x, mPosition.y, &townButtonClips[mCurrentSprite]);
		break;
	case TOWN_BUTTON_RIGHT:
		townButtonSST.render(mPosition.x, mPosition.y, &townButtonClips[mCurrentSprite + 4]);
		break;
	case PICK_CHAR1:
	case PICK_CHAR2:
	case PICK_CHAR3:
		buttonSpriteSheetTexture.render(mPosition.x, mPosition.y, &buttonSpriteClips[mCurrentSprite + 8]);
		break;
	};

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

/* Character selection */
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
	chars.unshowChar();
	for (int i = 0; i < chars.size(); i++) {
		if (chars.isAvailable(i)) {
			gParty.addChar(i);
		}
	}
	//initTown();
	state = TOWN_QUEST_BOARD;
}

/* Town transition functions */
void gotoQuest(int index) {
	state = TOWN_QUEST_BOARD;
	townButtons[0].setHandler(gotoShop);
	townButtons[1].setHandler(gotoCharUp);
	leftText.loadFromRenderedText("Shop", textColor, 200);
	rightText.loadFromRenderedText("Training", textColor, 200);
}

void gotoCharUp(int index) {
	state = TOWN_CHAR_UP;
	townButtons[0].setHandler(gotoQuest);
	townButtons[1].setHandler(gotoPartyUp);
	leftText.loadFromRenderedText("Quests", textColor, 200);
	rightText.loadFromRenderedText("Upgrades", textColor, 200);
}

void gotoPartyUp(int index) {
	state = TOWN_PARTY_UP;
	townButtons[0].setHandler(gotoCharUp);
	townButtons[1].setHandler(gotoShop);
	leftText.loadFromRenderedText("Training", textColor, 200);
	rightText.loadFromRenderedText("Shop", textColor, 200);
}

void gotoShop(int index) {
	state = TOWN_SHOP;
	townButtons[0].setHandler(gotoPartyUp);
	townButtons[1].setHandler(gotoQuest);
	leftText.loadFromRenderedText("Upgrades", textColor, 200);
	rightText.loadFromRenderedText("Quests", textColor, 200);
}

void questInfo(int index) {
	state = SELECTED_QUEST;
}

void questAccept(int index) {
	//should generate based off of what quest
	current_dungeon = Dungeon(EASY);
	gParty.moveParty(current_dungeon.getStartX(), current_dungeon.getStartY());
	state = DUNGEON;
}

void questReject(int index) {
	state = TOWN_QUEST_BOARD;
}
/* Used primarily for testing */
void emptyHandler(int index) {
	return;
}
