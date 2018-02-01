#include "Mob.h"
#include "Room.h"
#include "LButton.h"

struct AbilityStats {
	int atk_mod;
	int dmg_mod;
	int dmg_dice;
	int num_dice;
	int range;
	//some special effect
};

std::unordered_map<std::string, std::vector<Mob>> mobEncMap;
std::unordered_map<std::string, AbilityStats> abilityMap;

//load all mobs into the map
void loadMobs() {
	//Each quest will have a list of mobs to populate mob encounters with
	//the last mob in list is always the boss

	//Spooky skeleton dungeon
	mobEncMap["Spooky, Scary, Skeletons"] = { 
		  Mob(11, 14, 4, 1, 4, 2, 1, mobClips[SKELETON], "Skeleton", 0, 2, 2, -2, -2, -3, 6, mobHandler, ENEMY)
		, Mob(10, 9, 6, 1, 4, 1, 6, mobClips[RANGED_SKELETON], "Ranged Skeleton", 0, 2, 2, -2, -2, -3, 6, mobHandler, ENEMY)
		, Mob(12, 40, 8, 1, 5, 3, 8, mobClips[BONENAGA], "Bone Naga", 2, 3, 1, 2, 2, 3, 6, boneNagaHandler, BOSS_BOI) 
	};
	abilityMap["Ray of Frost 1"] = { 5, 0, 8, 1, 6 };
	abilityMap["Bone Naga Bite"] = { 5, 3, 4, 2, 1 };


	//graveyard dungeon
	mobEncMap["Graveyard of the Forsaken"] = {
		Mob(8, 22, 6, 1, 3, 1, 1, mobClips[ZOMBIE], "Zombie", 1, -2, 3, -4, -2, -3, 4, mobHandler, ENEMY),
		Mob(8, 15, 4, 1, 3, 1, 1, mobClips[RUNNER], "Runner", 1, -2, 3, -4, -2, -3, 7, mobHandler, ENEMY),
		Mob(11, 45, 4, 1, 3, 1, 1, mobClips[WIGHT], "Wight", 2, 2, 3, 0, 1, 2, 6, wightHandler, BOSS_BOI),
	};
	abilityMap["Longbow"] = { 4, 2, 8, 1, 6 };
	abilityMap["Longsword"] = { 4, 2, 10, 1, 1 };
	abilityMap["Lifedrain"] = { 4, 2, 6, 1, 1 };

	//goblin dungeon
	mobEncMap["Goblins and Garden Gnomes"] = {
		Mob(10, 11, 6, 1, 4, 2, 4, mobClips[GOBLIN], "Goblin", -1, 2, 0, 0, -1, -1, 5, mobHandler, ENEMY),
		Mob(11, 13, 8, 1, 5, 0, 1, mobClips[HOBGOBLIN], "Hobgoblin", 1, 1, 1, 0, 0, -1, 6, mobHandler, ENEMY),
		Mob(10, 9, 4, 2, 5, 2, 1, mobClips[WORG], "Worg", 3, 1, 1, -2, 0, -1, 8, mobHandler, ENEMY),
		Mob(13, 35, 6, 1, 3, 1, 1, mobClips[BUGBEAR], "Bugbear", 3, 2, 2, 0, 1, 0, 6, bugBearHandler, BOSS_BOI),
	};
	abilityMap["Morningstar"] = { 4, 3, 8, 1, 1 };
	abilityMap["Javelin"] = { 4, 2, 6, 2, 6 };

	//beast dungeon
	
}

/* helpers */
void fillPartyVec(std::vector<Unit*>& party) {
	std::vector<Unit*>* initlist = room->getInititiveOrder();
	int i;
	int width = room->getWidth();
	for (i = 0; i < initlist->size(); i++) {
		if (initlist->at(i)->getType() == CHARACTER)
			party.push_back(initlist->at(i));
	}
}

Mob::Mob(){}

Mob::Mob(int AC_, int HP_, int dmg_, int dice_, int atk_mod_, int dmg_mod_, int range_, SDL_Rect icon, std::string name_, int s, int d, int c, int i, int w, int ch, int move_, void(*handler_)(), UnitType type_) {
	type = type_;
	AC = AC_;
	dmg_dice = dmg_;
	dice = dice_;
	icon_50 = icon;
	name = name_;
	atk_mod = atk_mod_;
	dmg_mod = dmg_mod_;
	str = s;
	dex = d;
	con = c;
	intel = i;
	wis = w;
	cha = ch;
	move = move_;
	health = HP_;
	max_health = HP_;
	threat.resize(3);
	threat[0] = 0;
	threat[1] = 0;
	threat[2] = 0;
	handler = handler_;
	range = range_;
}

