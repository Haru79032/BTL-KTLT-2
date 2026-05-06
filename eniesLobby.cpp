#include "eniesLobby.h"
int clamp(int value, int minVal, int maxVal){
    if (value < minVal) return minVal;
    if (value > maxVal) return maxVal;
    return value;
}
/*
 * BattleContext
 */
BattleContext::BattleContext() {
    turnCount = 0;
    morale = 0;
    alarmLevel = 0;
    rescueProgress = 0;
    escapeProgress = 0;
    busterCallTimer = 0;
    mainGateDestroyed = false;
    robinRescued = false;
    bridgeOpened = false;
    battleEnded = false;
    resultCode = "";
}

void BattleContext::nextTurn() {
    // TODO: implement
}

/*
 * Character
 */
Character::Character() {
    name = "";
    hp = 0;
    maxHp = 0;
    atk = 0;
    def = 0;
    speed = 0;
    energy = 0;
    alive = false;
}

Character::Character(string name, int hp, int atk, int def, int speed, int energy): name(name), hp(hp), atk(atk), def(def), speed(speed), energy(energy) {
    // TODO: implement
    maxHp = hp;
    if (hp > 0) alive =true;
    else alive=false;

}

Character::~Character() {
    // TODO: implement if needed
}

int Character::attack(Building* target, BattleContext& context) {
    return 0;
}

int Character::specialSkill(Building* target, BattleContext& context) {
    return 0;
}

void Character::endTurn(BattleContext& context) {
    return ;
}

void Character::receiveDamage(int damage) {
    // TODO: implement
    if (damage - def <=0 ) return;
    hp-=(damage -def);
    if (hp <=0) {
        hp=0;
        alive=false;
    }
}

bool Character::isAlive() const {
    // TODO: implement
    return alive;
}

string Character::getName() const {
    // TODO: implement
    return name;
}

int Character::getHP() const {
    // TODO: implement
    return hp;
}

int Character::getEnergy() const {
    // TODO: implement
    return energy;
}

int Character::getDef() const{
    return def;
}

bool Character::isStrawHat() const {
    return false;
}

bool Character::isCP9() const {
    return false;
}

float Character::getHpPercen() const {
    if (maxHp == 0) return 0.0f; // Avoid division by zero
    return static_cast<float>(hp) / maxHp;
}

/*
 * StrawHat
 */
StrawHat::StrawHat() : Character() {
    bounty = 0;
}

StrawHat::StrawHat(string name, int hp, int atk, int def,
                   int speed, int energy, long long bounty): Character(name, hp, atk, def, speed, energy), bounty(bounty) {
    // TODO: implement
}

bool StrawHat::isStrawHat() const {
    // TODO: implement
    return true;
}

string StrawHat::str() const {
    // TODO: implement
    stringstream ss;
    ss << "StrawHat [ name =" << name << ", hp = " << hp << ", atk = " << atk
       << ", def = " << def << ", speed = " << speed
       << ", energy = " << energy << ", bounty = " << bounty << " ]";
    return ss.str();
}

/*
 * Luffy
 */
Luffy::Luffy(string name, int hp, int atk, int def,
             int speed, int energy, long long bounty): StrawHat(name, hp, atk, def, speed, energy, bounty) {
    // TODO: implement
}

int Luffy::attack(Character* target, BattleContext& context) {
    // TODO: implement
    int damage = 0;
    if (getHpPercen() > 0.5){damage = atk;}
    else if (getHpPercen() > 0.3 && getHpPercen() <=0.5) {damage = ceil(atk*1.15);}
    else {damage = ceil(atk*1.3);}
    target->receiveDamage(damage);
    if (!target->isAlive()){
        context.updateMorale(5);
        killsDuringTurn=true;
    }
    return damage;
}

int Luffy::specialSkill(Character* target, BattleContext& context) {
    // TODO: implement
    if (getHpPercen() >= 0.15 && energy >= 20){
        int damage = atk*2;
        target->receiveDamage(damage);
        speed += 15; 
        atk += 15;
        context.alarmLevel+=10;
        hp=ceil(hp*0.8);
        energy= clamp(energy - 20, 0, 100);
        if (!target->isAlive()){
            killsDuringTurn=true;
            context.updateMorale();
        }
        return damage;
    }
    return 0;
}

