#include "LButton.h"
#include "party.h"
#include "Quest.h"

SDL_Color textColor = { 255, 255, 255 };

int quest_index = 0;

std::vector<LButton> Buttons;
std::vector<LButton> menuButtons;
std::vector<LButton> charButtons;
std::vector<LButton> questButtons;
std::vector<LButton> acceptrejectButtons;
std::vector<LButton> townButtons;
std::vector<LButton> dungeonButtons;
std::vector<LButton> choiceButtons;

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
	SST->render(mPosition.x, mPosition.y, &vec[mCurrentSprite]);
}

void LButton::setHandler(void(*new_handler)(int index)) {
	handler = new_handler;
}

void LButton::setConstraints(int w, int h) {
	width = w;
	height = h;
}

void LButton::setSST(LTexture* SST_) { SST = SST_; }
void LButton::setClips(std::vector<SDL_Rect>  vec_) { vec = vec_; }

int LButton::getWidth() {
	return width;
}

int LButton::getHeight() {
	return height;
}


//different button clicked function handlers
void newGame(int index) {
	charButtons[0].setHandler(char1Clicked);
	charButtons[1].setHandler(char1Clicked);
	charButtons[2].setHandler(char1Clicked);
	//generate chars
	chars.unshowChar();
	chars.getChars();
	state = PICK_CHAR1;
}

void loadGame(int index) {
	return;
}

void tutorial(int index) {
	return;
}

void credits(int index) {
	return;
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
	goldmenu.loadFromRenderedText("0", textColor, 200);
	xp0.loadFromRenderedText("0", textColor, 200);
	xp1.loadFromRenderedText("0", textColor, 200);
	xp2.loadFromRenderedText("0", textColor, 200);
	completed.loadFromRenderedText("0", textColor, 200);
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
	quest_index = index;
	questTitle.loadFromRenderedText(current_quests[index].getTitle(), textColor, 250);
	questInfoText.loadFromRenderedText(current_quests[index].getInfo(), textColor, 250);
	questGold.loadFromRenderedText("Gold: " + std::to_string(current_quests[index].getGold()), textColor, 200);
	questXP.loadFromRenderedText("XP: " + std::to_string(current_quests[index].getXP()), textColor, 200);
	questDiff.loadFromRenderedText("Difficulty: " + diffToString(current_quests[index].getDiff()), textColor, 200);
}

void questAccept(int index) {
	//should generate based off of what quest
	SDL_Color Color = { 0, 0, 0 };
	current_dungeon = Dungeon(current_quests[quest_index].getDiff());
	dungText1.loadFromRenderedText("Rest", Color, 200);
	dungText2.loadFromRenderedText("Scout", Color, 200);
	dungText3.loadFromRenderedText("Peek", Color, 200);
	gParty.moveParty(current_dungeon.getStartX(), current_dungeon.getStartY());
	quests.unShowQuest();
	current_dungeon.updateLOS();
	state = DUNGEON;
}

void questReject(int index) {
	state = TOWN_QUEST_BOARD;
}

//handlers for choice tile

void wellHandler(int index) {
	state = DUNGEON;
	msg_queue.pop();
	int DC = 8;
	std::string name;

	std::chrono::milliseconds ms = std::chrono::duration_cast< std::chrono::milliseconds >(
		std::chrono::system_clock::now().time_since_epoch()
		);
	srand(ms.count());//random seed
	int roll = (rand() % 20) + 1;
	if (current_dungeon.getScouted(gParty.getX() + gParty.getY()*current_dungeon.getWidth()))
		roll += 5;
	switch (index) {
	case 0:
	case 1:
	case 2:
		name = gParty.getChar(index)->getName();
		if (roll > DC) {
			gParty.getChar(index)->heal(gParty.getChar(index)->getMaxHP() / 2);
			msg_queue.push("The well replenishes the health of the " + name + ".");
		}
		else {
			gParty.getChar(index)->damage(gParty.getChar(index)->getHP() / 2);
			msg_queue.push("The well turns hostile and harms the " + name + ".");
		}
		break;
	case 3:
		msg_queue.push("The party rejects the well's calling.");
		break;
	}
}

