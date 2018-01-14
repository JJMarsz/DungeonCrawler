#include "Quest.h"

std::vector<Quest> current_quests;

Quest::Quest(){

}

int Quest::getGold() { return gold_reward; }
int Quest::getXP() { return xp_reward; }
std::string Quest::getTitle() { return name; }
std::string Quest::getInfo() { return info; }
Difficulty Quest::getDiff() { return diff; }

Quest::Quest(std::string name_, std::string info_, int gold, int xp, Difficulty diff_) {
	name = name_;
	info = info_;
	gold_reward = gold;
	xp_reward = xp;
	diff = diff_;
}

void loadQuests() {
	questButtons[0].setHandler(questInfo);
	questButtons[1].setHandler(questInfo);
	questButtons[2].setHandler(questInfo);
	acceptrejectButtons[0].setHandler(questAccept);
	acceptrejectButtons[1].setHandler(questReject);
	//questlist like charlist to maintain all possible quests
	//load a random onne that meets specifications into a quest
	current_quests[0] = Quest("Quest 1", "Temp info", 10, 1, EASY);
	current_quests[1] = Quest("Quest 2", "Temp info", 100, 2, MEDIUM);
	current_quests[2] = Quest("Quest 3", "Temp info", 500, 3, HARD);
}


EncounterType Encounter::getType() { return type; }
void Encounter::setType(EncounterType type_) { type = type_; }