void Luffy::endTurn(BattleContext& context) {
    // TODO: implement
    if (getHpPercen() <= 0.3){
        context.updateMorale(3);
    }
    if (killsDuringTurn){
        energy = clamp(energy + 5, 0, 100);
        killsDuringTurn=false;
    }
}

/*
 * Zoro
 */
Zoro::Zoro(string name, int hp, int atk, int def,
           int speed, int energy, long long bounty): StrawHat(name, hp, atk, def, speed, energy, bounty) {
    // TODO: implement
}

int Zoro::attack(Character* target, BattleContext& context) {
    // TODO: implement
    int damage = 0;
    if (getHpPercen() < 0.4) {
        damage = ceil(ceil(atk+def*0.2)*1.15);
    }else{
        damage = ceil(atk+def*0.2);
    }
    target->receiveDamage(damage);
    if (!target->isAlive()){
        context.updateMorale();
        killsDuringTurn=true;
    }
    return damage;
}

int Zoro::specialSkill(Character* target, BattleContext& context) {
    // TODO: implement
    if (energy >= 15){
        int damage = 0;
        if (target->getHpPercen() < 0.5){
            damage = ceil(ceil(atk*2.2)*1.5);
            if (!target->isAlive()){
                context.updateMorale(4);
                killsDuringTurn=true;
                energy+=8;
            }
        }else{
            damage = ceil(atk*2.2);
            if (!target->isAlive()){
                context.updateMorale(4);
                killsDuringTurn=true;
                energy+=8;
            }
        }
        target->receiveDamage(damage);
        energy= clamp(energy - 15, 0, 100);
        return damage;
    }
    return 0;
}

void Zoro::endTurn(BattleContext& context) {
    // TODO: implement
    if (killsDuringTurn){
        context.updateMorale(6);
        atk = ceil(atk*1.05);
        killsDuringTurn=false;
    }
}

/*
 * Sanji
 */
Sanji::Sanji(string name, int hp, int atk, int def,
             int speed, int energy, long long bounty): StrawHat(name, hp, atk, def, speed, energy, bounty) {
    // TODO: implement
}

int Sanji::attack(Character* target, BattleContext& context) {
    // TODO: implement
    int damage = 0;
    if (target->getDef() >= def){
        damage = ceil(atk+speed*0.5);
    }else{
        damage = ceil(ceil(atk+speed*0.5)*1.1);
    }
    target->receiveDamage(damage);
    if (!target->isAlive()){
        context.updateMorale();
        killsDuringTurn=true;
    }
    return damage;
}

int Sanji::specialSkill(Character* target, BattleContext& context) {
    // TODO: implement
    if (energy >= 18){
        int damage = ceil(atk*2.1);
        target->receiveDamage(damage);
        if (target->getName() == "Jabra"){
            target->setDef(target->getDef()-12);
        }else target->setDef(target->getDef()-8);
        energy= clamp(energy - 18, 0, 100);
        if (!target->isAlive()){
            context.updateMorale();
            killsDuringTurn=true;
        }
        return damage;
    }
    return 0;
}

void Sanji::endTurn(BattleContext& context) {
    // TODO: implement
    if (killsDuringTurn){
        context.updateMorale(8);
        atk=ceil(atk*1.1);
        killsDuringTurn=false;
    }
}

/*
 * Nami
 */
Nami::Nami(string name, int hp, int atk, int def,
           int speed, int energy, long long bounty): StrawHat(name, hp, atk, def, speed, energy, bounty) {
    // TODO: implement
}

int Nami::attack(Character* target, BattleContext& context) {
    // TODO: implement
    float baMultiplier = (0.3f * target->getDef())/(atk) + 1.0f;
    int damage = ceil(atk*baMultiplier);
    target->receiveDamage(damage);
    if (!target->isAlive()){
        context.updateMorale();
        killsDuringTurn=true;
    }
    return damage;
}

int Nami::specialSkill(Character* target, BattleContext& context) {
    // TODO: implement
    int damage = atk + 40;
    if (energy >= 20){
        target->receiveDamage(damage);
        target->setSpeed(target->getSpeed()-10);
        energy= clamp(energy-200,0,100);
        context.busterCallTimer+=1;
        context.alarmLevel-=5;
        if (!target->isAlive()){
            killsDuringTurn=true;
            context.updateMorale();
        }
    }
    return damage;
}

int Nami::attack(Building* target, BattleContext& context) {
    // TODO: implement
    return 0;
}

