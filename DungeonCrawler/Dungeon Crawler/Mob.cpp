#include "Mob.h"
#include "Room.h"
#include "LButton.h"

std::unordered_map<std::string, Mob> mobMap;

//load all mobs into the map
void loadMobs() {
	mobMap["Skeleton"] = Mob(13, 13, 6, 1, 4, 2, mobClips[SKELETON], "Skeleton", 0, 2, 2, -2, -2, -3, 6);

}

//handles AI response for melee mobs
void meleeMobHandler() {
	Unit* mob = room->getCurrUnit();
	mob->updateThreat();
	mob->attack(0);
}

Mob::Mob(){}

Mob::Mob(int AC_, int HP_, int dmg_, int dice_, int atk_mod_, int dmg_mod_, SDL_Rect icon, std::string name_, int s, int d, int c, int i, int w, int ch, int move_){
	type = ENEMY;
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
	HP = HP_;
	max_HP = HP_;
	threat.resize(3);
	threat[0] = 0;
	threat[1] = 0;
	threat[2] = 0;
	handler = meleeMobHandler;
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
	for (i = 0; i < party.size(); i++) {
		x = party[i]->getRMO() % width;
		y = party[i]->getRMO() / width;
		distance = std::abs(x - mobx) + std::abs(y = moby);
		if (distance > 10)
			threat[i] += 1;
		else if (distance > 5)
			threat[i] += 2;
		else if (distance > 3)
			threat[i] += 3;
		else
			threat[i] += 4;
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
}

void Mob::attack(int index) {
	//discard index since mob will attack stored in target index
	//check if already adjacent
	std::vector<Unit*>* initlist = room->getInititiveOrder();
	std::vector<Unit*> party;
	int i;
	int width = room->getWidth();
	for (i = 0; i < initlist->size(); i++) {
		if (initlist->at(i)->getType() == CHARACTER)
			party.push_back(initlist->at(i));
	}
	int distance = std::abs((room->getCurrUnit()->getRMO() % width) - party[target_index]->getRMO() % width) + std::abs((room->getCurrUnit()->getRMO() / width) - party[target_index]->getRMO() / width);
	//already near attack
	if (distance != 1) {
		int dist;
		//move towards target
		moveButton(0);
		ab = NOPE;
		std::vector<int> distances(room->getWidth()*room->getHeight(), -1);
		for (i = 0; i < distances.size(); i++) {
			if (room->getTile(i % width, i/width)->type == RANGE) {
				//get distance and store in vector
				dist = std::abs(party[target_index]->getRMO() % width - i%width) + std::abs(party[target_index]->getRMO() / width - i/width);
				distances[i] = dist;
			}
		}
		int RMO = 0;
		//get RMO
		dist = 10;
		for (i = 0; i < distances.size(); i++) {
			if (dist > distances[i] && distances[i] != -1) {
				dist = distances[i];
				RMO = i;
			}
		}
		room->getTile(getRMO() % width, getRMO() / width)->type = NOTHING;
		std::vector<int> RMO_list = getPath(RMO, getRMO());
		room->clearRange();
		for (i = 0; i < RMO_list.size(); i++) {
			//SDL_Delay(100);
			setRMO(RMO_list[i]);
			room->getTile(RMO_list[i] % width, RMO_list[i] / width)->type = ENEMY;
			drawRoom();
			room->getTile(RMO_list[i] % width, RMO_list[i] / width)->type = NOTHING;
			SDL_RenderPresent(gRenderer);
			Sleep(200);
		}
		room->getTile(getRMO() % width, getRMO() / width)->type = ENEMY;
	}
	if (distance == 1) {
		//attack
	}
	endTurnHandler(0);
}