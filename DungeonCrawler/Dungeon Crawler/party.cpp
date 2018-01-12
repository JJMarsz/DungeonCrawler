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
}

Character::Character(){}

void Character::setInfo(std::string info_) { info = info_; }
std::string Character::getInfo() { return info; }

SDL_Rect Character::getClip() { return sprite; }
void Character::setClip(SDL_Rect sprite_) { sprite = sprite_; }

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

/* Party calss definitions */
Party::Party() {
	numChar = 0;
	party_x = -1;
	party_y = -1;
	charList.resize(0);
}

int Party::getCompleted() { return completed; }

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


/* Charlist class definiitons */
CharList::CharList() {
	//quell compiler bitching
}
CharList::CharList(int magic) {
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
	charList[0].setClip(charClips[0]);
	charList[0].setInfo(
		"Temp text for now, Figther."
	);

	//Barb
	picked[1] = false;
	showed[1] = false;
	charList[1].setClip(charClips[1]);
	charList[1].setInfo(
		"Temp text for now, Barbarian."
	);

	//Paladin
	picked[2] = false;
	showed[2] = false;
	charList[2].setClip(charClips[2]);
	charList[2].setInfo(
		"Temp text for now, Paladin."
	);

	//Rogue
	picked[3] = false;
	showed[3] = false;
	charList[3].setClip(charClips[3]);
	charList[3].setInfo(
		"Temp text for now, Rogue."
	);

	//Ranger
	picked[4] = false;
	showed[4] = false;
	charList[4].setClip(charClips[4]);
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