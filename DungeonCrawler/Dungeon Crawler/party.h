#ifndef PARTY_H
#define PARTY_H

#include "lib.h"
#include "LButton.h"
#include "Ability.h"

#define NUM_CHAR	5
#define MAX_STAT	20
#define MAX_CHAR	3
//all characters will be defined as if they are dnd lvl 2ish

class Unit {
public:
	/* maintain unit data */
	void setRMO(int i) { RMO = i; }
	int getRMO() { return RMO; }
	int getMoveLeft() { return move_left; }
	void setMoveLeft(int i) { move_left = i; }
	UnitType getType() { return type; }
	void resetTurn() { move_left = move; bAction = true; action = true; }

	int getStr() { return str; }
	int getDex() { return dex; }
	int getCon() { return con; }
	int getInt() { return intel; }
	int getWis() { return wis; }
	int getCha() { return cha; }
	int getMaxHP() { return max_health; }
	int getHP() { return health; }
	int getAC() { return AC; }
	int getMove() { return move; }
	std::string getName() { return name; }

	void useAction() { action = false; }
	bool getAction() { return action; }
	void useBAction() { bAction = false; }
	bool getBAction() { return bAction; }

	void heal(int heal){ health += heal; if (health > max_health) { health = max_health; } }
	void damage(int dmg) { health -= dmg; }

	/* getters */
	virtual SDL_Rect getIcon50() { return SDL_Rect(); }
	virtual std::vector<Ability*> getAbilities() { return std::vector<Ability*>(0); }
	virtual Ability* getAb(std::string name, AbilityType type) { return NULL; }


	/* for chars and enemies alike */
	virtual void moveTo(int index) { return; }
	virtual void attack(int index) { return; }

	/* mob only functions */
	virtual void callHandler() { return; }
	virtual void updateThreat() { return; }

protected:
	int RMO;
	UnitType type;
	int move_left;

	//universal stats
	std::string name;
	int str;
	int dex;
	int con;
	int intel;
	int wis;
	int cha;
	int move;
	int health;
	int max_health;
	int AC;

	//actions
	bool action;
	bool bAction;

};


class Character: public Unit{
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
	SDL_Rect getIcon100();
	void setIcon100(SDL_Rect sprite_);

	void loadAbility(std::string name);
	std::vector<Ability*> getAbilities();

	void resetMove() { move_left = move; }

	void setStr(int n);
	void setDex(int n);
	void setCon(int n);
	void setInt(int n);
	void setWis(int n);
	void setCha(int n);
	void setMaxHP(int n);
	int getTextIndex();
	void setTextIndex(int index);
	void render(int x, int y);
	void addXP(int xp_);
	void subXP(int xp_);
	Ability* getAb(std::string name, AbilityType type);

private:
	//stats and whatnot
	int xp;
	std::vector<Ability> actionList;
	std::vector<Ability> bActionList;
	std::vector<Ability> freeList;


	SDL_Rect sprite;
	SDL_Rect icon_100;
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
	Character* getChar(int index);
	bool isAdj(int x, int y);
	void incCompleted();
	int getGold();
	void addGold(int gold_);
	bool subGold(int gold_);
	int getXP(int index);
	int getLOS();
	void addXP(int xp_);
	bool subXP(int xp_);
	int getRest();
	int getPeek();
	int getScout();
	bool useScout();
	bool usePeek();
	bool useRest();
	bool checkPeek();
	bool checkScout();
	bool checkRest();
	void resetAbilities();
	void resetHealth();

private:
	int numChar;
	int completed;
	int gold;
	std::vector<Character> charList;
	int party_x, party_y;

	//upgrades
	int LOS;
	int peek;
	int peek_max;
	bool scoutUp;
	int scout;
	int scout_max;
	int rest;
	int rest_max;
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