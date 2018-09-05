#include "Ability.h"
#include "Room.h"

std::unordered_map<std::string, Ability> abMap;

void loadAbilityMap(){
	//universal abilities
	abMap["Move"] = Ability("Move", "Move the character", FREE, 1, 6, 0, 0, moveButton, moveClick, &abClips[MOVEMENT]);

	//character abilities
	abMap["Greataxe"] = Ability("Greataxe", "Use and action to attack with a greataxe", ACTION, 1, 1, 1, 8, greatAxeButton, greatAxeClick, &abClips[GREATAXE]);
	abMap["Longsword"] = Ability("Longsword", "Use and action to attack with a longsword", ACTION, 1, 1, 1, 8, longSwordButton, longSwordClick, &abClips[LONGSWORD]);
	abMap["Morningstar"] = Ability("Morningstar", "Use and action to attack with a morningstar", ACTION, 1, 1, 1, 6, morningStarButton, morningStarClick, &abClips[MORNINGSTAR]);
	abMap["Dagger"] = Ability("Dagger", "Use and action to attack with a dagger, with sneak attack", ACTION, 1, 1, 1, 4, daggerButton, daggerClick, &abClips[DAGGER]);
	abMap["Bow"] = Ability("Bow", "Use and action to shoot an arrow", ACTION, 1, 6, 1, 8, bowButton, bowClick, &abClips[BOW]);
}

//generate paths
void bloom(std::vector<int> &prevVec, int start, int end) {
	int width = room->getWidth();
	std::queue<int> prevQ;
	prevQ.push(end);
	while(!prevQ.empty()) {
		int x = prevQ.front() % width;
		int y = prevQ.front() / width;
		if (x == start%width && y == start/width)
			return;
		prevQ.pop();
		if (room->getTile(x, y)->color == NORMAL)
			continue;
		//add all adjacent non in prevvec tiles
		if (x > 0) {
			if (prevVec[x - 1 + y * width] == -1) {
				prevVec[x - 1 + y * width] = x + y * width;
				prevQ.push(x - 1 + y * width);
			}
		}
		if (y > 0) {
			if (prevVec[x + (y-1) * width] == -1) {
				prevVec[x + (y-1) * width] = x + y * width;
				prevQ.push(x + (y-1) * width);
			}
		}
		if (x < room->getWidth() - 1) {
			if (prevVec[x + 1 + y * width] == -1) {
				prevVec[x + 1 + y * width] = x + y * width;
				prevQ.push(x + 1 + y * width);
			}
		}
		if (y < room->getHeight() - 1) {
			if (prevVec[x + (y + 1) * width] == -1) {
				prevVec[x + (y + 1) * width] = x + y * width;
				prevQ.push(x + (y + 1) * width);
			}
		}
	}
}

std::vector<int> getPath(int end, int start) {
	std::vector<int> retvec;
	int width = room->getWidth();
	int curr_x = start % width;
	int curr_y = start / width;
	int next_x = end % width;
	int next_y = end / width;
	int RMO = start;
	int distance = std::abs(curr_x - next_x) + std::abs(curr_y - next_y);
	/*std::vector<bool> visited(width*room->getHeight(), false);
	pathHelper(curr_x, curr_y, next_x, next_y, distance, retvec, visited);*/
	std::vector<int> prevvec(width*room->getHeight(), -1);
	bloom(prevvec, start, end);
	while (RMO != end) {
		retvec.push_back(RMO);
		RMO = prevvec[RMO];
	}
	retvec.push_back(RMO);
	return retvec;
}

Ability::Ability(std::string name_, std::string info_, AbilityType type_, int cd, int l, int dice, int dmg, void(*button)(int index), void(*click)(int index), SDL_Rect* icon_) {
	name = name_;
	info = info_;
	type = type_;
	cooldown = cd;
	length = l;
	num_dice = dice;
	dmg_dice = dmg;
	button_handler = button;
	click_handler = click;
	icon = icon_;
}

int Ability::rollSingleHit(int atk_mod, int dmg_mod, int target_AC) {
	//returns -1 on miss and dmg on hit
	std::chrono::milliseconds ms = std::chrono::duration_cast< std::chrono::milliseconds >(
		std::chrono::system_clock::now().time_since_epoch()
		);
	srand(ms.count());//random seed
	int i;
	int roll = 1 + rand() % 20;
	if (roll + atk_mod + gParty->getProf() <= target_AC) {
		messageBox.loadFromRenderedText("The " + room->getCurrUnit()->getName() + " missed the attack!", { 255, 255, 255 }, 650);
		return -1;
	}
	else if (roll == 20) {
		messageBox.loadFromRenderedText("A critical hit!", { 255, 255, 255 }, 650);
		messageBox.render((650 - messageBox.getWidth()) / 2, 614);
		SDL_RenderPresent(gRenderer);
		Sleep(800);
		roll = dmg_mod + num_dice*dmg_dice;
	}
	else
		roll = dmg_mod;
	for (i = 0; i < num_dice; i++)
		roll += 1 + rand() % dmg_dice;
	drawRoom();
	messageBox.loadFromRenderedText("The " + room->getCurrUnit()->getName() + " landed a hit and dealt " + std::to_string(roll) + " damage!", { 255, 255, 255 }, 650);
	return roll;
}