void obHandler(int index) {

	state = DUNGEON;
	msg_queue.pop();
	int DC = 8;
	std::string name;

	std::chrono::milliseconds ms = std::chrono::duration_cast< std::chrono::milliseconds >(
		std::chrono::system_clock::now().time_since_epoch()
		);
	srand(ms.count());//random seed
	int roll = (rand() % 20) + 1;
	if (current_dungeon.getScouted(gParty.getX() + gParty.getY()*current_dungeon.getWidth()))
		roll += 5;
	switch (index) {
	case 0:
	case 1:
	case 2:
		name = gParty.getChar(index)->getName();
		if (roll > DC) {
			gParty.getChar(index)->addXP(1);
			msg_queue.push("The obelisk accepts the courage of the " + name + ".");
		}
		else {
			gParty.getChar(index)->subXP(1);
			msg_queue.push("The obelisk glows red and weakens the " + name + ".");
		}
		break;
	case 3:
		msg_queue.push("The party denies the power of the obelisk.");
		break;
	}
}

void chestHandler(int index) {
	state = DUNGEON;
	msg_queue.pop();
	int DC = 8;
	std::string name;
	std::chrono::milliseconds ms = std::chrono::duration_cast< std::chrono::milliseconds >(
		std::chrono::system_clock::now().time_since_epoch()
		);
	srand(ms.count());//random seed
	int roll = (rand() % 20) + 1;
	if (current_dungeon.getScouted(gParty.getX() + gParty.getY()*current_dungeon.getWidth()))
		roll += 5;
	switch (index) {
	case 0:
	case 1:
	case 2:
		name = gParty.getChar(index)->getName();
		if (roll > DC) {
			gParty.addGold(100 + current_quests[quest_index].getDiff()*50);
			msg_queue.push("The chest opens revealing its riches to the " + name + ".");
		}
		else {
			gParty.getChar(index)->damage(gParty.getChar(index)->getHP() / 2);
			msg_queue.push("The chest was a mimic! It damages the " + name + " before the party can fend it off.");
		}
		break;
	case 3:
		msg_queue.push("The party lets the chest be.");
		break;
	}
}
void tomeHandler(int index) {
	state = DUNGEON;
	msg_queue.pop();
	int DC = 10;
	std::string name;
	std::chrono::milliseconds ms = std::chrono::duration_cast< std::chrono::milliseconds >(
		std::chrono::system_clock::now().time_since_epoch()
		);
	srand(ms.count());//random seed
	int roll = (rand() % 20) + 1;
	if (current_dungeon.getScouted(gParty.getX() + gParty.getY()*current_dungeon.getWidth()))
		roll += 5;
	switch (index) {
	case 0:
	case 1:
	case 2:
		name = gParty.getChar(index)->getName();
		if (roll > DC) {
			gParty.getChar(index)->setMaxHP((gParty.getChar(index)->getMaxHP()*115)/100);
			msg_queue.push("The tome teaches the " + name + " how to be more resiliant.");
		}
		else {
			gParty.getChar(index)->setMaxHP((gParty.getChar(index)->getMaxHP() * 85) / 100);
			msg_queue.push("The tome upsets the " + name + " as they become a bit more fragile.");
		}
		break;
	case 3:
		msg_queue.push("The party lets the book rest.");
		break;
	}
}
void strHandler(int index) {
	state = DUNGEON;
	msg_queue.pop();
	int DC = 15;
	std::string name;
	std::chrono::milliseconds ms = std::chrono::duration_cast< std::chrono::milliseconds >(
		std::chrono::system_clock::now().time_since_epoch()
		);
	srand(ms.count());//random seed
	int roll = (rand() % 20) + 1;
	if (current_dungeon.getScouted(gParty.getX() + gParty.getY()*current_dungeon.getWidth()))
		roll += 4;
	switch (index) {
	case 0:
	case 1:
	case 2:
		roll += gParty.getChar(index)->getStr();
		name = gParty.getChar(index)->getName();
		if (roll > DC) {
			gParty.getChar(index)->setStr(gParty.getChar(index)->getStr()+1);
			msg_queue.push("The feat was completed! The " + name + " grows in strength!");
		}
		else {
			gParty.getChar(index)->setStr(gParty.getChar(index)->getStr() - 1);
			msg_queue.push("The feat was too difficult for the " + name + ". They grow weaker.");
		}
		break;
	case 3:
		msg_queue.push("The party leaves the feat of strength uncompleted.");
		break;
	}
}
void dexHandler(int index) {
	state = DUNGEON;
	msg_queue.pop();
	int DC = 15;
	std::string name;
	std::chrono::milliseconds ms = std::chrono::duration_cast< std::chrono::milliseconds >(
		std::chrono::system_clock::now().time_since_epoch()
		);
	srand(ms.count());//random seed
	int roll = (rand() % 20) + 1;
	if (current_dungeon.getScouted(gParty.getX() + gParty.getY()*current_dungeon.getWidth()))
		roll += 4;
	switch (index) {
	case 0:
	case 1:
	case 2:
		roll += gParty.getChar(index)->getDex();
		name = gParty.getChar(index)->getName();
		if (roll > DC) {
			gParty.getChar(index)->setDex(gParty.getChar(index)->getDex() + 1);
			msg_queue.push("The " + name + " was fast enough to complete the trial. Their agility grows!");
		}
		else {
			gParty.getChar(index)->setDex(gParty.getChar(index)->getDex() - 1);
			msg_queue.push("The " + name + " wasn't quick enough. They feel demoralized and less dexterous.");
		}
		break;
	case 3:
		msg_queue.push("The party leaves the trial un tested.");
		break;
	}
}
void conHandler(int index) {
	state = DUNGEON;
	msg_queue.pop();
	int DC = 15;
	std::string name;
	std::chrono::milliseconds ms = std::chrono::duration_cast< std::chrono::milliseconds >(
		std::chrono::system_clock::now().time_since_epoch()
		);
	srand(ms.count());//random seed
	int roll = (rand() % 20) + 1;
	if (current_dungeon.getScouted(gParty.getX() + gParty.getY()*current_dungeon.getWidth()))
		roll += 4;
	switch (index) {
	case 0:
	case 1:
	case 2:
		roll += gParty.getChar(index)->getCon();
		name = gParty.getChar(index)->getName();
		if (roll > DC) {
			gParty.getChar(index)->setCon(gParty.getChar(index)->getCon() + 1);
			msg_queue.push("The " + name + " resists the poisen of the trial! They feel their fortitude strengthen!");
		}
		else {
			gParty.getChar(index)->setCon(gParty.getChar(index)->getCon() - 1);
			msg_queue.push("The " + name + " feels quesy and probably should see a doctor. They don't feel as resiliant anymore.");
		}
		break;
	case 3:
		msg_queue.push("The party leaves the poisen on the table. As one should.");
		break;
	}
}
void intHandler(int index) {
	state = DUNGEON;
	msg_queue.pop();
	int DC = 15;
	std::string name;
	std::chrono::milliseconds ms = std::chrono::duration_cast< std::chrono::milliseconds >(
		std::chrono::system_clock::now().time_since_epoch()
		);
	srand(ms.count());//random seed
	int roll = (rand() % 20) + 1;
	if (current_dungeon.getScouted(gParty.getX() + gParty.getY()*current_dungeon.getWidth()))
		roll += 4;
	switch (index) {
	case 0:
	case 1:
	case 2:
		roll += gParty.getChar(index)->getInt();
		name = gParty.getChar(index)->getName();
		if (roll > DC) {
			gParty.getChar(index)->setInt(gParty.getChar(index)->getInt() + 1);
			msg_queue.push("After a few attempts, the " + name + " completes the puzzle and feels much smarter!");
		}
		else {
			gParty.getChar(index)->setInt(gParty.getChar(index)->getInt() - 1);
			msg_queue.push("The " + name + " fails the puzzle to many times and now doesn't feel more smarter.");
		}
		break;
	case 3:
		msg_queue.push("The party leaves the puzzle unsolved.");
		break;
	}
}
void wisHandler(int index) {
	state = DUNGEON;
	msg_queue.pop();
	int DC = 15;
	std::string name;
	std::chrono::milliseconds ms = std::chrono::duration_cast< std::chrono::milliseconds >(
		std::chrono::system_clock::now().time_since_epoch()
		);
	srand(ms.count());//random seed
	int roll = (rand() % 20) + 1;
	if (current_dungeon.getScouted(gParty.getX() + gParty.getY()*current_dungeon.getWidth()))
		roll += 4;
	switch (index) {
	case 0:
	case 1:
	case 2:
		roll += gParty.getChar(index)->getWis();
		name = gParty.getChar(index)->getName();
		if (roll > DC) {
			gParty.getChar(index)->setWis(gParty.getChar(index)->getWis() + 1);
			msg_queue.push("Using their vast wisdom gained over the years, the " + name + " out smarts the test and grows wiser!");
		}
		else {
			gParty.getChar(index)->setWis(gParty.getChar(index)->getWis() - 1);
			msg_queue.push("Clearly the " + name + " was not meant for this. Their wisdom depreciates.");
		}
		break;
	case 3:
		msg_queue.push("The party leaves the test alone. Speeking of which, why is there a test here anyways?");
		break;
	}
}
void chaHandler(int index) {
	state = DUNGEON;
	msg_queue.pop();
	int DC = 15;
	std::string name;
	std::chrono::milliseconds ms = std::chrono::duration_cast< std::chrono::milliseconds >(
		std::chrono::system_clock::now().time_since_epoch()
		);
	srand(ms.count());//random seed
	int roll = (rand() % 20) + 1;
	if (current_dungeon.getScouted(gParty.getX() + gParty.getY()*current_dungeon.getWidth()))
		roll += 4;
	switch (index) {
	case 0:
	case 1:
	case 2:
		roll += gParty.getChar(index)->getCha();
		name = gParty.getChar(index)->getName();
		if (roll > DC) {
			gParty.getChar(index)->setCha(gParty.getChar(index)->getCha() + 1);
			msg_queue.push("The " + name + " manages to convince a statue of it's sentience. This boosts their self confidence.");
		}
		else {
			gParty.getChar(index)->setCha(gParty.getChar(index)->getCha() - 1);
			msg_queue.push("The " + name + " talked with a non-living statue. Their self confidence diminishes.");
		}
		break;
	case 3:
		msg_queue.push("The party leaves the statue to figure out it's sentience by itself.");
		break;
	}
}

