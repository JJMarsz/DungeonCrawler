#include "party.h"
#include "LTexture.h"


Party gParty;

CharList chars;

std::vector<Character> displayList;

/* Char clas definitions */
Character::Character(std::string name_, int str_, int dex_, int con_, int intel_, 
						      int wis_, int cha_, int mov_, int health_, int AC_) {
	name = name_;
	str = str_;
	dex = dex_;
	con = con_;
	intel = intel_;
	wis = wis_;
	cha = cha_;
	move = mov_;
	health = health_;
	AC = AC_;
	xp = 0;
}

Character::Character(){}

void Character::setInfo(std::string info_) { info = info_; }
std::string Character::getInfo() { return info; }

SDL_Rect Character::getBigClip() { return sprite; }
void Character::setBigClip(SDL_Rect sprite_) { sprite = sprite_; }
SDL_Rect Character::getIcon25() { return icon_25; }
void Character::setIcon25(SDL_Rect sprite_) { icon_25 = sprite_; }
SDL_Rect Character::getIcon50() { return icon_50; }
void Character::setIcon50(SDL_Rect sprite_) { icon_50 = sprite_; }

int Character::getXP() { return xp; }
int Character::getStr() { return str; }
int Character::getDex() { return dex; }
int Character::getCon() { return con; }
int Character::getInt() { return intel; }
int Character::getWis() { return wis; }
int Character::getCha() { return cha; }
int Character::getMove() { return move; }
int Character::getHP() { return health; }
int Character::getAC() { return AC; }
std::string Character::getName() { return name; }

void Character::setTextIndex(int index) { textIndex = index; }
int Character::getTextIndex() { return textIndex; }

//used to draw character info during character selection state
void Character::render(int x, int y) {
	//draw sprite
	charSST.render(x, y, &sprite);
	texts[textIndex]->render(x + 10, y + 200);



}

void Character::addXP(int xp_) { xp += xp_; }
/* Party calss definitions */
Party::Party() {
	numChar = 0;
	party_x = -1;
	party_y = -1;
	charList.resize(0);
	gold = 0;
	LOS = 1;
}

int Party::getCompleted() { return completed; }
int Party::getX() { return party_x; }
int Party::getY() { return party_y; }
Character Party::getChar(int index) { return charList[index]; }
int Party::getGold() { return gold; }
int Party::getXP(int index) { return charList[index].getXP(); }
int Party::getLOS() { return LOS; }

void Party::incCompleted() { completed++; }
bool Party::addChar(int index) {
	if (numChar < 3)
		charList.push_back(chars.getChar(index));
	else
		return false;
	numChar++;
	return true;
}
 
bool Party::remChar(std::string nameKill) {
	for (int i = 0; i < 3; i++) {
		if (charList[i].getName() == nameKill) {


			numChar--;
			return true;
		}
	}
	return false;
}

bool Party::moveParty(int x, int y) {
	party_x = x;
	party_y = y;
	return true;
}

/* Check all directions for adjacency and if tile is a valid type */
bool Party::isAdj(int x, int y) {
	if (x < 0 || y < 0 || x >= current_dungeon.getWidth() || y >= current_dungeon.getHeight())
		return false;
	int RMO = x + y * current_dungeon.getWidth();
	if (current_dungeon.getTile(RMO).getType() == NONE)
		return false;
	if (current_dungeon.getTile(RMO).getType() == BARRIER)
		return false;
	if (party_x + 1 + current_dungeon.getWidth()*party_y == RMO)
		return true;
	if (party_x - 1 + current_dungeon.getWidth()*party_y == RMO)
		return true;
	if (party_x + current_dungeon.getWidth()*(party_y + 1) == RMO)
		return true;
	if (party_x + current_dungeon.getWidth()*(party_y - 1) == RMO)
		return true;
	return false;
}

/* post dungeon update functions */
void Party::addGold(int gold_) { gold += gold_; }
bool Party::subGold(int gold_){ 
	if (gold_ > gold)
		return false;
	gold -= gold_;
	return true;
}
void Party::addXP(int xp_) {
	for (int i = 0; i < MAX_CHAR; i++) {
		charList[i].addXP(xp_);
	}
}

