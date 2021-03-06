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
		  Mob(11, 18, 6, 1, 4, 2, 1, mobClips[SKELETON], "Skeleton", 0, 2, 2, -2, -2, -3, 6, mobHandler, ENEMY)
		, Mob(10, 15, 8, 1, 4, 1, 4, mobClips[RANGED_SKELETON], "Ranged Skeleton", 0, 2, 2, -2, -2, -3, 6, mobHandler, ENEMY)
		, Mob(10, 15, 6, 2, 5, 2, 1, mobClips[HORSE_SKELETON], "Skeleton Warhorse", 4, 1, 2, -4, -1, -3, 8, mobHandler, ENEMY)
		, Mob(13, 50, 10, 1, 5, 3, 8, mobClips[BONENAGA], "Bone Naga", 2, 3, 1, 2, 2, 3, 6, boneNagaHandler, ENEMY) 
	};
	abilityMap["Ray of Frost 1"] = { 5, 0, 10, 1, 6 };
	abilityMap["Bone Naga Bite"] = { 5, 4, 4, 2, 1 };


	//graveyard dungeon
	mobEncMap["Graveyard of the Forsaken"] = {
		Mob(8, 20, 6, 1, 3, 1, 1, mobClips[ZOMBIE], "Zombie", 1, -2, 3, -4, -2, -3, 4, mobHandler, ENEMY),
		Mob(8, 15, 4, 1, 3, 1, 1, mobClips[RUNNER], "Runner", 1, -2, 3, -4, -2, -3, 7, mobHandler, ENEMY),
		Mob(8, 28, 8, 1, 4, 2, 1, mobClips[BLOATED], "Bloated Zombie", 4, -2, 4, -4, -2, -3, 3, mobHandler, ENEMY),
		Mob(12, 58, 4, 1, 3, 1, 1, mobClips[WIGHT], "Wight", 2, 2, 3, 0, 1, 2, 6, wightHandler, ENEMY),
	};
	abilityMap["Longbow"] = { 5, 2, 8, 1, 6 };
	abilityMap["Longsword"] = { 5, 2, 10, 1, 1 };
	abilityMap["Lifedrain"] = { 5, 3, 6, 1, 1 };

	//goblin dungeon
	mobEncMap["Goblins and Garden Gnomes"] = {
		Mob(10, 14, 6, 1, 4, 2, 4, mobClips[GOBLIN], "Goblin", -1, 2, 0, 0, -1, -1, 5, mobHandler, ENEMY),
		Mob(11, 16, 8, 1, 5, 0, 1, mobClips[HOBGOBLIN], "Hobgoblin", 1, 1, 1, 0, 0, -1, 6, mobHandler, ENEMY),
		Mob(10, 12, 4, 2, 4, 0, 1, mobClips[WORG], "Worg", 3, 1, 1, -2, 0, -1, 8, mobHandler, ENEMY),
		Mob(13, 50, 6, 1, 3, 1, 1, mobClips[BUGBEAR], "Bugbear", 3, 2, 2, 0, 1, 0, 6, bugBearHandler, ENEMY),
	};
	abilityMap["Morningstar"] = { 4, 3, 8, 1, 1 };
	abilityMap["Javelin"] = { 4, 2, 6, 2, 6 };

	//orc dungeon
	mobEncMap["Mines of Dorimir"] = {
		Mob(11, 15, 6, 1, 5, 2, 3, mobClips[ORC], "Orc", 3, 1, 3, -2, 0, 0, 6, mobHandler, ENEMY),
		Mob(12, 18, 8, 1, 3, 1, 1, mobClips[OROG], "Orog", 4, 1, 4, 1, 0, 1, 6, mobHandler, ENEMY),
		Mob(11, 24, 8, 1, 3, 1, 1, mobClips[OGRILLON], "Ogrillon", 3, 0, 2, -2, -1, 0, 6, mobHandler, ENEMY),
		Mob(13, 52, 12, 1, 5, 1, 1, mobClips[WAR_CHIEF], "Orc War Chief", 4, 1, 4, 0, 0, 3, 6, warChiefHandler, ENEMY)
	};
	abilityMap["Greataxe 1"] = { 5, 2, 12, 1, 1 };
	abilityMap["Greataxe 2"] = { 3, 3, 10, 1, 1 };
	
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
	//reset threat vec
	for (i = 0; i < threat.size(); i++) {
		threat[i] = 0;
	}
	int width = room->getWidth();
	fillPartyVec(party);
	int mobx = getRMO() % width;
	int moby = getRMO() / width;
	int x, y;
	double slice, diff, ratio;
	int health_ratio;
	std::chrono::milliseconds ms = std::chrono::duration_cast< std::chrono::milliseconds >(
		std::chrono::system_clock::now().time_since_epoch()
		);
	srand(ms.count());//random seed
	//get true distance, unreachable is set to 1000
	std::vector<int> dist = scanRoom();

	for (i = 0; i < party.size(); i++) {
		//distance update threat
		x = party[i]->getRMO() % width;
		y = party[i]->getRMO() / width;
		//make unreachable targets very unthreatening
		if (dist[i] == 1000) {
			threat[i] = -10;
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
			threat[i] += 14;
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
	int highest_threat = -11;
	for (i = 0; i < party.size(); i++) {
		if (threat[i] > highest_threat)
			highest_threat = threat[i];
	}
	//if (highest_threat != 0) {
		//put highest threat into vec
		for (i = 0; i < party.size(); i++) {
			if (threat[i] == highest_threat)
				target.push_back(i);
		}
		target_index = target[rand() % target.size()];
	//}
	//case where char dies, everything else is obstructed, so default to prev target
	//choose target as last in party
	//more than likely only happens 
	//if (target_index >= party.size()) {
	//	target_index = party.size() - 1;
	//}
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
		if (room->getTile(x, y)->type == CHARACTER)
			continue;
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
	//im not sure as to why it breaks if this isn't here, but i guess critical part
	partyDistVec[0] = partyDistVec[0];
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
		room->getTile(RMO_list[i] % width, RMO_list[i] / width)->type = type_stash;
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
		messageBox.loadFromRenderedText("The " + name + " hit the " + party[target_index]->getName() + " for " + std::to_string(dmg) + "!", { 255, 255, 255 }, 650);
		messageBox.render((650 - messageBox.getWidth()) / 2, 614);
		party[target_index]->damage(dmg);
	}
	else {
		messageBox.loadFromRenderedText("The " + name + " missed the " + party[target_index]->getName() + ".", { 255, 255, 255 }, 650);
		messageBox.render((650 - messageBox.getWidth()) / 2, 614);
	}
	SDL_RenderPresent(gRenderer);
	Sleep(1000);
	return dmg;
}

