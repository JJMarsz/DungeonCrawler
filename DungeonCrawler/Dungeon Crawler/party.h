#ifndef PARTY_H
#define PARTY_H

#include <vector>
#include <string>
#include <unordered_map>

#define MAX_STAT	20;


class Character {
public:
	Character(std::string name);
	std::string getName();
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
	//items and whatnot

};

class Party {
public:
	bool addChar(Character newChar);
	//bool remChar(std::string name);
	bool moveParty(int x, int y);

private:
	int numChar;
	std::vector<Character> charList;
	int party_x, party_y;
};

std::unordered_map<std::string, Character> baseChars;
#endif /* party.h */