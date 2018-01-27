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
	health = HP_;
	max_health = HP_;
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


void Mob::moveMob() {
	std::vector<Unit*>* initlist = room->getInititiveOrder();
	std::vector<Unit*> party;
	int i;
	int width = room->getWidth();
	for (i = 0; i < initlist->size(); i++) {
		if (initlist->at(i)->getType() == CHARACTER)
			party.push_back(initlist->at(i));
	}
	int dist;
	//move towards target
	moveButton(0);
	ab = NOPE;
	std::vector<int> distances(room->getWidth()*room->getHeight(), -1);
	for (i = 0; i < distances.size(); i++) {
		if (room->getTile(i % width, i / width)->color == YELLOW) {
			//get distance and store in vector
			dist = std::abs(party[target_index]->getRMO() % width - i % width) + std::abs(party[target_index]->getRMO() / width - i / width);
			distances[i] = dist;
		}
	}
	int RMO = 0;
	//get RMO
	dist = 15;
	for (i = 0; i < distances.size(); i++) {
		if (dist > distances[i] && distances[i] != -1) {
			dist = distances[i];
			RMO = i;
		}
	}
	room->getTile(getRMO() % width, getRMO() / width)->type = NOTHING;
	std::vector<int> RMO_list = getPath(RMO, getRMO());
	setMoveLeft(move_left - (RMO_list.size()-1));
	room->clearRange();
	messageBox.loadFromRenderedText("The " + name + " lumbers towards you.", { 255, 255, 255 } , 800);
	for (i = 0; i < RMO_list.size(); i++) {
		//SDL_Delay(100);
		setRMO(RMO_list[i]);
		room->getTile(RMO_list[i] % width, RMO_list[i] / width)->type = ENEMY;
		drawRoom();
		messageBox.render((650 - messageBox.getWidth()) / 2, 614);
		room->getTile(RMO_list[i] % width, RMO_list[i] / width)->type = NOTHING;
		SDL_RenderPresent(gRenderer);
		Sleep(200);
	}
	room->getTile(getRMO() % width, getRMO() / width)->type = ENEMY;
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
	//use move
	if (distance != 1) {
		moveMob();
	}
	distance = std::abs((room->getCurrUnit()->getRMO() % width) - party[target_index]->getRMO() % width) + std::abs((room->getCurrUnit()->getRMO() / width) - party[target_index]->getRMO() / width);
	//if still not within range, use action to move
	if (distance != 1) {
		//repeat move
		resetTurn();
		moveMob();
	}
	else {
		//attack target
		drawRoom();
		messageBox.loadFromRenderedText("The " + name + " attacks the " + party[target_index]->getName() + "!", { 255, 255, 255 }, 650);
		messageBox.render((650 - messageBox.getWidth()) / 2, 614);
		SDL_RenderPresent(gRenderer);
		Sleep(1000);
		drawRoom();
		int roll = rand() % 20 + 1 + atk_mod;
		if (roll > party[target_index]->getAC()) {
			int dmg=dmg_mod;
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
	endTurnHandler(0);
}