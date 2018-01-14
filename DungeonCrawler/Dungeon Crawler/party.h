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
	int getXP();
	std::string getInfo();
	void setInfo(std::string info_);

	SDL_Rect getBigClip();
	void setBigClip(SDL_Rect sprite_);
	SDL_Rect getIcon25();
	void setIcon25(SDL_Rect sprite_);
	SDL_Rect getIcon50();
	void setIcon50(SDL_Rect sprite_);

	int getStr();
	int getDex();
	int getCon();
	int getInt();
	int getWis();
	int getCha();
	int getMove();
	int getHP();
	int getAC();
	int getTextIndex();
	void setTextIndex(int index);
	void render(int x, int y);
	void addXP(int xp_);
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
	int xp;
	SDL_Rect sprite;
	SDL_Rect icon_50;
	SDL_Rect icon_25;

	std::string info; 
	int textIndex;
};

class Party {
public:
	Party();
	bool addChar(int index);
	bool remChar(std::string name);
	bool moveParty(int x, int y);
	int getCompleted();
	int getX();
	int getY();
	Character getChar(int index);
	bool isAdj(int x, int y);
	void incCompleted();
	int getGold();
	void addGold(int gold_);
	bool subGold(int gold_);
	int getXP(int index);
	void addXP(int xp_);
	bool subXP(int xp_);


private:
	int numChar;
	int completed;
	int gold;
	std::vector<Character> charList;
	int party_x, party_y;
};

/* contains available characters */
class CharList {
public:
	CharList();
	bool isAvailable(int index);
	void pickChar(int index);
	Character showChar(int index);
	void unshowChar();
	void unpickChar(int index);
	void getChars();
	int getIndex(std::string);
	void loadSprites();
	int size();
	Character getChar(int index);
private:
	std::vector<Character> charList;
	std::vector<bool> picked;
	std::vector<bool> showed;
};

extern Party gParty;

extern CharList chars;

extern std::vector<Character> displayList;

#endif 