int Mob::getBestManhDist(int index) {
	int width = room->getWidth();
	std::queue<int> prevQ;
	std::vector<int> distVec(width*room->getHeight(), 1000);
	distVec[RMO] = 0;
	prevQ.push(RMO);
	int best = 100000;
	int best_RMO = -1;
	//this will populate our distance vector with valid true path cost from the target
	while (!prevQ.empty()) {
		int x = prevQ.front() % width;
		int y = prevQ.front() / width;
		int dist = std::abs(x - index%width) + std::abs(y - index / width);
		if (dist < best) {
			best_RMO = prevQ.front();
			best = dist;
		}
		prevQ.pop();
		//add all adjacent non in distVec tiles
		if (x > 0) {
			if ((room->getTile(x - 1, y)->type == NOTHING)) {
				distVec[x - 1 + y * width] = distVec[x + y * width] + 1;
				prevQ.push(x - 1 + y * width);
			}
		}
		if (y > 0) {
			if (distVec[x + (y - 1) * width] == 1000 && (room->getTile(x, y - 1)->type == NOTHING)) {
				distVec[x + (y - 1) * width] = distVec[x + y * width] + 1;
				prevQ.push(x + (y - 1) * width);
			}
		}
		if (x < room->getWidth() - 1) {
			if (distVec[x + 1 + y * width] == 1000 && (room->getTile(x + 1, y)->type == NOTHING)) {
				distVec[x + 1 + y * width] = distVec[x + y * width] + 1;
				prevQ.push(x + 1 + y * width);
			}
		}
		if (y < room->getHeight() - 1) {
			if (distVec[x + (y + 1) * width] == 1000 && (room->getTile(x, y + 1)->type == NOTHING)) {
				distVec[x + (y + 1) * width] = distVec[x + y * width] + 1;
				prevQ.push(x + (y + 1) * width);
			}
		}
	}
	return best_RMO;

}