SDL_Rect Mob::getIcon50() {

	return icon_50;
}

void Mob::updateThreat() {
	//get distance to all 3 party members
	std::vector<Unit*> party;
	int i;
	int width = room->getWidth();
	fillPartyVec(party);
	int mobx = getRMO() % width;
	int moby = getRMO() / width;
	int x, y;
	double slice, diff, ratio;
	int health_ratio;
	//get true distance, unreachable is set to 1000
	std::vector<int> dist = scanRoom();

	for (i = 0; i < party.size(); i++) {
		//distance update threat
		x = party[i]->getRMO() % width;
		y = party[i]->getRMO() / width;
		//make unreachable targets very unthreatening
		if (dist[i] == 1000) {
			threat[i] == -50;
		}
		else if(dist[i] > 16)
			threat[i] += 1;
		else if (dist[i] > 8)
			threat[i] += 2;
		else if (dist[i] > 7)
			threat[i] += 3;
		else if (dist[i] > 6)
			threat[i] += 4;
		else if (dist[i] > 5)
			threat[i] += 5;
		else if (dist[i] > 4)
			threat[i] += 6;
		else if (dist[i] > 3)
			threat[i] += 7;
		else if (dist[i] > 2)
			threat[i] += 8;
		else if (dist[i] > 1)
			threat[i] += 9;
		else if (dist[i] == 1)
			threat[i] += 16;
		//hp update threat
		slice = (double)(party[i]->getMaxHP()) / 11;
		diff = (party[i]->getMaxHP() - party[i]->getHP());
		ratio = diff / slice;
		ratio += .5;
		health_ratio = (int)ratio;
		if (health_ratio > 10)
			health_ratio = 10;
		switch (health_ratio) {
		case 4:
		case 5:
			threat[i] += 1;
			break;
		case 6:
		case 7:
			threat[i] += 2;
			break;
		case 8:
			threat[i] += 3;
				break;
		case 9:
			threat[i] += 4;
				break;

		}
	}
	
	//find highest threat 
	std::vector<int> target;
	int highest_threat = 0;
	for (i = 0; i < party.size(); i++) {
		if (threat[i] > highest_threat)
			highest_threat = threat[i];
	}
	//put highest threat into vec
	for (i = 0; i < party.size(); i++) {
		if (threat[i] == highest_threat)
			target.push_back(i);
	}
	std::chrono::milliseconds ms = std::chrono::duration_cast< std::chrono::milliseconds >(
		std::chrono::system_clock::now().time_since_epoch()
		);
	srand(ms.count());//random seed
	target_index = target[rand() % target.size()];
	//reset threat vec
	for (i = 0; i < threat.size(); i++) {
		threat[i] = 0;
	}
}

void Mob::addAttackThreat(int t) {
	std::vector<Unit*>* init = room->getInititiveOrder();
	int count=-1;
	for (int i = 0; i < init->size(); i++) {
		if (init->at(i)->getType() == CHARACTER)
			count++;
		if (init->at(i)->getName() == room->getCurrUnit()->getName()) {
			threat[count] += t;
			return;
		}
	}
}

