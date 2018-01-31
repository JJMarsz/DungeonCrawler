#ifndef ABILITY_H
#define ABILITY_H

#include "LButton.h"

typedef void(*func2)(int);
#define NUM_ABILITIES	6

enum AbilityType {
	ACTION,
	BACTION,
	FREE
};

enum AbilityIcons {
	MOVEMENT,
	GREATAXE,
	LONGSWORD,
	MORNINGSTAR,
	DAGGER,
	BOW
};

//each individual ability
class Ability {
public:
	Ability();
	Ability(std::string name_, std::string info_, AbilityType type_, int cd, int l, int dice, int dmg, void(*button)(int index), void(*click)(int index), SDL_Rect* icon_);
	std::string getName() { return name; }
	std::string getInfo() { return info; }
	AbilityType getType() { return type; }
	int getLength() { return length; }
	void setLength(int l) { length = l; }
	func2 getButtonHandler() { return button_handler; }
	func2 getClickHandler() { return click_handler; }
	bool onCooldown() { return (count >= cooldown); }
	void use() { count = 0; }
	int rollSingleHit(int atk_mod, int dmg_mod, int target_AC);
	int rollSneakAttack(int atk_mod, int dmg_mod, int target_AC);
	SDL_Rect* getIcon() { return icon; }

private:
	//handler to put into a button
	void(*button_handler)(int index);
	//handler to be executed after successful mouse click
	void(*click_handler)(int index);

	SDL_Rect* icon;
	std::string name;
	std::string info;
	//what is needed to use ability
	AbilityType type;
	//cooldown per use
	int cooldown;
	//how many rounds have passed since last use
	int count;

	//ability specs
	int length;
	//used if ability does damage
	int dmg_dice;
	int num_dice;

	//for rogue sneak
	int sneak_dice;
};

void loadAbilityMap();

//maintains every ability used in game
extern std::unordered_map<std::string, Ability> abMap;
extern void(*click_handler)(int index);
extern std::vector<int> getPath(int end, int start);
//ability click handlers
void moveClick(int index);
void greatAxeClick(int index);
void longSwordClick(int index);
void morningStarClick(int index);
void daggerClick(int index);
void bowClick(int index);

#endif 