int Mob::getBestRMO(int index) {
	int width = room->getWidth();
	std::vector<int> distVec(room->getWidth()*room->getHeight(), 1000);
	std::queue<int> prevQ;
	distVec[index] = 0;
	
	prevQ.push(index);
	//this will populate our distance vector with valid true path cost from the target
	while (!prevQ.empty()) {
		int x = prevQ.front() % width;
		int y = prevQ.front() / width;
		prevQ.pop();

		//add all adjacent non in distVec tiles
		if (x > 0) {
			if (distVec[x - 1 + y * width] == 1000 && (room->getTile(x - 1, y)->type == NOTHING)) {
				distVec[x - 1 + y * width] = distVec[x + y * width] + 1;
				prevQ.push(x - 1 + y * width);
			}
		}
		if (y > 0) {
			if (distVec[x + (y - 1) * width] == 1000 && (room->getTile(x, y - 1)->type == NOTHING)) {
				distVec[x + (y - 1) * width] = distVec[x + y * width] + 1;
				prevQ.push(x + (y - 1) * width);
			}
		}
		if (x < room->getWidth() - 1) {
			if (distVec[x + 1 + y * width] == 1000 && (room->getTile(x + 1, y)->type == NOTHING)) {
				distVec[x + 1 + y * width] = distVec[x + y * width] + 1;
				prevQ.push(x + 1 + y * width);
			}
		}
		if (y < room->getHeight() - 1) {
			if (distVec[x + (y + 1) * width] == 1000 && (room->getTile(x, y + 1)->type == NOTHING)) {
				distVec[x + (y + 1) * width] = distVec[x + y * width] + 1;
				prevQ.push(x + (y + 1) * width);
			}
		}
	}
	//now we have a vector filled with distance data
	//use a priorityqueue setup to get to closest tile
	int closest_dist = 999;
	int RMO_target = -1;
	for (int i = 0; i < width*room->getHeight(); i++) {
		//uses an A* like hueristic to make sure mob choose closest tile to target that uses the least amount of movement
		if (room->getTile(i%width, i / width)->color != NORMAL && distVec[i] <= closest_dist) {
			if (RMO_target != -1 && distVec[i] == closest_dist) {
				if (std::abs(RMO_target%width - RMO % width) + std::abs(RMO_target / width - RMO / width) > std::abs(i%width - RMO % width) + std::abs(i / width - RMO / width)) {
					RMO_target = i;
					closest_dist = distVec[i];
				}
			}
			else {
				RMO_target = i;
				closest_dist = distVec[i];
			}
		}
	}
	//mob has no path to best RMO
	if (RMO_target != -1) {
		if (room->getTile(RMO_target%width, RMO_target / width)->type == NORMAL)
			return -1;
	}
	return RMO_target;
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
			//move towards target
			moveButton(0);
			ab = NOPE;
			int RMO_target = getBestRMO(party[target_index]->getRMO());
			if (RMO_target == -1) {
				//cannot reach anymob, revert to using manhatten dist
				RMO_target = getBestManhDist(party[target_index]->getRMO());
			}
			switch (room->getTile(RMO_target%width, RMO_target / width)->color) {
			case YELLOW:
				//move and attack
				moveMob(RMO_target);
				break;
			case BLUE:
				//move
				useAction();
				moveMob(RMO_target);
				break;
			case NORMAL:
				//find closest highlighted tile to target tile
				int new_target = getBestRMO(RMO_target);
				useAction();
				moveMob(new_target);
				break;
			}
		}
		room->clearRange();
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
			int target_RMO = getBestRMO(RMO);
			if (target_RMO == -1) {
				//cannot reach anymob, revert to using manhatten dist
				//since you cannot reach, delete action so that it isnt used later even though it doesnt have LOS
				target_RMO = getBestManhDist(party[target_index]->getRMO());
				useAction();
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
				int new_target = getBestRMO(target_RMO);
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
		int dist = 0, RMO = -1;
		if (mob->getMoveLeft() > 0) {
			for (int i = 0; i < width*room->getHeight(); i++) {
				int dist_to_target = std::abs(target_RMO%width - i % width) + std::abs(target_RMO / width - i / width);
				if (room->getTile(i%width, i / width)->color != NORMAL && dist_to_target > dist) {
					dist = dist_to_target;
					RMO = i;
				}
			}
			if (RMO != -1)
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
		int dist = 0, RMO = -1;
		if (mob->getMoveLeft() > 0) {
			for (int i = 0; i < width*room->getHeight(); i++) {
				int dist_to_target = std::abs(target_RMO%width - i % width) + std::abs(target_RMO / width - i / width);
				if (room->getTile(i%width, i / width)->color != NORMAL && dist_to_target > dist) {
					dist = dist_to_target;
					RMO = i;
				}
			}
			if(RMO != -1)
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
				if(room->getTile(x-1, y)->type == ENEMY)
					adj.push_back(party[i]);
			}
			if (y > 0) {
				if (room->getTile(x, y - 1)->type == ENEMY) 
					adj.push_back(party[i]);
			}
			if (x < room->getWidth() - 1) {
				if (room->getTile(x + 1, y)->type == ENEMY) 
					adj.push_back(party[i]);
			}

			if (y < room->getHeight() - 1) {
				if (room->getTile(x, y + 1)->type == ENEMY) 
					adj.push_back(party[i]);
			}
		}
		if (adj.size() != 0) {
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
					messageBox.loadFromRenderedText("The Bugbear grazes the " + adj[i]->getName()  + " for " + std::to_string(dmg/2) + "!", { 255, 255, 255 }, 650);
					adj[i]->damage(dmg / 2);
				}
				else {
					messageBox.loadFromRenderedText("The Bugbear hits the " + adj[i]->getName() + " for " + std::to_string(dmg) + "!", { 255, 255, 255 }, 650);
					adj[i]->damage(dmg);
				}
				drawRoom();
				messageBox.render((650 - messageBox.getWidth()) / 2, 614);
				SDL_RenderPresent(gRenderer);
				Sleep(1000);
			}
		}
		else {
			//revert to prev stage since no one is near
			mob->loadAbility("Morningstar");
			mob->attack(0);
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
	
}