//will find true distance to every character
std::vector<int> Mob::scanRoom() {
	std::vector<Unit*> party;
	fillPartyVec(party);
	std::vector<int> partyDistVec(party.size(), 1000);
	int width = room->getWidth();
	std::vector<int>distVec(width*room->getHeight(), -1);
	distVec[RMO] = 0;
	std::queue<int> prevQ;
	prevQ.push(RMO);
	int count=0;
	while (!prevQ.empty()) {
		int x = prevQ.front() % width;
		int y = prevQ.front() / width;
		if (room->getTile(x, y)->type == CHARACTER) {
			for (int i = 0; i < party.size(); i++) {
				if (party[i]->getRMO() == prevQ.front()) {
					partyDistVec[i] = distVec[party[i]->getRMO()];
					count++;
					break;
				}
			}
		}
		if (count == party.size())
			return partyDistVec;
		prevQ.pop();

		//add all adjacent non in distVec tiles
		if (x > 0) {
			if (distVec[x - 1 + y * width] == -1 && (room->getTile(x - 1,y)->type == NOTHING || room->getTile(x - 1, y)->type == CHARACTER)) {
				distVec[x - 1 + y * width] = distVec[x + y * width] + 1;
				prevQ.push(x - 1 + y * width);
			}
		}
		if (y > 0) {
			if (distVec[x + (y - 1) * width] == -1 && (room->getTile(x, y - 1)->type == NOTHING || room->getTile(x, y - 1)->type == CHARACTER)) {
				distVec[x + (y - 1) * width] = distVec[x + y * width] + 1;
				prevQ.push(x + (y - 1) * width);
			}
		}
		if (x < room->getWidth() - 1) {
			if (distVec[x + 1 + y * width] == -1 && (room->getTile(x + 1, y)->type == NOTHING || room->getTile(x + 1, y)->type == CHARACTER)) {
				distVec[x + 1 + y * width] = distVec[x + y * width] + 1;
				prevQ.push(x + 1 + y * width);
			}
		}
		if (y < room->getHeight() - 1) {
			if (distVec[x + (y + 1) * width] == -1 && (room->getTile(x, y + 1)->type == NOTHING || room->getTile(x, y + 1)->type == CHARACTER)) {
				distVec[x + (y + 1) * width] = distVec[x + y * width] + 1;
				prevQ.push(x + (y + 1) * width);
			}
		}
	}
	return partyDistVec;
}

//moves mob to target RMO
void Mob::moveMob(int RMO_target) {
	int width = room->getWidth();
	UnitType type_stash = room->getTile(RMO % width, RMO / width)->type;
	room->getTile(RMO % width, RMO / width)->type = NOTHING;
	std::vector<int> RMO_list = getPath(RMO_target, RMO);
	setMoveLeft(move_left - (RMO_list.size()-1));
	room->clearRange();
	messageBox.loadFromRenderedText("The " + name + " lumbers towards you.", { 255, 255, 255 } , 800);
	for (int i = 0; i < RMO_list.size(); i++) {
		//SDL_Delay(100);
		setRMO(RMO_list[i]);
		room->getTile(RMO_list[i] % width, RMO_list[i] / width)->type = ENEMY;
		drawRoom();
		messageBox.render((650 - messageBox.getWidth()) / 2, 614);
		room->getTile(RMO_list[i] % width, RMO_list[i] / width)->type = NOTHING;
		SDL_RenderPresent(gRenderer);
		Sleep(200);
	}
	room->getTile(RMO % width, RMO / width)->type = type_stash;
}

int Mob::roll_attack() {
	useAction();
	std::vector<Unit*> party;
	int i;
	fillPartyVec(party);
	drawRoom();
	messageBox.loadFromRenderedText("The " + name + " attacks the " + party[target_index]->getName() + "!", { 255, 255, 255 }, 650);
	messageBox.render((650 - messageBox.getWidth()) / 2, 614);
	SDL_RenderPresent(gRenderer);
	Sleep(1000);
	drawRoom();
	int roll = rand() % 20 + 1 + atk_mod;
	int dmg = -1;
	if (roll > party[target_index]->getAC()) {
		dmg = dmg_mod;
		for (i = 0; i < dice; i++) {
			dmg += (rand() % dmg_dice) + 1;
		}
		party[target_index]->damage(dmg);
		messageBox.loadFromRenderedText("The " + name + " hit the " + party[target_index]->getName() + " for " + std::to_string(dmg) + "!", { 255, 255, 255 }, 650);
		messageBox.render((650 - messageBox.getWidth()) / 2, 614);
	}
	else {
		messageBox.loadFromRenderedText("The " + name + " missed the " + party[target_index]->getName() + ".", { 255, 255, 255 }, 650);
		messageBox.render((650 - messageBox.getWidth()) / 2, 614);
	}
	SDL_RenderPresent(gRenderer);
	Sleep(1000);
	return dmg;
}