int Nami::specialSkill(Building* target, BattleContext& context) {
    // TODO: implement
    return 0;
}

void Nami::endTurn(BattleContext& context) {
    // TODO: implement
    if (killsDuringTurn){
        energy= clamp (energy + 6,0,100);
        killsDuringTurn=false;
    }
}

/*
 * Chopper
 */
Chopper::Chopper(string name, int hp, int atk, int def,
                 int speed, int energy, long long bounty): StrawHat(name, hp, atk, def, speed, energy, bounty) {
    // TODO: implement
}

int Chopper::attack(Character* target, BattleContext& context) {
    // TODO: implement
    return 0;
}

int Chopper::specialSkill(Character* target, BattleContext& context) {
    // TODO: implement
    return 0;
}

void Chopper::endTurn(BattleContext& context) {
    // TODO: implement
}

/*
 * Usopp
 */
Usopp::Usopp(string name, int hp, int atk, int def,
             int speed, int energy, long long bounty): StrawHat(name, hp, atk, def, speed, energy, bounty) {
    // TODO: implement
}

int Usopp::attack(Character* target, BattleContext& context) {
    // TODO: implement
    return 0;
}

int Usopp::specialSkill(Character* target, BattleContext& context) {
    // TODO: implement
    return 0;
}

void Usopp::endTurn(BattleContext& context) {
    // TODO: implement
}

/*
 * Franky
 */
Franky::Franky(string name, int hp, int atk, int def,
               int speed, int energy, long long bounty): StrawHat(name, hp, atk, def, speed, energy, bounty) {
    // TODO: implement
}

int Franky::attack(Character* target, BattleContext& context) {
    // TODO: implement
    return 0;
}

int Franky::specialSkill(Character* target, BattleContext& context) {
    // TODO: implement
    return 0;
}

int Franky::attack(Building* target, BattleContext& context) {
    // TODO: implement
    return 0;
}

int Franky::specialSkill(Building* target, BattleContext& context) {
    // TODO: implement
    return 0;
}

void Franky::endTurn(BattleContext& context) {
    // TODO: implement
}

/*
 * CP9Agent
 */
CP9Agent::CP9Agent() : Character() {
    doriki = 0;
}

CP9Agent::CP9Agent(string name, int hp, int atk, int def,
                   int speed, int energy, int doriki) {
    // TODO: implement
}

bool CP9Agent::isCP9() const {
    // TODO: implement
}

string CP9Agent::str() const {
    // TODO: implement
    return "";
}

/*
 * Lucci
 */
Lucci::Lucci(string name, int hp, int atk, int def,
             int speed, int energy, int doriki) {
    // TODO: implement
}

int Lucci::attack(Character* target, BattleContext& context) {
    // TODO: implement
    return 0;
}

int Lucci::specialSkill(Character* target, BattleContext& context) {
    // TODO: implement
    return 0;
}

void Lucci::endTurn(BattleContext& context) {
    // TODO: implement
}

/*
 * Kaku
 */
Kaku::Kaku(string name, int hp, int atk, int def,
           int speed, int energy, int doriki) {
    // TODO: implement
}

int Kaku::attack(Character* target, BattleContext& context) {
    // TODO: implement
    return 0;
}

int Kaku::specialSkill(Character* target, BattleContext& context) {
    // TODO: implement
    return 0;
}

void Kaku::endTurn(BattleContext& context) {
    // TODO: implement
}

/*
 * Jabra
 */
Jabra::Jabra(string name, int hp, int atk, int def,
             int speed, int energy, int doriki) {
    // TODO: implement
}

int Jabra::attack(Character* target, BattleContext& context) {
    // TODO: implement
    return 0;
}

int Jabra::specialSkill(Character* target, BattleContext& context) {
    // TODO: implement
    return 0;
}

void Jabra::endTurn(BattleContext& context) {
    // TODO: implement
}

/*
 * Blueno
 */
Blueno::Blueno(string name, int hp, int atk, int def,
               int speed, int energy, int doriki) {
    // TODO: implement 
}

int Blueno::attack(Character* target, BattleContext& context) {
    // TODO: implement
    return 0;
}

int Blueno::specialSkill(Character* target, BattleContext& context) {
    // TODO: implement
    return 0;
}

void Blueno::endTurn(BattleContext& context) {
    // TODO: implement
}

/*
 * Kalifa
 */