void warChiefHandler() {
	//3 staged
	//stage 1 charge into battle
	//stage 2 2/3 hp rage, start spinning
	//stage 3 1/3 hp extra attack

	int i, roll;
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

	if (mob->getHP() < mob->getMaxHP() / 3) {
		//stage 3 attacks twice
		mob->loadAbility("Greataxe 2");
		messageBox.loadFromRenderedText("The War Chief is raging! He attacks with great speed!", { 255, 255, 255 }, 650);
		drawRoom();
		messageBox.render((650 - messageBox.getWidth()) / 2, 614);
		SDL_RenderPresent(gRenderer);
		Sleep(1000);
		mob->attack(0);
		mob->resetTurn();
		mob->updateThreat();
		target_index = mob->getTarget();
		mob->attack(0);
		
	}
	else if (mob->getHP() < (2*mob->getMaxHP()) / 3) {
		//stage 2 spinint i, roll;
		std::vector<Unit*> adj;
		for (i = 0; i < party.size(); i++) {
			int x = party[i]->getRMO() % width;
			int y = party[i]->getRMO() / width;
			if (x > 0) {
				if (room->getTile(x - 1, y)->type == ENEMY)
					adj.push_back(party[i]);
			}
			if (y > 0) {
				if (room->getTile(x, y - 1)->type == ENEMY)
					adj.push_back(party[i]);
			}
			if (x < room->getWidth() - 1) {
				if (room->getTile(x + 1, y)->type == ENEMY)
					adj.push_back(party[i]);
			}

			if (y < room->getHeight() - 1) {
				if (room->getTile(x, y + 1)->type == ENEMY)
					adj.push_back(party[i]);
			}
		}
		if (adj.size() != 0) {
			messageBox.loadFromRenderedText("The War Chief furiously spins!", { 255, 255, 255 }, 650);
			drawRoom();
			messageBox.render((650 - messageBox.getWidth()) / 2, 614);
			SDL_RenderPresent(gRenderer);
			Sleep(1000);
			int dmg = 4 + rand() % 10;
			for (i = 0; i < adj.size(); i++) {
				roll = adj[i]->getDex() + 1 + rand() % 20;
				if (roll > 13) {
					messageBox.loadFromRenderedText("The War Chief misses the " + adj[i]->getName() + "!", { 255, 255, 255 }, 650);
				}
				else if (roll > 10) {
					messageBox.loadFromRenderedText("The War Chief grazes the " + adj[i]->getName() + " for " + std::to_string(dmg / 2) + "!", { 255, 255, 255 }, 650);
					adj[i]->damage(dmg / 2);
				}
				else {
					messageBox.loadFromRenderedText("The War Chief hits the " + adj[i]->getName() + " for " + std::to_string(dmg) + "!", { 255, 255, 255 }, 650);
					adj[i]->damage(dmg);
				}
				drawRoom();
				messageBox.render((650 - messageBox.getWidth()) / 2, 614);
				SDL_RenderPresent(gRenderer);
				Sleep(1000);
			}
		}
		else {
			//revert to stage 1 if no chars
			mob->loadAbility("Greataxe 1");
			mob->attack(0);
		}
	}
	else {
		//stage 1 charge at target
		mob->loadAbility("Greataxe 1");
		mob->attack(0);

	}
	
}