int Mob::attack(int index) {
	//discard index since mob will attack stored in target index
	//check if already adjacent
	std::chrono::milliseconds ms = std::chrono::duration_cast< std::chrono::milliseconds >(
		std::chrono::system_clock::now().time_since_epoch()
		);
	srand(ms.count());//random seed

	int i;
	int width = room->getWidth();
	std::vector<Unit*> party;
	fillPartyVec(party);

	int distance = std::abs((room->getCurrUnit()->getRMO() % width) - party[target_index]->getRMO() % width) + std::abs((room->getCurrUnit()->getRMO() / width) - party[target_index]->getRMO() / width);
	//for melee mobs
	if (range == 1) {
		//use move
		if (distance != 1) {
			int dist;
			//move towards target
			moveButton(0);
			ab = NOPE;
			std::vector<int> distances(room->getWidth()*room->getHeight(), -1);
			for (i = 0; i < distances.size(); i++) {
				if (room->getTile(i % width, i / width)->color != NORMAL) {
					//get distance and store in vector
					dist = std::abs(party[target_index]->getRMO() % width - i % width) + std::abs(party[target_index]->getRMO() / width - i / width);
					distances[i] = dist;
				}
			}
			int RMO_target = 0;
			//get RMO
			dist = 20;
			for (i = 0; i < distances.size(); i++) {
				if (dist > distances[i] && distances[i] != -1) {
					dist = distances[i];
					RMO_target = i;
				}
			}
			//dash if selected tile is in dash zone
			if (room->getTile(RMO_target%width, RMO_target / width)->color == BLUE) {
				useAction();
			}
			moveMob(RMO_target);
		}
		distance = std::abs((room->getCurrUnit()->getRMO() % width) - party[target_index]->getRMO() % width) + std::abs((room->getCurrUnit()->getRMO() / width) - party[target_index]->getRMO() / width);
		if (distance == 1 && action) {
			//attack target
			return roll_attack();
		}
	}
	else {
		//for ranged mobs

		//the line of sight coloring uses the tile types, remove current mob from tile to not consider it in calculations
		UnitType type_stash = room->getTile(RMO%width, RMO / width)->type;
		room->getTile(RMO%width, RMO / width)->type = NOTHING;
		rangeColor(party[target_index]->getRMO()%width, party[target_index]->getRMO() / width, range, true);
		LOSColor(party[target_index]->getRMO() % width, party[target_index]->getRMO() / width);
		//place unit type back into the tiles
		room->getTile(RMO%width, RMO / width)->type = type_stash;
		//fails if already in range
		if(room->getTile(RMO%width, RMO / width)->color != RED) {
			//move to closest red
		int closest = 50;
		int target_RMO, target_dist, dist_to_target;
		for (i = 0; i < width*room->getHeight(); i++) {
			int dist_to_RMO = std::abs(i%width - RMO % width) + std::abs(i / width - RMO / width);
			dist_to_target = std::abs(i%width - party[target_index]->getRMO() % width) + std::abs(i / width - party[target_index]->getRMO() / width);
			if (room->getTile(i%width, i / width)->color == RED && dist_to_RMO < closest) {
				target_RMO = i;
				closest = dist_to_RMO;
				target_dist = dist_to_target;
			}
		}
		//now we have a target index, clear the range and commence moving
		room->clearRange();
		moveButton(0);
		ab = NOPE;
		//use highlights to move mob accordingly
		switch (room->getTile(target_RMO%width, target_RMO / width)->color) {
		case YELLOW:
			//move and attack
			moveMob(target_RMO);
			break;
		case BLUE:
			//move
			useAction();
			moveMob(target_RMO);
			break;
		case NORMAL:
			//find closest highlighted tile to target tile
			int dist_to_tile = 100, new_target;
			for (i = 0; i < width*room->getHeight(); i++) {
				int new_dist = std::abs(i%width - target_RMO % width) + std::abs(i / width - target_RMO / width);
				if (room->getTile(i%width, i / width)->color != NORMAL && new_dist < dist_to_tile) {
					dist_to_tile = new_dist;
					new_target = i;
				}
			}
			useAction();
			moveMob(new_target);
			break;
		}

		}
		room->clearRange();
		if (getAction()) {
			//attack target_index
			return roll_attack();
		}
	}
	return -1;
}

void Mob::loadAbility(std::string name) {
	AbilityStats ab = abilityMap[name];
	atk_mod = ab.atk_mod;
	dmg_mod = ab.dmg_mod;
	dmg_dice = ab.dmg_dice;
	dice = ab.num_dice;
	range = ab.range;
}
/* normal mob handlers */

