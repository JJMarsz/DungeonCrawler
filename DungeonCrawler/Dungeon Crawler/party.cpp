#include "party.h"

bool Party::addChar(Character newChar) {
	if (numChar < 3)
		charList.push_back(newChar);
	else
		return false;
	numChar++;
	return true;
}
 /*
bool Party::remChar(std::string nameKill) {
	for (int i = 0; i < 3; i++) {
		if (charList[i].getName() == nameKill) {


			numChar--;
			return true;
		}
	}
	return false;
}
*/

bool Party::moveParty(int x, int y) {
	party_x = x;
	party_y = y;
	return true;
}

std::string Character::getName() { return name;}