//click handlers for abilities
int Ability::rollSneakAttack(int atk_mod, int dmg_mod, int s_dice, int target_AC) {
	//returns -1 on miss and dmg on hit
	std::chrono::milliseconds ms = std::chrono::duration_cast< std::chrono::milliseconds >(
		std::chrono::system_clock::now().time_since_epoch()
		);
	srand(ms.count());//random seed
	int i;
	int roll = 1 + rand() % 20;
	if (roll + atk_mod + gParty->getProf() <= target_AC) {
		messageBox.loadFromRenderedText("The " + room->getCurrUnit()->getName() + " missed the sneak attack!", { 255, 255, 255 }, 650);
		return -1;
	}
	else if (roll == 20) {
		messageBox.loadFromRenderedText("A critical hit!", { 255, 255, 255 }, 650);
		messageBox.render((650 - messageBox.getWidth()) / 2, 614);
		SDL_RenderPresent(gRenderer);
		Sleep(800);
		roll = dmg_mod + num_dice*dmg_dice + 6*s_dice;
	}
	else
		roll = dmg_mod;
	for (i = 0; i < num_dice; i++)
		roll += 1 + rand() % dmg_dice;
	for (i = 0; i < s_dice; i++)
		roll += 1 + rand() % 6;
	drawRoom();
	messageBox.loadFromRenderedText("The " + room->getCurrUnit()->getName() + " landed a sneak attack and dealt " + std::to_string(roll) + " damage!", { 255, 255, 255 }, 650);
	return roll;
}

Unit* getTarget(int index) {
	std::vector<Unit*>* init = room->getInititiveOrder();
	//get the target
	for (int i = 0; i < init->size(); i++) {
		if (index == init->at(i)->getRMO()) {
			return init->at(i);
		}
	}
	return NULL;
}

int getAdjAllies(int l) {
	Unit* curr = room->getCurrUnit();
	int width = room->getWidth();
	std::queue<int> q;
	std::unordered_map<int, int> distmap;
	distmap[curr->getRMO()] = l;
	q.push(curr->getRMO());
	int char_count = 0;
	while (!q.empty()) {
		int x = q.front() % width;
		int y = q.front() / width;
		q.pop();
		if (distmap[x + y * width] <= 0)
			continue;
		if (x > 0) {
			if (room->getTile(x - 1, y)->type == CHARACTER)
				char_count++;
			distmap[(x - 1) + y * width] = distmap[(x)+y * width] - 1;
			q.push((x - 1) + y * width);
		}
		if (y > 0) {
			if (room->getTile(x, y - 1)->type == CHARACTER)
				char_count++;
			distmap[(x) + (y-1) * width] = distmap[(x)+y * width] - 1;
			q.push((x) + (y-1) * width);
		}
		if (x < room->getWidth() - 1) {
			if (room->getTile(x + 1, y)->type == CHARACTER)
				char_count++;
			distmap[(x + 1) + y * width] = distmap[(x)+y * width] - 1;
			q.push((x + 1) + y * width);
		}
		if (y < room->getHeight() - 1) {
			if (room->getTile(x, y + 1)->type == CHARACTER)
				char_count++;
			distmap[(x) + (y+1) * width] = distmap[(x)+y * width] - 1;
			q.push((x) + (y+1) * width);
		}
	}
	return char_count;
}

void moveClick(int index) {
	int i;
	int width = room->getWidth();
	Unit* curr = room->getCurrUnit();
	int curr_x = curr->getRMO() % width;
	int curr_y = curr->getRMO() / width;
	int next_x = index % width;
	int next_y = index / width;
	if (room->getTile(next_x, next_y)->color == BLUE) {
		curr->useAction();
		curr->setMoveLeft(curr->getMoveLeft() + curr->getMove());
	}
	std::vector<int> RMO_list = getPath(index, curr->getRMO());
	curr->setMoveLeft(curr->getMoveLeft() - (RMO_list.size()-1));
	room->clearRange();
	room->getTile(curr->getRMO() % width, curr->getRMO() / width)->type = NOTHING;
	for (i = 0; i < RMO_list.size(); i++) {
		curr->setRMO(RMO_list[i]);
		room->getTile(RMO_list[i] % width, RMO_list[i] / width)->type = CHARACTER;
		drawRoom();
		room->getTile(RMO_list[i] % width, RMO_list[i] / width)->type = NOTHING;
		SDL_RenderPresent(gRenderer);
		Sleep(200);
	}
	room->getTile(curr->getRMO() % width, curr->getRMO() / width)->type = CHARACTER;
	if (curr->getMoveLeft() > 0 || curr->getAction())
		moveButton(0);
	else
		gameData.ab = NOPE;
}