Kalifa::Kalifa(string name, int hp, int atk, int def,
               int speed, int energy, int doriki) {
    // TODO: implement
}

int Kalifa::attack(Character* target, BattleContext& context) {
    // TODO: implement
    return 0;
}

int Kalifa::specialSkill(Character* target, BattleContext& context) {
    // TODO: implement
    return 0;
}

void Kalifa::endTurn(BattleContext& context) {
    // TODO: implement
}

/*
 * Kumadori
 */
Kumadori::Kumadori(string name, int hp, int atk, int def,
                   int speed, int energy, int doriki) {
    // TODO: implement
}

int Kumadori::attack(Character* target, BattleContext& context) {
    // TODO: implement
    return 0;
}

int Kumadori::specialSkill(Character* target, BattleContext& context) {
    // TODO: implement
    return 0;
}

void Kumadori::endTurn(BattleContext& context) {
    // TODO: implement
}

/*
 * Fukurou
 */
Fukurou::Fukurou(string name, int hp, int atk, int def,
                 int speed, int energy, int doriki) {
    // TODO: implement
}

int Fukurou::attack(Character* target, BattleContext& context) {
    // TODO: implement
    return 0;
}

int Fukurou::specialSkill(Character* target, BattleContext& context) {
    // TODO: implement
    return 0;
}

void Fukurou::endTurn(BattleContext& context) {
    // TODO: implement
}

/*
 * Building
 */
Building::Building(string name, int hp) {
    // TODO: implement
}

Building::~Building() {
    // TODO: implement if needed
}

void Building::receiveDamage(int damage) {
    // TODO: implement
}

bool Building::isDestroyed() const {
    // TODO: implement
    return destroyed;
}

void Building::onDestroyed(BattleContext& context) {
    return ;
}

string Building::getName() const {
    // TODO: implement
    return name;
}

int Building::getHP() const {
    // TODO: implement
    return hp;
}

/*
 * MainGate
 */
MainGate::MainGate(string name, int hp) : Building(name, hp) {}

void MainGate::applyEffect(BattleContext& context) {
    // TODO: implement
}

void MainGate::onDestroyed(BattleContext& context) {
    // TODO: implement
}

/*
 * Courthouse
 */
Courthouse::Courthouse(string name, int hp) : Building(name, hp) {}

void Courthouse::applyEffect(BattleContext& context) {
    // TODO: implement
}

void Courthouse::onDestroyed(BattleContext& context) {
    // TODO: implement
}

/*
 * TowerOfJustice
 */
TowerOfJustice::TowerOfJustice(string name, int hp) : Building(name, hp) {}

void TowerOfJustice::applyEffect(BattleContext& context) {
    // TODO: implement
}

/*
 * BridgeOfHesitation
 */
BridgeOfHesitation::BridgeOfHesitation(string name, int hp) : Building(name, hp) {}

void BridgeOfHesitation::applyEffect(BattleContext& context) {
    // TODO: implement
}

/*
 * BusterCallShip
 */
BusterCallShip::BusterCallShip(string name, int hp) : Building(name, hp) {}

void BusterCallShip::applyEffect(BattleContext& context) {
    // TODO: implement
}

void BusterCallShip::onDestroyed(BattleContext& context) {
    // TODO: implement
}

/*
 * EniesLobbyBattle
 */
EniesLobbyBattle::EniesLobbyBattle(const string& filename) {
    // TODO: implement
}

EniesLobbyBattle::~EniesLobbyBattle() {
    // TODO: implement
}

void EniesLobbyBattle::loadFromFile(const string& filename) {
    // TODO: implement
}

void EniesLobbyBattle::addStrawHat(Character* character) {
    // TODO: implement
}

void EniesLobbyBattle::addCP9Agent(Character* character) {
    // TODO: implement
}

void EniesLobbyBattle::addBuilding(Building* building) {
    // TODO: implement
}

void EniesLobbyBattle::buildTurnOrder() {
    // TODO: implement
}

void EniesLobbyBattle::runBattle() {
    // TODO: implement
}

void EniesLobbyBattle::processTurn(Character* character) {
    // TODO: implement
}

void EniesLobbyBattle::processBuildings() {
    // TODO: implement
}

void EniesLobbyBattle::checkEndCondition() {
    // TODO: implement
}

string EniesLobbyBattle::getResult() const {
    // TODO: implement
    return "";
}