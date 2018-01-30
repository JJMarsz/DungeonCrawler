#include "Mob.h"
#include "Room.h"
#include "LButton.h"

std::unordered_map<std::string, std::vector<Mob>> mobEncMap;

//load all mobs into the map
void loadMobs() {
	//Each quest will have a list of mobs to populate mob encounters with
	//the last mob in list is always the boss

	//Spooky skeleton dungeon
	mobEncMap["Spooky, Scary, Skeletons"] = { 
		  Mob(12, 13, 6, 1, 4, 2, 1, mobClips[SKELETON], "Skeleton", 0, 2, 2, -2, -2, -3, 6, mobHandler, ENEMY)
		, Mob(11, 9, 8, 1, 4, 1, 6, mobClips[RANGED_SKELETON], "Ranged Skeleton", 0, 2, 2, -2, -2, -3, 6, mobHandler, ENEMY)
		//, Mob(15, 40, 8, 1, 5, 3, mobClips[BONENAGA], "Bone Naga", 2, 3, 1, 2, 2, 3, 6, meleeMobHandler, BOSS_BOI) 
	};


}

//handles AI response for generic non-boss melee mobs
void mobHandler() {
	Unit* mob = room->getCurrUnit();
	mob->updateThreat();
	mob->attack(0);
}

Mob::Mob(){}

Mob::Mob(int AC_, int HP_, int dmg_, int dice_, int atk_mod_, int dmg_mod_, int range_, SDL_Rect icon, std::string name_, int s, int d, int c, int i, int w, int ch, int move_, void(*handler_)(), UnitType type_) {
	type = type_;
	AC = AC_;
	dmg = dmg_;
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
	std::vector<Unit*>* initlist = room->getInititiveOrder();
	std::vector<Unit*> party;
	int i;
	int width = room->getWidth();
	for (i = 0; i < initlist->size(); i++) {
		if (initlist->at(i)->getType() == CHARACTER)
			party.push_back(initlist->at(i));
	}
	int mobx = getRMO() % width;
	int moby = getRMO() / width;
	int x, y, distance;
	double slice, diff, ratio;
	int health_ratio;
	for (i = 0; i < party.size(); i++) {
		//distance update threat
		x = party[i]->getRMO() % width;
		y = party[i]->getRMO() / width;
		distance = std::abs(x - mobx) + std::abs(y - moby);
		if(distance > 16)
			threat[i] += 1;
		else if (distance > 8)
			threat[i] += 2;
		else if (distance > 7)
			threat[i] += 3;
		else if (distance > 6)
			threat[i] += 4;
		else if (distance > 5)
			threat[i] += 5;
		else if (distance > 4)
			threat[i] += 6;
		else if (distance > 3)
			threat[i] += 7;
		else if (distance > 2)
			threat[i] += 8;
		else if (distance > 1)
			threat[i] += 9;
		else if (distance == 1)
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

void Mob::roll_attack() {
	std::vector<Unit*>* initlist = room->getInititiveOrder();
	std::vector<Unit*> party;
	int i;
	int width = room->getWidth();
	for (i = 0; i < initlist->size(); i++) {
		if (initlist->at(i)->getType() == CHARACTER)
			party.push_back(initlist->at(i));
	}
	drawRoom();
	messageBox.loadFromRenderedText("The " + name + " attacks the " + party[target_index]->getName() + "!", { 255, 255, 255 }, 650);
	messageBox.render((650 - messageBox.getWidth()) / 2, 614);
	SDL_RenderPresent(gRenderer);
	Sleep(1000);
	drawRoom();
	int roll = rand() % 20 + 1 + atk_mod;
	if (roll > party[target_index]->getAC()) {
		int dmg = dmg_mod;
		for (i = 0; i < dice; i++) {
			dmg += (rand() % dmg) + 1;
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
}

void Mob::attack(int index) {
	//discard index since mob will attack stored in target index
	//check if already adjacent
	std::chrono::milliseconds ms = std::chrono::duration_cast< std::chrono::milliseconds >(
		std::chrono::system_clock::now().time_since_epoch()
		);
	srand(ms.count());//random seed
	std::vector<Unit*>* initlist = room->getInititiveOrder();
	std::vector<Unit*> party;
	int i;
	int width = room->getWidth();
	for (i = 0; i < initlist->size(); i++) {
		if (initlist->at(i)->getType() == CHARACTER)
			party.push_back(initlist->at(i));
	}
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
			roll_attack();
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
		drawRoom();
		SDL_RenderPresent(gRenderer);

		//fails if already in range
		if(room->getTile(RMO%width, RMO / width)->color != RED) {
			//move to closest red
			int closest = 50;
			int target_RMO, target_dist, dist_to_target;
			for (i = 0; i < width*room->getHeight(); i++) {
				int dist_to_RMO = std::abs(i%width - RMO % width) + std::abs(i / width - RMO / width);
				dist_to_target = std::abs(i%width - party[target_index]->getRMO()%width) + std::abs(i / width - party[target_index]->getRMO() / width);
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
		if (getAction()) {
			//attack target_index
			roll_attack();
		}

	}
	endTurnHandler(0);
}