//handles AI response for generic non-boss melee mobs
void mobHandler() {
	Unit* mob = room->getCurrUnit();
	mob->updateThreat();
	mob->attack(0);
	endTurnHandler(0);
}

/* Boss mob handlers */

void boneNagaHandler() {
	//has ranged ray of frost
	//has melee bite

	Unit* mob = room->getCurrUnit();
	mob->updateThreat();
	int target_index = mob->getTarget();
	int width = room->getWidth();
	std::vector<Unit*> party;
	fillPartyVec(party);
	//staged fight

	//stage 1, far away from characters
	//use range, stay away

	//stage 2, characters are close
	//use melee to attack

	//stage 3, hp gets low (1/3)?
	//run away and use range
	//if no movement options, use melee

	//check stages in decreasing order
	if (mob->getHP() < mob->getMaxHP() / 3) {
		//stage 3
		//do some running
		int target_RMO = party[target_index]->getRMO();
		if (std::abs(target_RMO%width - mob->getRMO() % width) + std::abs(target_RMO / width - mob->getRMO() / width) == 1) {
			//do a melee attack then run
			mob->loadAbility("Bone Naga Bite");
			mob->attack(0);
		}
		else {
			//try to get into the red and attack
			mob->loadAbility("Ray of Frost 1");
			mob->attack(0);
		}
		//run
		moveButton(0);
		int dist = 0, RMO;
		if (mob->getMoveLeft() > 0) {
			for (int i = 0; i < width*room->getHeight(); i++) {
				int dist_to_target = std::abs(target_RMO%width - i % width) + std::abs(target_RMO / width - i / width);
				if (room->getTile(i%width, i / width)->color != NORMAL && dist_to_target > dist) {
					dist = dist_to_target;
					RMO = i;
				}
			}
			mob->moveMob(RMO);
		}

	}
	else if (std::abs(mob->getRMO() % width - party[target_index]->getRMO() % width) + std::abs(mob->getRMO() / width - party[target_index]->getRMO() / width) < mob->getMove() - 3) {
		//stage 2
		mob->loadAbility("Bone Naga Bite");
		mob->attack(0);
	}
	else {
		//stage 1
		mob->loadAbility("Ray of Frost 1");
		mob->attack(0);

	}
	endTurnHandler(0);
}

void wightHandler() {
	std::chrono::milliseconds ms = std::chrono::duration_cast< std::chrono::milliseconds >(
		std::chrono::system_clock::now().time_since_epoch()
		);
	srand(ms.count());//random seed
	Unit* mob = room->getCurrUnit();
	mob->updateThreat();
	int target_index = mob->getTarget();
	int width = room->getWidth();
	std::vector<Unit*> party;
	fillPartyVec(party);
	//staged fight

	//stage 1, far away from characters
	//use longbow

	//stage 2, characters are close
	//use longsword

	//stage 3, hp gets low (1/2)?
	//use lifedrain if close, else use longbow and run
	//if no movement options, use melee

	//check stages in decreasing order
	if (mob->getHP() < mob->getMaxHP() / 2) {
		//stage 3
		//do some running
		int target_RMO = party[target_index]->getRMO();
		if (std::abs(target_RMO%width - mob->getRMO() % width) + std::abs(target_RMO / width - mob->getRMO() / width) == 1) {
			//custom lifedrain
			mob->loadAbility("Lifedrain");
			int healed = mob->attack(0);
			if (healed != -1) {
				messageBox.loadFromRenderedText("The Wight attempts to drain your life!", { 255, 255, 255 }, 650);
				drawRoom();
				messageBox.render((650 - messageBox.getWidth()) / 2, 614);
				SDL_RenderPresent(gRenderer);
				Sleep(1000);
				if (1 + (rand() % 20) + party[target_index]->getCon() <= 13) {
					mob->heal(healed);
					messageBox.loadFromRenderedText("The " + party[target_index]->getName() + " cannot resist the dark pull!", { 255, 255, 255 }, 650);
				}
				else {
					messageBox.loadFromRenderedText("The " + party[target_index]->getName() + " resists the Wight!", { 255, 255, 255 }, 650);
				}
				drawRoom();
				messageBox.render((650 - messageBox.getWidth()) / 2, 614);
				SDL_RenderPresent(gRenderer);
				Sleep(1000);
			}
		}
		else {
			//try to get into the red and attack
			mob->loadAbility("Longbow");
			mob->attack(0);
		}
		//run
		moveButton(0);
		int dist = 0, RMO;
		if (mob->getMoveLeft() > 0) {
			for (int i = 0; i < width*room->getHeight(); i++) {
				int dist_to_target = std::abs(target_RMO%width - i % width) + std::abs(target_RMO / width - i / width);
				if (room->getTile(i%width, i / width)->color != NORMAL && dist_to_target > dist) {
					dist = dist_to_target;
					RMO = i;
				}
			}
			mob->moveMob(RMO);
		}

	}
	else if (std::abs(mob->getRMO() % width - party[target_index]->getRMO() % width) + std::abs(mob->getRMO() / width - party[target_index]->getRMO() / width) < mob->getMove()-1) {
		//stage 2
		mob->loadAbility("Longsword");
		mob->attack(0);
	}
	else {
		//stage 1
		mob->loadAbility("Longbow");
		mob->attack(0);

	}
	endTurnHandler(0);
}

