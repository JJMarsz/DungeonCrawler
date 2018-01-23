#include "Quest.h"

std::vector<Quest> current_quests;
EncounterList encounters;
QuestList quests;
Quest::Quest(){

}

int Quest::getGold() { return gold_reward; }
int Quest::getXP() { return xp_reward; }
std::string Quest::getTitle() { return name; }
std::string Quest::getInfo() { return info; }
Difficulty Quest::getDiff() { return diff; }
func Quest::fetchTrap(TrapIndex index) { return trapEnc[index]; }
func Quest::fetchInfo(InfoIndex index) { return infoEnc[index]; }
func Quest::fetchLoot(LootIndex index) { return lootEnc[index]; }
func Quest::fetchChoice(ChoiceIndex index) { return choiceEnc[index]; }
void Quest::loadTrap(TrapIndex i) { trapEnc.push_back(encounters.getTrap(i));}
void Quest::loadInfo(InfoIndex i) { infoEnc.push_back(encounters.getInfo(i)); }
void Quest::loadLoot(LootIndex i) { lootEnc.push_back(encounters.getLoot(i)); }
void Quest::loadChoice(ChoiceIndex i) { choiceEnc.push_back(encounters.getChoice(i)); }

Quest::Quest(std::string name_, std::string info_, int gold, int xp, Difficulty diff_) {
	name = name_;
	info = info_;
	gold_reward = gold;
	xp_reward = xp;
	diff = diff_;
	trapEnc.resize(0);
	choiceEnc.resize(0);
	lootEnc.resize(0);
	infoEnc.resize(0);
	enemyEnc.resize(0);
}

void initQuests() {
	questButtons[0].setHandler(questInfo);
	questButtons[1].setHandler(questInfo);
	questButtons[2].setHandler(questInfo);
	acceptrejectButtons[0].setHandler(questAccept);
	acceptrejectButtons[1].setHandler(questReject);
}

QuestList::QuestList() {
	//put in all quests that need to be loaded
	// 0	kobolds
	// 1	skeletons
	// 2
	// 3
	// 4
	easyQuestIndex = 0;
	medQuestIndex = 6;
	hardQuestIndex = 9;
	list.resize(NUM_QUEST);
	list[0].q =
	{ "Mines of Dorimir",
	"The mines have become overrun with pesky kobolds. Would anyone take care of them for us?",
	100,
	1,
	EASY
	};
	list[0].complete = false;
	list[0].show = false;
	list[1].q =
	{ "Graveyard of the Forsaken",
		"A mass grave of war criminals has come back to life due to unforseen reasons. Someone return them to the grave.",
		100,
		1,
		EASY
	};
	list[1].complete = false;
	list[1].show = false;
	list[2].q =
	{ "Goblins and Garden Gnomes",
		"Goblins are kidnapping gnomes to use as decorations in their gardens! Save our smaller townsfolk by clearing out their lair!",
		100,
		1,
		EASY
	};
	list[2].complete = false;
	list[2].show = false;
	list[3].q =
	{ "Tyranno-WHAT?",
		"The town has gone jurassic! We need someone to clear out these dinosaurs!",
		100,
		1,
		EASY
	};
	list[3].complete = false;
	list[3].show = false;
	list[4].q =
	{ "Spooky, Scary, Skeletons",
		"These skeletons are sending shivers down our spines! Please someone bury them properly!",
		100,
		1,
		EASY
	};
	list[4].complete = false;
	list[4].show = false;
	list[5].q =
	{ "Beasts of the Wild",
		"Something is controlling beasts to ravage the town's lands. Putting a stop to this can help the townsfolk survive.",
		100,
		1,
		EASY
	};
	list[5].complete = false;
	list[5].show = false;
	list[medQuestIndex].q =
	{ "Cult of Roshan",
		"There is a cult that is converting civilians to do their evil bidding. Is there someone that can stop this?",
		250,
		2,
		MEDIUM
	};
	list[medQuestIndex].complete = false;
	list[medQuestIndex].show = false;
	list[medQuestIndex+1].q =
	{ "Barbarian King of Kor",
		"A hairy and unseemly group has claimed our farmland. Can someone send them on their way?",
		250,
		2,
		MEDIUM
	};
	list[medQuestIndex+1].complete = false;
	list[medQuestIndex+1].show = false;
	list[medQuestIndex + 2].q =
	{ "Jeph, the Wrestler",
		"Jeph and his wrestlers are killing the townsfolk with their long range beams! Help us heros!",
		250,
		2,
		MEDIUM
	};
	list[medQuestIndex + 2].complete = false;
	list[medQuestIndex + 2].show = false;
	list[hardQuestIndex].q =
	{ "Cult of the Dragon Queen",
		"The Dragon Queen Liktin has claimed our town as her domain. Her tyrannical rule will destroy this town. Help us someone!",
		600,
		3,
		HARD
	};
	list[hardQuestIndex].complete = false;
	list[hardQuestIndex].show = false;
	list[hardQuestIndex+1].q =
	{ "The Beholder Xanathar",
		"The sacred town sword was stolen by Xanathar! Reclaim our sword for a worthwhile compensation!",
		600,
		3,
		HARD
	};
	list[hardQuestIndex+1].complete = false;
	list[hardQuestIndex+1].show = false;
	for (int i = 0; i < NUM_QUEST; i++) {
		list[i].q.loadTrap(BASIC);

		list[i].q.loadInfo(AREA_REVEAL);
		list[i].q.loadInfo(BOSS_REVEAL);
		list[i].q.loadInfo(MOB_REVEAL);
		list[i].q.loadInfo(LOOT_REVEAL);
		list[i].q.loadInfo(CHOICE_REVEAL);
		list[i].q.loadInfo(TRAP_REVEAL);

		list[i].q.loadLoot(GOLD);

		list[i].q.loadChoice(WELL);
		list[i].q.loadChoice(OBELISK);
	}
}

