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
    turnCount++;
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
int Character::getMaxHp() const{ return maxHp;}

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
        hp=ceil(hp*0.92);
        energy= clamp(energy - 20, 0, 100);
        if (!target->isAlive()){
            killsDuringTurn=true;
            context.updateMorale();
        }
        return damage;
    }
    return 0;
}

int Luffy::attack(Building* target, BattleContext& context) {
    // TODO: implement
    int damage = 0;
    if (getHpPercen() > 0.5){damage = atk;}
    else if (getHpPercen() > 0.3 && getHpPercen() <=0.5) {damage = ceil(atk*1.15);}
    else {damage = ceil(atk*1.3);}
    target->receiveDamage(damage);
    return damage;
}

int Luffy::specialSkill(Building* target, BattleContext& context) {
    // TODO: implement
    if (getHpPercen() >= 0.15 && energy >= 20){
        int damage = atk*2;
        target->receiveDamage(damage);
        speed += 15; 
        atk += 15;
        context.alarmLevel+=10;
        hp=ceil(hp*0.92);
        energy= clamp(energy - 20, 0, 100);
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
        damage = ceil((atk+def*0.2)*1.15);
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
            damage = ceil((atk*2.2)*1.5);
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

int Zoro::attack(Building* target, BattleContext& context) {
    // TODO: implement
    return 0;
}

int Zoro::specialSkill(Building* target, BattleContext& context) {
    // TODO: implement
    if (energy >= 15){
        int damage = 0;
        if (target->getHpPercen() < 0.5){
            damage = ceil((atk*2.2)*1.5);
        }else{
            damage = ceil(atk*2.2);
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
        damage = ceil((atk+speed*0.5)*1.1);
    }
    target->receiveDamage(damage);
    if (!target->isAlive()){
        context.updateMorale();
        killsDuringTurn=true;
    }
    return damage;
}

int Sanji::attack(Building* target, BattleContext& context){
    int damage = ceil(atk + 0.5*speed);
    target->receiveDamage(damage);
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

int Sanji::specialSkill(Building* target, BattleContext& context){
    int damage=0;
    if (energy >=18){
        damage=ceil(atk*2.1);
        target->receiveDamage(damage);
        energy = clamp(energy - 18, 0, 100);
    }
    return damage;
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
    int damage =0 ;
    if (energy >= 20){
        damage=atk + 40;
        target->receiveDamage(damage);
        target->setSpeed(target->getSpeed()-10);
        energy= clamp(energy-20,0,100);
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
    int damage=ceil(atk*0.5);
    target->receiveDamage(damage);
    return damage;
}

int Nami::specialSkill(Building* target, BattleContext& context) {
    // TODO: implement
    int damage=0;
    if (energy >= 20){
        damage=ceil((atk+40)*1.5);
        target->receiveDamage(damage);
        energy = clamp(energy-20,0,100);
        context.busterCallTimer+=1;
        context.alarmLevel-=5;
    }
    return damage;
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
    int damage=atk;
    target->receiveDamage(damage);
    if (!target->isAlive()){
        context.updateMorale();
        killsDuringTurn=true;
    }
    return damage;
}

int Chopper::attack(Building* target, BattleContext& context) {
    // TODO: implement
    int damage=atk;
    target->receiveDamage(damage);
    return damage;
}

int Chopper::specialSkill(Character* target, BattleContext& context) {
    // TODO: implement
    if (energy >= 15 && target->isStrawHat()){
        int heal = ceil(35+atk*0.5);
        target->setHp(clamp(target->getHP()+heal,0,target->getMaxHp()));
        if (target->getName() == name){
            context.updateMorale(5);
        }
        energy = clamp(energy - 15, 0, 100);
    }
    return 0;
}

void Chopper::endTurn(BattleContext& context) {
    // TODO: implement
    return;
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
    int damage;
    if (target->getSpeed() < 50) {
        damage = ceil(atk*1.2);
    }else damage=atk;
    target->receiveDamage(damage);
    if (!target->isAlive()){
        context.updateMorale();
    }
    killsDuringTurn=true; // Attacked this turn
    return damage;
}

int Usopp::attack(Building* target, BattleContext& context){
    int damage=0;
    damage=ceil(atk*0.5);
    target->receiveDamage(damage);
    killsDuringTurn=true; // Attacked this turn
    return damage;
}

int Usopp::specialSkill(Character* target, BattleContext& context) {
    // TODO: implement
    int damage=0;
    if (energy >= 16){
        damage=ceil(atk*0.8);
        target->receiveDamage(damage);
        target->setSpeed(target->getSpeed()-12);
        energy = clamp(energy-16,0,100);
        context.escapeProgress=clamp(context.escapeProgress+8,0,100);
    }
    return damage;
}

int Usopp::specialSkill(Building* target, BattleContext& context){
    int damage=0;
    if (energy>=16){
        damage=ceil(atk*0.8);
        target->receiveDamage(damage);
        energy=clamp(energy-16,0,100);
        context.escapeProgress=clamp(context.escapeProgress+8,0,100);
    }
    return damage;
}

void Usopp::endTurn(BattleContext& context) {
    // TODO: implement
    if (killsDuringTurn){
        context.updateMorale(10);
        killsDuringTurn=false;
    }
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
    int damage=ceil(atk+0.3*def);
    if (target->isCP9()){
        damage=ceil(damage*1.1);
    }
    target->receiveDamage(damage);
    if (!target->isAlive()){
        context.updateMorale();
        killsDuringTurn=true;
    }
    return damage;
}

int Franky::specialSkill(Character* target, BattleContext& context) {
    // TODO: implement
    int damage=0;
    if (energy >= 20){
        damage=ceil(atk*1.8);
        if (target->getName() == "Lucci"){
            damage=ceil(damage*1.2);
        }
        target->setSpeed(target->getSpeed()-8);
        target->receiveDamage(damage);
        energy = clamp(energy-20,0,100);
        if (!target->isAlive()){
            context.updateMorale();
            killsDuringTurn=true;
        }
    }
    return damage;
}

int Franky::attack(Building* target, BattleContext& context) {
    // TODO: implement
    int damage=ceil(atk+0.3*def);
    target->receiveDamage(damage);
    return damage;
}

int Franky::specialSkill(Building* target, BattleContext& context) {
    // TODO: implement
    int damage=0;
    if (energy >= 30){
        damage=target->getHP();
        target->receiveDamage(target->getHP());
        energy = clamp(energy-30,0,100);
    }else if (energy >= 20){
        damage=ceil(atk*1.8);
        target->receiveDamage(damage);
        energy = clamp(energy-20,0,100);
    }
    return damage;
}

void Franky::endTurn(BattleContext& context) {
    // TODO: implement
    if (getHpPercen() > 0.7){
        def+=5;
    }else if (getHpPercen() < 0.3){
        atk=ceil(atk*1.1);
    }
    killsDuringTurn=false;
}

/*
 * CP9Agent
 */
CP9Agent::CP9Agent() : Character() {
    doriki = 0;
}

CP9Agent::CP9Agent(string name, int hp, int atk, int def,
                   int speed, int energy, int doriki): Character(name, hp, atk, def, speed, energy), doriki(doriki) {
    // TODO: implement
}

bool CP9Agent::isCP9() const {
    // TODO: implement
    return true;
}

string CP9Agent::str() const {
    // TODO: implement
    stringstream ss;
    ss << "CP9Agent [ name =" << name << ", hp = " << hp << ", atk = " << atk
       << ", def = " << def << ", speed = " << speed
       << ", energy = " << energy << ", doriki = " << doriki << " ]";
    return ss.str();
}

/*
 * Lucci
 */
Lucci::Lucci(string name, int hp, int atk, int def,
             int speed, int energy, int doriki): CP9Agent(name, hp, atk, def, speed, energy, doriki) {
    // TODO: implement
}

int Lucci::attack(Character* target, BattleContext& context) {
    // TODO: implement
    int damage = atk + ceil(doriki/12.0f);
    if (target->getHpPercen() < 0.5){
        damage = ceil(damage*1.2);
    }
    target->receiveDamage(damage);
    if (!target->isAlive()){
        context.updateMorale(-5);
    }
    return damage;
}

int Lucci::specialSkill(Character* target, BattleContext& context) {
    // TODO: implement
    int damage=0;
    if (energy >= 25){
        damage=ceil(atk*2.8);
        float skillMultiplier=ceil((0.5f*target->getDef())/(damage*1.0f) + 1.0f);
        damage=ceil(damage*skillMultiplier);
        target->receiveDamage(damage);
        energy=clamp(energy-25,0,100);
        if (!target->isAlive()){
            context.updateMorale(-15);
        }
    }
    return damage;
}

void Lucci::endTurn(BattleContext& context) {
    // TODO: implement
    if (getHpPercen() < 0.4){
        atk=ceil(atk*1.05);
    }
}

/*
 * Kaku
 */
Kaku::Kaku(string name, int hp, int atk, int def,
           int speed, int energy, int doriki) : CP9Agent(name, hp, atk, def, speed, energy, doriki) {
    // TODO: implement
}

int Kaku::attack(Character* target, BattleContext& context) {
    // TODO: implement
    int damage=atk;
    target->receiveDamage(damage);
    if (!target->isAlive()){
        context.updateMorale(-5);
    }
    return damage;
}

int Kaku::specialSkill(Character* target, BattleContext& context) {
    // TODO: implement
    int damage1,damage2,damage3;
    if (energy >=20){
        damage1=ceil(atk*1.2);
        damage2=ceil(atk);
        damage3=ceil(atk*0.8);
        energy = clamp(energy-20,0,100);
        target->receiveDamage(damage1);
        if (!target->isAlive()){
            context.updateMorale(-5);
            return damage1;
        }
        target->receiveDamage(damage2);
        if (!target->isAlive()){
            context.updateMorale(-5);
            return damage1+damage2;
        }
        target->receiveDamage(damage3);
        if (!target->isAlive()){
            context.updateMorale(-5);
            return damage1+damage2+damage3;
        }
    }
    return 0;
}

void Kaku::endTurn(BattleContext& context) {
    // TODO: implement
    return;
}

/*
 * Jabra
 */
Jabra::Jabra(string name, int hp, int atk, int def,
             int speed, int energy, int doriki): CP9Agent(name, hp, atk, def, speed, energy, doriki) {
    // TODO: implement
}

int Jabra::attack(Character* target, BattleContext& context) {
    // TODO: implement
    target->receiveDamage(atk);
    if (!target->isAlive()){
        context.updateMorale(-5);
    }
    return atk;
}

int Jabra::specialSkill(Character* target, BattleContext& context) {
    // TODO: implement
    int damage=0;
    if (energy>=18){
        damage=ceil(atk*1.5);
        if (getHpPercen() < 0.3){
            damage=ceil(atk*1.5*1.25);
        }
        target->receiveDamage(damage);
        if (!target->isAlive()){
            context.updateMorale(-10);
        }
        energy = clamp(energy-18,0,100);
    }
    return damage;
}

void Jabra::endTurn(BattleContext& context) {
    // TODO: implement
    return;
}

/*
 * Blueno
 */
Blueno::Blueno(string name, int hp, int atk, int def,
               int speed, int energy, int doriki): CP9Agent(name, hp, atk, def, speed, energy, doriki) {
    // TODO: implement 
}

int Blueno::attack(Character* target, BattleContext& context) {
    // TODO: implement
    target->receiveDamage(atk);
    if (!target->isAlive()){
        context.updateMorale(-5);
    }
    return atk;
}

int Blueno::specialSkill(Character* target, BattleContext& context) {
    // TODO: implement
    int damage=0;
    if (energy >= 15){
        damage=ceil(atk*1.3);
        if (getHpPercen() > 0.5){
            damage+=20;
        }else{ damage+=40;}
        target->receiveDamage(damage);
        if (!target->isAlive()){
            context.updateMorale(-5);
        }
        energy = clamp(energy-15,0,100);
    }
    return damage;
}

void Blueno::endTurn(BattleContext& context) {
    // TODO: implement
    return;
}

/*
 * Kalifa
 */
Kalifa::Kalifa(string name, int hp, int atk, int def,
               int speed, int energy, int doriki): CP9Agent(name, hp, atk, def, speed, energy, doriki) {
    // TODO: implement
}

int Kalifa::attack(Character* target, BattleContext& context) {
    // TODO: implement
    target->receiveDamage(atk);
    if (!target->isAlive()){
        context.updateMorale(-5);
    }
    return atk;
}

int Kalifa::specialSkill(Character* target, BattleContext& context) {
    // TODO: implement
    int damage=0;
    if (energy >= 18){
        damage=ceil(atk*1.4);
        target->receiveDamage(damage);
        if (!target->isAlive()){
            context.updateMorale(-5);
        }
        if (target->getName() == "Nami"){
            context.updateMorale(-12);
        }else context.updateMorale(-8);
        target->setSpeed(target->getSpeed()-6);
        energy = clamp(energy-18,0,100);
    }
    return damage;
}

void Kalifa::endTurn(BattleContext& context) {
    // TODO: implement
    return;
}

/*
 * Kumadori
 */
Kumadori::Kumadori(string name, int hp, int atk, int def,
                   int speed, int energy, int doriki): CP9Agent(name, hp, atk, def, speed, energy, doriki) {
    // TODO: implement
}

int Kumadori::attack(Character* target, BattleContext& context) {
    // TODO: implement
    target->receiveDamage(atk);
    if (!target->isAlive()){
        context.updateMorale(-5);
    }
    return atk;
}

int Kumadori::specialSkill(Character* target, BattleContext& context) {
    // TODO: implement
    int damage=0;
    if (energy >=16 ){
        if (getHpPercen() < 0.4 ) damage=ceil(30 + doriki*0.1 + 25);
        else damage=ceil(30 + doriki*0.1);
        target->receiveDamage(damage);
        if (!target->isAlive()){
            context.updateMorale(-5);
        }
        energy = clamp(energy-16,0,100);
    }
    return damage;
}

void Kumadori::endTurn(BattleContext& context) {
    // TODO: implement
    return;
}

/*
 * Fukurou
 */
Fukurou::Fukurou(string name, int hp, int atk, int def,
                 int speed, int energy, int doriki): CP9Agent(name, hp, atk, def, speed, energy, doriki) {
    // TODO: implement
}

int Fukurou::attack(Character* target, BattleContext& context) {
    // TODO: implement
    target->receiveDamage(atk);
    if (!target->isAlive()){
        context.updateMorale(-5);
    }
    return atk;
}

int Fukurou::specialSkill(Character* target, BattleContext& context) {
    // TODO: implement
    int damage=0;
    if (energy >= 14){
        damage=ceil(atk*1.3);
        target->receiveDamage(damage);
        if (!target->isAlive()){
            context.updateMorale(-11);
        }
        energy=clamp(energy-14,0,100);
    }
    return damage;
}

void Fukurou::endTurn(BattleContext& context) {
    // TODO: implement
    return;
}

/*
 * Building
 */
Building::Building(string name, int hp): name(name), hp(hp), destroyed(false) {
    // TODO: implement
    maxHP=hp;
}

Building::~Building() {
    // TODO: implement if needed
}

void Building::receiveDamage(int damage) {
    // TODO: implement
    hp -= damage;
    if (hp <= 0) {
        hp = 0;
        destroyed = true;
    };
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

string Building::str() const{
    stringstream ss;
    ss << std::boolalpha << "Building[name=" << name << ", hp=" << hp << ", maxHP=" << maxHP << ", destroyed=" << destroyed << "]";
    return ss.str();
}

/*
 * MainGate
 */
MainGate::MainGate(string name, int hp) : Building(name, hp) {}

void MainGate::applyEffect(BattleContext& context) {
    // TODO: implement
    return;
}

void MainGate::onDestroyed(BattleContext& context) {
    // TODO: implement
    if (isDestroyed()){
        context.mainGateDestroyed=true;
        context.updpateRescueProgress(20);
        context.updateMorale(5);
    }
}

/*
 * Courthouse
 */
Courthouse::Courthouse(string name, int hp) : Building(name, hp) {}

void Courthouse::applyEffect(BattleContext& context) {
    // TODO: implement
    if (!isDestroyed()){
        context.updateAlarmLevel(5);
    }
}

void Courthouse::onDestroyed(BattleContext& context) {
    // TODO: implement
    if (isDestroyed()){
        context.updateAlarmLevel(-20);
    }
}

/*
 * TowerOfJustice
 */
TowerOfJustice::TowerOfJustice(string name, int hp) : Building(name, hp) {}

void TowerOfJustice::applyEffect(BattleContext& context) {
    // TODO: implement
    if (context.mainGateDestroyed && !context.robinRescued){
        context.updpateRescueProgress(5);
    }
    if (context.rescueProgress >= 100){
        context.robinRescued=true;
        context.updateMorale(10);
    }
}

/*
 * BridgeOfHesitation
 */
BridgeOfHesitation::BridgeOfHesitation(string name, int hp) : Building(name, hp) {}

void BridgeOfHesitation::applyEffect(BattleContext& context) {
    // TODO: implement
    if (context.robinRescued){
        context.bridgeOpened=true;
    }
    if (context.bridgeOpened){
        context.updateEscapeProgress(5);
    }
}

/*
 * BusterCallShip
 */
BusterCallShip::BusterCallShip(string name, int hp) : Building(name, hp) {}

void BusterCallShip::applyEffect(BattleContext& context) {
    // TODO: implement
    if (!isDestroyed()){
        context.busterCallTimer--;
    }
    if (context.busterCallTimer <= 0){
        context.busterCallTimer=0;
    }

}

void BusterCallShip::onDestroyed(BattleContext& context) {
    // TODO: implement
    if (isDestroyed()){
        context.busterCallTimer+=3;
    }
}

/*
 * EniesLobbyBattle
 */
EniesLobbyBattle::EniesLobbyBattle(const string& filename) {
    // TODO: implement
    strawHats = new Character*[7];
    for (Character** p=strawHats, **pE=strawHats+7; p!=pE; p++){
        *p=nullptr;
    }
    cp9Agents = new Character*[7];
    for (Character** p=cp9Agents, **pE=cp9Agents+7; p!=pE; p++){
        *p=nullptr;
    }
    buildings = new Building*[5];
    for (Building** p=buildings, **pE=buildings+5; p!=pE; p++){
        *p=nullptr;
    }
    maxTurns=0;
    loadFromFile(filename);
    strawHatCount=cp9Count=buildingCount=0;
    for (Character** p=strawHats;p!=strawHats+7; ++p){
        if (*p!=nullptr){
            strawHatCount++;
        }
    }
    for (Character** p=cp9Agents;p!=cp9Agents+7; ++p){
        if(*p!=nullptr){
            cp9Count++;
        }
    }
    for (Building** p=buildings;p!=buildings+5; ++p){
        if(*p!=nullptr){
            buildingCount++;
        }
    }
}

EniesLobbyBattle::~EniesLobbyBattle() {
    // TODO: implement
    for (int i=0;i<7;++i){
        delete strawHats[i];
        delete cp9Agents[i];
    }
    for (int i=0;i<5;++i){
        delete buildings[i];
    }
    delete[] strawHats;
    delete[] cp9Agents;
    delete[] buildings;
    TurnNode* curr=turnOrder;
    while (curr != nullptr){
        TurnNode* next=curr->next;
        delete curr;
        curr = next;
    }
}

void EniesLobbyBattle::loadFromFile(const string& filename) {
    // TODO: implement
    fstream file;
    file.open(filename, fstream::in);
    string indicatorInput="";
    if (!file.is_open()) {return;}
    while (file >> indicatorInput){
        if (indicatorInput=="CONTEXT"){
            int *contextVal=new int[6];
            for (int *p=contextVal; p!=contextVal+6;p++){
                file >> *p;
            }
            context.morale=*(contextVal);
            context.alarmLevel=*(contextVal+1);
            context.rescueProgress=*(contextVal+2);
            context.escapeProgress=*(contextVal+3);
            context.busterCallTimer=*(contextVal+4);
            maxTurns=*(contextVal+5);
            delete[] contextVal;
        }
        else if (indicatorInput=="STRAW_HAT"){
            string name;
            file >> name;
            if (name=="Luffy"){
                int hp,atk,def,speed,energy;
                long long bounty;
                file >> hp >> atk >> def >> speed >> energy;
                file >> bounty;
                Luffy* luffyPtr=new Luffy(name, hp, atk, def, speed, energy, bounty);
                addStrawHat(luffyPtr);
            }else if(name=="Zoro"){
                int hp,atk,def,speed,energy;
                long long bounty;
                file >> hp >> atk >> def >> speed >> energy;
                file >> bounty;
                Zoro* zoroPtr=new Zoro(name, hp, atk, def, speed, energy, bounty);
                addStrawHat(zoroPtr);
            }else if(name=="Sanji"){
                int hp,atk,def,speed,energy;
                long long bounty;
                file >> hp >> atk >> def >> speed >> energy;
                file >> bounty;
                Sanji* sanjiPtr=new Sanji(name, hp, atk, def, speed, energy, bounty);
                addStrawHat(sanjiPtr);
            }else if(name=="Nami"){
                int hp,atk,def,speed,energy;
                long long bounty;
                file >> hp >> atk >> def >> speed >> energy;
                file >> bounty;
                Nami* namiPtr=new Nami(name, hp, atk, def, speed, energy, bounty);
                addStrawHat(namiPtr);
            }else if(name=="Chopper"){
                int hp,atk,def,speed,energy;
                long long bounty;
                file >> hp >> atk >> def >> speed >> energy;
                file >> bounty;
                Chopper* chopperPtr=new Chopper(name, hp, atk, def, speed, energy, bounty);
                addStrawHat(chopperPtr);
            }else if(name=="Usopp"){
                int hp,atk,def,speed,energy;
                long long bounty;
                file >> hp >> atk >> def >> speed >> energy;
                file >> bounty;
                Usopp* usoppPtr=new Usopp(name, hp, atk, def, speed, energy, bounty);
                addStrawHat(usoppPtr);
            }else if(name=="Franky"){
                int hp,atk,def,speed,energy;
                long long bounty;
                file >> hp >> atk >> def >> speed >> energy;
                file >> bounty;
                Franky* frankyPtr=new Franky(name, hp, atk, def, speed, energy, bounty);
                addStrawHat(frankyPtr);
            }
        }
        else if (indicatorInput=="CP9"){
            string name;
            file >> name;
            if (name=="Lucci"){
                int hp,atk,def,speed,energy,doriki;
                file >> hp >> atk >> def >> speed >> energy >> doriki;
                Lucci* ptr=new Lucci(name,hp,atk,def,speed,energy,doriki);
                addCP9Agent(ptr);
            }else if(name=="Kaku"){
                int hp,atk,def,speed,energy,doriki;
                file >> hp >> atk >> def >> speed >> energy >> doriki;
                Kaku* ptr=new Kaku(name,hp,atk,def,speed,energy,doriki);
                addCP9Agent(ptr);
            }else if(name=="Jabra"){
                int hp,atk,def,speed,energy,doriki;
                file >> hp >> atk >> def >> speed >> energy >> doriki;
                Jabra* ptr=new Jabra(name,hp,atk,def,speed,energy,doriki);
                addCP9Agent(ptr);
            }else if(name=="Blueno"){
                int hp,atk,def,speed,energy,doriki;
                file >> hp >> atk >> def >> speed >> energy >> doriki;
                Blueno* ptr=new Blueno(name,hp,atk,def,speed,energy,doriki);
                addCP9Agent(ptr);
            }else if(name=="Kalifa"){
                int hp,atk,def,speed,energy,doriki;
                file >> hp >> atk >> def >> speed >> energy >> doriki;
                Kalifa* ptr=new Kalifa(name,hp,atk,def,speed,energy,doriki);
                addCP9Agent(ptr);
            }else if(name=="Kumadori"){
                int hp,atk,def,speed,energy,doriki;
                file >> hp >> atk >> def >> speed >> energy >> doriki;
                Kumadori* ptr=new Kumadori(name,hp,atk,def,speed,energy,doriki);
                addCP9Agent(ptr);
            }else if(name=="Fukurou"){
                int hp,atk,def,speed,energy,doriki;
                file >> hp >> atk >> def >> speed >> energy >> doriki;
                Fukurou* ptr=new Fukurou(name,hp,atk,def,speed,energy,doriki);
                addCP9Agent(ptr);
            }
        }
        else if (indicatorInput=="BUILDING"){
            string name;
            file >> name;
            if (name=="MainGate"){
                int hp;
                file >> hp;
                MainGate* ptr=new MainGate(name,hp);
                addBuilding(ptr);
            }else if(name=="Courthouse"){
                int hp;
                file >> hp;
                Courthouse* ptr=new Courthouse(name,hp);
                addBuilding(ptr);
            }else if(name=="TowerOfJustice"){
                int hp;
                file >> hp;
                TowerOfJustice* ptr=new TowerOfJustice(name,hp);
                addBuilding(ptr);
            }else if(name=="BridgeOfHesitation"){
                int hp;
                file >> hp;
                BridgeOfHesitation* ptr=new BridgeOfHesitation(name,hp);
                addBuilding(ptr);
            }else if(name=="BusterCallShip"){
                int hp;
                file >> hp;
                BusterCallShip* ptr=new BusterCallShip(name,hp);
                addBuilding(ptr);
            }
        }
    }
    file.close();
    buildTurnOrder();
}

void EniesLobbyBattle::addStrawHat(Character* character) {
    // TODO: implement
    if (character==nullptr) {return;}
    for (Character** p=strawHats,**pE=strawHats+7;p!=pE;p++){
        if (*p==nullptr){
            *p=character;
            return;
        }
    }
}

void EniesLobbyBattle::addCP9Agent(Character* character) {
    // TODO: implement
    if (character==nullptr) {return;}
    for (Character** p=cp9Agents,**pE=cp9Agents+7;p!=pE;p++){
        if (*p==nullptr){
            *p=character;
            return;
        }
    }
}

void EniesLobbyBattle::addBuilding(Building* building) {
    // TODO: implement
    if (building==nullptr) {return;}
    for (Building** p=buildings,**pE=buildings+5;p!=pE;p++){
        if (*p==nullptr){
            *p=building;
            return;
        }
    }
}


TurnNode* getNodebeforeTail(TurnNode* head){
    TurnNode* ite=head;
    while (ite->next!=nullptr){
        ite=ite->next;
    }
    return ite;
}
void EniesLobbyBattle::buildTurnOrder() {
    // TODO: implement
    TurnNode* head = new TurnNode;
    TurnNode* tail = nullptr;
    head->next=tail;
    for (Character** p=strawHats; p!=strawHats+7; ++p){
        if (*p==nullptr) continue;
        TurnNode* temp=getNodebeforeTail(head);
        temp->next=new TurnNode;
        temp=temp->next;
        temp->data=*p;
        temp->next=tail;
    }
    for (Character** p=cp9Agents; p!=cp9Agents+7; ++p){
        if (*p==nullptr) continue;
        TurnNode* temp=getNodebeforeTail(head);
        temp->next=new TurnNode;
        temp=temp->next;
        temp->data=*p;
        temp->next=tail;
    }
    turnOrder=head->next;
    delete head;
}

void EniesLobbyBattle::runBattle() {
    // TODO: implement
    
}

void EniesLobbyBattle::processTurn(Character* character) {
    // TODO: implement
}

void EniesLobbyBattle::processBuildings() {
    // TODO: implement
    for (Building** p=buildings; p!=buildings+5;++p){
        (*p)->applyEffect(context);
    }
}

void EniesLobbyBattle::checkEndCondition() {
    // TODO: implement
    if (context.robinRescued && context.escapeProgress >= 100){
        context.resultCode = "STRAW_HAT_WIN";
        context.battleEnded=true;
    }else if (context.busterCallTimer <= 0){
        context.resultCode = "BUSTER_CALL";
        context.battleEnded=true;
    }else if (strawHatCount==0){
        context.resultCode="CP9_WIN";
        context.battleEnded=true;
    }else if (cp9Count==0){
        context.resultCode="STRAW_HAT_WIN_BY_DEFEAT_CP9";
        context.battleEnded=true;
    }else if (context.turnCount >= maxTurns){
        context.resultCode = "TIME_OUT";
        context.battleEnded=true;
    }
}

string EniesLobbyBattle::getResult() const {
    // TODO: implement
    stringstream ss;
    ss << context.resultCode << " " << context.turnCount << " " << context.morale << " " << context.alarmLevel << " "
     << context.rescueProgress << " " << context.escapeProgress << " " << context.busterCallTimer;

    return ss.str();
}