/* Charlist class definiitons */
CharList::CharList() {
	//Figther		0
	//Barb			1
	//Paladin		2
	//Rogue 		3
	//Ranger 		4
	charList = {
		{ "Fighter",
		3,	//str	
		3,	//dex
		2,	//con
		0,	//int
		1,	//wis
		0,	//cha
		6,	//movement
		20,	//Health
		15	//AC
		},
	{ "Barbarian",
		4,	//str	
		2,	//dex
		3,	//con
		-1,	//int
		0,	//wis
		-1,	//cha
		6,	//movement
		25,	//Health
		14	//AC
	},
	{ "Paladin",
		3,	//str	
		0,	//dex
		1,	//con
		1,	//int
		3,	//wis
		0,	//cha
		6,	//movement
		18,	//Health
		16	//AC
	},
	{ "Rogue",
		-1,	//str	
		3,	//dex
		0,	//con
		2,	//int
		2,	//wis
		1,	//cha
		6,	//movement
		13,	//Health
		14	//AC
	},
	{ "Ranger",
		-1,	//str	
		3,	//dex
		1,	//con
		1,	//int
		3,	//wis
		0,	//cha
		7,	//movement
		18,	//Health
		14	//AC
	}
	};
}
void CharList::loadSprites() {
	//Go through each character to set sprites and info texts
	picked.resize(NUM_CHAR);
	showed.resize(NUM_CHAR);
	charList.resize(NUM_CHAR);
	//Figther
	picked[0] = false;
	showed[0] = false;
	charList[0].setBigClip(charClips[0]);
	charList[0].setIcon50(charClips[NUM_CHAR]);
	charList[0].setIcon25(charClips[NUM_CHAR*2]);
	charList[0].setInfo(
		"Temp text for now, Figther."
	);

	//Barb
	picked[1] = false;
	showed[1] = false;
	charList[1].setBigClip(charClips[1]);
	charList[1].setIcon50(charClips[1 + NUM_CHAR]);
	charList[1].setIcon25(charClips[1 + NUM_CHAR * 2]);
	charList[1].setInfo(
		"Temp text for now, Barbarian."
	);

	//Paladin
	picked[2] = false;
	showed[2] = false;
	charList[2].setBigClip(charClips[2]);
	charList[2].setIcon50(charClips[2 + NUM_CHAR]);
	charList[2].setIcon25(charClips[2 + NUM_CHAR * 2]);
	charList[2].setInfo(
		"Temp text for now, Paladin."
	);

	//Rogue
	picked[3] = false;
	showed[3] = false;
	charList[3].setBigClip(charClips[3]);
	charList[3].setIcon50(charClips[3 + NUM_CHAR]);
	charList[3].setIcon25(charClips[3 + NUM_CHAR * 2]);
	charList[3].setInfo(
		"Temp text for now, Rogue."
	);

	//Ranger
	picked[4] = false;
	showed[4] = false;
	charList[4].setBigClip(charClips[4]);
	charList[4].setIcon50(charClips[4 + NUM_CHAR]);
	charList[4].setIcon25(charClips[4 + NUM_CHAR * 2]);
	charList[4].setInfo(
		"Temp text for now, Ranger."
	);
	//generate text and whatnot
	int count = (CHAR_BUTTON_WIDTH - 20) / 2;
	int i;
	std::string displayText = "";
	int w;
	for (i = 0; i < NUM_CHAR; i++) {
		const char* cstr = charList[i].getName().c_str();
		TTF_SizeText(gFont, cstr, &w, NULL);
		displayText = "";
		count -= (w / 2);
		for (int i = 0; i < count; i++) {
			displayText += " ";
		}
		std::stringstream buf;
		buf << charList[i].getName() << "\n" << std::endl;
		buf << "Strength: " << charList[i].getStr() << "\n";
		buf << "Dexterity: " << charList[i].getDex() << "\n";
		buf << "Constitution: " << charList[i].getCon() << "\n";
		buf << "Intelligence: " << charList[i].getInt() << "\n";
		buf << "Wisdom: " << charList[i].getWis() << "\n";
		buf << "Charisma: " << charList[i].getCha() << "\n";
		buf << "Speed: " << charList[i].getMove() << "\n";
		buf << "Health: " << charList[i].getHP() << "\n";
		buf << "AC: " << charList[i].getAC() << "\n";
		buf << "Info: " << charList[i].getInfo() << "\n";

		//store index into text texture vector
		charList[i].setTextIndex(loadText(CHAR_BUTTON_WIDTH, buf.str()));
	}
}
/* Check to see if a character is available to be displayed */
bool CharList::isAvailable(int index) { return (picked[index] || showed[index]); }
/* Label character as picked */
void CharList::pickChar(int index) { picked[index] = true; }
/* Return the selected char to store locally */
Character CharList::showChar(int index) { showed[index] = true;  return charList[index]; }
/* UNshow all characters */
void CharList::unshowChar() { 
	for (int i = 0; i < NUM_CHAR; i++) {
		showed[i] = false;
	}
}
/* Unpick a character */
void CharList::unpickChar(int index) { picked[index] = false; }

Character CharList::getChar(int index) { return charList[index]; }

int CharList::size() { return charList.size(); }

void CharList::getChars() {
	//pick 3 characters
	displayList = std::vector<Character>();
	int index, i;
	for (i = 0; i < MAX_CHAR; i++) {
		while (1) {
			index = rand() % NUM_CHAR;
			if (!chars.isAvailable(index))
				break;
		}
		displayList.push_back(chars.showChar(index));
	}
}

int CharList::getIndex(std::string name) {
	int i;
	for (i = 0; i < NUM_CHAR; i++) {
		if (charList[i].getName() == name)
			return i;
	}
	return -1;
}