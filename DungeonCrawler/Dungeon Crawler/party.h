#ifndef PARTY_H
#define PARTY_H

#include "lib.h"
#include "LTexture.h"

#define NUM_CHAR	5
#define MAX_STAT	20
#define MAX_CHAR	3
//all characters will be defined as if they are dnd lvl 2ish


class Character {
public:
	Character();
	Character(
		std::string name_,
		int str_,
		int dex_,
		int con_,
		int intel_,
		int wis_,
		int cha_,
		int mov_,
		int health_,
		int AC_
	
	);
	std::string getName();

	std::string getInfo();
	void setInfo(std::string info_);

	SDL_Rect getClip();
	void setClip(SDL_Rect sprite_);

	int getStr();
	int getDex();
	int getCon();
	int getInt();
	int getWis();
	int getCha();
	int getMove();
	int getHP();
	int getAC();

	void render(int x, int y);
	//Hit(int atk_roll, int dmg_roll);
	//Attack()


private:
	std::string name;
	//stats and whatnot
	int str;
	int dex;
	int con;
	int intel;
	int wis;
	int cha;
	int move;
	int health;
	int AC;
	//somehow maintain lvlups
	//items and whatnot
	SDL_Rect sprite;
	std::string info;
};

class Party {
public:
	Party();
	bool addChar(Character newChar);
	bool remChar(std::string name);
	bool moveParty(int x, int y);

private:
	int numChar;
	std::vector<Character> charList;
	int party_x, party_y;
};

/* contains available characters */
class CharList {
public:
	CharList();
	CharList(int magic);
	bool isAvailable(int index);
	void pickChar(int index);
	Character showChar(int index);
	void unshowChar();
	void unpickChar(int index);
	void getChars();
	int getIndex(std::string);
private:
	std::vector<Character> charList;
	std::vector<bool> picked;
	std::vector<bool> showed;
};

extern Party gParty;

extern CharList chars;

extern std::vector<Character> displayList;

#endif 