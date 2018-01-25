#ifndef ABILITY_H
#define ABILITY_H

#include "lib.h"

typedef void(*func2)(int);
#define NUM_ABILITIES	1

enum AbilityType {
	ACTION,
	BACTION,
	FREE
};

//each individual ability
class Ability {
public:
	Ability();
	Ability(std::string name_, std::string info_, AbilityType type_, int cd, int l, int dice, int dmg, void(*button)(int index), void(*click)(int index));
	std::string getName() { return name; }
	std::string getInfo() { return info; }
	AbilityType getType() { return type; }
	int getLength() { return length; }
	void setLength(int l) { length = l; }
	func2 getButtonHandler() { return button_handler; }
	bool onCooldown() { return (count >= cooldown); }
	void use() { count = 0; }


private:
	//handler to put into a button
	void(*button_handler)(int index);
	//handler to be executed after successful mouse click
	void(*click_handler)(int index);

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
};

void loadAbilityMap();

//maintains every ability used in game
extern std::unordered_map<std::string, Ability> abMap;

#endif 