bool QuestList::isAvailable(int index) {
	if (list[index].complete || list[index].show)
		return false;
	return true;
}

Quest QuestList::showQuest(int index) {
	list[index].show = true;
	return list[index].q;
}

void QuestList::completeQuest(std::string name) {
	int index;
	for (int i = 0; i < NUM_QUEST; i++) {
		if (list[i].q.getTitle() == name) {
			index = i;
			break;
		}
	}
	list[index].complete = true;
}

void QuestList::unShowQuest() {
	for (int i = 0; i < list.size(); i++)
		list[i].show = false;
}

void QuestList::getQuests() {
	current_quests = std::vector<Quest>();
	int index, i;
	int easyChance, medChance, hardChance, finalChance = 0;
	srand(time(NULL));
	switch (gParty.getCompleted()) {
	case 0:
		easyChance = 100;
		medChance = 0;
		hardChance = 0;
		break;
	case 1:
		easyChance = 80;
		medChance = 20;
		hardChance = 0;
		break;
	case 2:
		easyChance = 60;
		medChance = 40;
		hardChance = 0;
		break;
	case 3:
		easyChance = 40;
		medChance = 60;
		hardChance = 0;
		break;
	case 4:
		easyChance = 25;
		medChance = 70;
		hardChance = 5;
		break;
	case 5:
		easyChance = 5;
		medChance = 80;
		hardChance = 15;
		break;
	case 6:
		easyChance = 0;
		medChance = 70;
		hardChance = 30;
		break;
	case 7:
		easyChance = 0;
		medChance = 60;
		hardChance = 40;
		break;
	case 8:
		easyChance = 0;
		medChance = 40;
		hardChance = 60;
		break;
	case 9:
		easyChance = 0;
		medChance = 20;
		hardChance = 80;
		break;
	case 10:
		easyChance = 0;
		medChance = 0;
		hardChance = 100;
		break;
	case 11:
		//maybe do final dungeon?
		easyChance = 0;
		medChance = 0;
		hardChance = 0;
		finalChance = 100;
		break;
	default:
		break;
	}

	for (i = 0; i < MAX_QUESTS; i++) {
		//choose the difficulty of this quest
		index = (rand() % 100) + 1;
		if (index <= easyChance)
			getEasyQuest();
		else if (index <= easyChance + medChance)
			getMedQuest();
		else if (index <= easyChance + medChance + hardChance)
			getHardQuest();
		
	}

}

void QuestList::getEasyQuest() {
	//0 to 1 below med quests
	int index;
	while (1) {
		index = rand() % medQuestIndex;
		if (isAvailable(index))
			break;
	}
	current_quests.push_back(showQuest(index));
}

void QuestList::getMedQuest() {
	//medQuestindex to 1 below hard quests
	int index;
	while (1) {
		index = (rand() % (hardQuestIndex - medQuestIndex)) + medQuestIndex;
		if (isAvailable(index))
			break;
	}
	current_quests.push_back(showQuest(index));
}

void QuestList::getHardQuest() {
	//hardQuestindex to end
	int index;
	while (1) {
		index = (rand() % (list.size() - hardQuestIndex)) + hardQuestIndex;
		if (isAvailable(index))
			break;
	}
	current_quests.push_back(showQuest(index));
}