void bugBearHandler() {
	Unit* mob = room->getCurrUnit();
	mob->updateThreat();
	int target_index = mob->getTarget();
	int width = room->getWidth();
	std::vector<Unit*> party;
	fillPartyVec(party);
	std::chrono::milliseconds ms = std::chrono::duration_cast< std::chrono::milliseconds >(
		std::chrono::system_clock::now().time_since_epoch()
		);
	srand(ms.count());//random seed
	//staged fight

	//stage 1, far away from characters
	//use range, stay away

	//stage 2, characters are close
	//use melee to attack

	//stage 3, spin, hit all adj chars if >2 chars near
	//run away and use range

	//check stages in decreasing order
	if (std::abs(mob->getRMO() % width - party[target_index]->getRMO() % width) + std::abs(mob->getRMO() / width - party[target_index]->getRMO() / width) == 1) {
		//stage 3 custom spin attack
		//find out who is adjacent to bugbear
		int i, roll;
		std::vector<Unit*> adj;
		for (i = 0; i < party.size(); i++) {
			int x = party[i]->getRMO() % width;
			int y = party[i]->getRMO() / width;
			if (x > 0) {
				if(room->getTile(x-1, y)->type == BOSS_BOI)
					adj.push_back(party[i]);
			}
			if (y > 0) {
				if (room->getTile(x, y - 1)->type == BOSS_BOI) 
					adj.push_back(party[i]);
			}
			if (x < room->getWidth() - 1) {
				if (room->getTile(x + 1, y)->type == BOSS_BOI) 
					adj.push_back(party[i]);
			}

			if (y < room->getHeight() - 1) {
				if (room->getTile(x, y + 1)->type == BOSS_BOI) 
					adj.push_back(party[i]);
			}
		}
		messageBox.loadFromRenderedText("The Bugbear furiously spins!", { 255, 255, 255 }, 650);
		drawRoom();
		messageBox.render((650 - messageBox.getWidth()) / 2, 614);
		SDL_RenderPresent(gRenderer);
		Sleep(1000);
		int dmg = 4 + rand() % 10;
		for (i = 0; i < adj.size(); i++) {
			roll = adj[i]->getDex() + 1 + rand() % 20;
			if (roll > 13) {
				messageBox.loadFromRenderedText("The Bugbear misses the " + adj[i]->getName() + "!", { 255, 255, 255 }, 650);
			}
			else if (roll > 10) {
				messageBox.loadFromRenderedText("The Bugbear grazes the " + adj[i]->getName() + "!", { 255, 255, 255 }, 650);
				adj[i]->damage(dmg / 2);
			}
			else {
				messageBox.loadFromRenderedText("The Bugbear hits the " + adj[i]->getName() + "!", { 255, 255, 255 }, 650);
				adj[i]->damage(dmg);
			}
			drawRoom();
			messageBox.render((650 - messageBox.getWidth()) / 2, 614);
			SDL_RenderPresent(gRenderer);
			Sleep(500);
		}
	}
	else if (std::abs(mob->getRMO() % width - party[target_index]->getRMO() % width) + std::abs(mob->getRMO() / width - party[target_index]->getRMO() / width) < mob->getMove() - 3) {
		//stage 2
		mob->loadAbility("Morningstar");
		mob->attack(0);
	}
	else {
		//stage 1
		mob->loadAbility("Javelin");
		mob->attack(0);

	}
	endTurnHandler(0);
}