void greatAxeClick(int index) {
	Unit* target = getTarget(index);
	if (target == NULL)
		return;
	Unit* curr = room->getCurrUnit();
	//custom barb passive, less hp, bonus to atk and dmg
	int bonus = 0;
	int HP_diff = curr->getMaxHP() - curr->getHP();
	int slice = curr->getMaxHP() / curr->getOffset();
	for (int i = 1; i < curr->getOffset(); i++) {
		if (HP_diff >= slice * i) {
			bonus++;
		}
	}
	int dmg = curr->getAb("Greataxe", ACTION)->rollSingleHit(curr->getStr() + bonus, curr->getStr() + bonus, target->getAC());
	messageBox.render((650 - messageBox.getWidth()) / 2, 614);
	SDL_RenderPresent(gRenderer);
	if (-1 != dmg) {
		target->damage(dmg);
	}
	Sleep(1000);
	room->getCurrUnit()->useAction();
	room->clearRange();
	//click_handler = NULL;
}

void longSwordClick(int index) {
	Unit* target = getTarget(index);
	if (target == NULL)
		return;
	Unit* curr = room->getCurrUnit();
	int bonus = 0;
	//vustom fighter passive, bonust atk and dmg the longer the battle takes
	for (int i = 0; i < room->getTurn()/curr->getOffset(); i++) {
		bonus++;
	}
	int dmg = curr->getAb("Longsword", ACTION)->rollSingleHit(curr->getStr() + bonus, curr->getStr() + bonus, target->getAC());
	messageBox.render((650 - messageBox.getWidth()) / 2, 614);
	SDL_RenderPresent(gRenderer);
	if (-1 != dmg) {
		target->damage(dmg);
	}
	Sleep(1000);
	room->getCurrUnit()->useAction();
	room->clearRange();
	//click_handler = NULL;
}

void morningStarClick(int index) {
	Unit* target = getTarget(index);
	if (target == NULL)
		return;
	Unit* curr = room->getCurrUnit();
	int bonus = getAdjAllies(curr->getOffset());
	int dmg = curr->getAb("Morningstar", ACTION)->rollSingleHit(curr->getStr() + bonus, curr->getStr() + bonus, target->getAC());
	messageBox.render((650 - messageBox.getWidth()) / 2, 614);
	SDL_RenderPresent(gRenderer);
	if (-1 != dmg) {
		target->damage(dmg);
	}
	Sleep(1000);
	room->getCurrUnit()->useAction();
	room->clearRange();
	//click_handler = NULL;
}
void daggerClick(int index) {
	Unit* target = getTarget(index);
	if (target == NULL)
		return;
	Unit* curr = room->getCurrUnit();
	int dmg;
	if (room->getTile(target->getRMO() % room->getWidth(), target->getRMO() / room->getWidth())->color == ORANGE) {
		dmg = curr->getAb("Dagger", ACTION)->rollSneakAttack(curr->getDex(), curr->getDex(), curr->getOffset(), target->getAC());
	}
	else
		dmg = curr->getAb("Dagger", ACTION)->rollSingleHit(curr->getDex(), curr->getDex(), target->getAC());
	messageBox.render((650 - messageBox.getWidth()) / 2, 614);
	SDL_RenderPresent(gRenderer);
	if (-1 != dmg) {
		target->damage(dmg);
	}
	Sleep(1000);
	room->getCurrUnit()->useAction();
	room->clearRange();
	//click_handler = NULL;
}

void bowClick(int index) {
	Unit* target = getTarget(index);
	if (target == NULL)
		return;
	Unit* curr = room->getCurrUnit();
	int width = room->getWidth();
	//custom ranger ability
	//add offset and distance away to atk mod
	int atk_mod = curr->getDex() + curr->getOffset() + std::abs(curr->getRMO() % width - target->getRMO() % width) + std::abs(curr->getRMO() / width - target->getRMO() / width);
	int dmg = curr->getAb("Bow", ACTION)->rollSingleHit(atk_mod, curr->getDex(), target->getAC());
	messageBox.render((650 - messageBox.getWidth()) / 2, 614);
	SDL_RenderPresent(gRenderer);
	if (-1 != dmg) {
		target->damage(dmg);
	}
	Sleep(1000);
	room->getCurrUnit()->useAction();
	room->clearRange();
	//click_handler = NULL;
}