void rest(int index) {
	if (gParty.checkRest()) {
		if (gParty.useRest()) {
			for (int i = 0; i < 3; i++)
				gParty.getChar(i)->heal(gParty.getChar(i)->getMaxHP() / 4);
			ab = REST;
		}
	}
}

void peek(int index) {
	if(gParty.checkPeek())
		ab = PEEK;
}

void scout(int index) {
	//reveal a 2x2 or 3x3 area of the map
	if(gParty.checkScout())
		ab = SCOUT;
}

void returnToTown(int index) {
	state = TOWN_QUEST_BOARD;
	//inc completed dungeons
	gParty.incCompleted();
	gParty.addGold(current_quests[quest_index].getGold());
	gParty.addXP(current_quests[quest_index].getXP());
	gParty.resetAbilities();
	gParty.resetHealth();
	//check if game is done
	if (gParty.getCompleted() >= END_GAME) {
		state = END;
	}
	quests.completeQuest(current_quests[quest_index].getTitle());
	quests.getQuests();
	//create a new quest if neccesary
	//setup all relevant things
	completed.loadFromRenderedText(std::to_string(gParty.getCompleted()), textColor, 200);
	goldmenu.loadFromRenderedText(std::to_string(gParty.getGold()), textColor, 200);
	xp0.loadFromRenderedText(std::to_string(gParty.getXP(0)), textColor, 200);
	xp1.loadFromRenderedText(std::to_string(gParty.getXP(1)), textColor, 200);
	xp2.loadFromRenderedText(std::to_string(gParty.getXP(2)), textColor, 200);

}
/* Used primarily for testing */
void emptyHandler(int index) {
	return;
}
