#include "GameObjects.h"

using namespace std;

//----------------------------class Essence----------------------------

Essence::Essence(ObjectType type) : appearance_(type) {}

ObjectType Essence::GetAppearance() const {
    return appearance_;
}

Essence::~Essence() = default;

//----------------------------class Character----------------------------

Character::Character(int health, int strength, int armor) : health_(health), strength_(strength), armor_(armor) {}

void Character::Attack(Character* opponent) {
    opponent->SetHealth(HEALTH_FORMULA(opponent->GetHealth(), strength_, opponent->GetArmor()));
    opponent->SetArmor(ARMOR_FORMULA(strength_, opponent->GetArmor()));
}

void Character::SetArmor(int newArmor) {
    armor_ = newArmor;
}

void Character::SetStrength(int newStrength) {
    strength_ = newStrength;
}

void Character::SetHealth(int newHealth) {
    health_ = newHealth;
}

int Character::GetStrength() const {
    return strength_;
}

int Character::GetArmor() const {
    return armor_;
}

int Character::GetHealth() const {
    return health_;
}

Character::~Character() = default;

//----------------------------class Hero----------------------------

Hero::Hero() : Character(HERO_HEALTH, HERO_STRENGTH, HERO_ARMOR), score_(0) {}

void Hero::SetScore(int newScore) {
    score_ = newScore;
}

int Hero::GetScore() const {
    return score_;
}

//----------------------------class Enemy----------------------------

Enemy::Enemy(int health, int strength, int armor, EnemyType enemyType) : Character(health, strength, armor),
        Essence(ObjectType::Enemy), enemyType_(enemyType) {}

EnemyType Enemy::GetEnemyType() const {
    return enemyType_;
}

//----------------------------class Item----------------------------

Item::Item(int effectHealth, int effectStrength, int effectArmor, ItemType itemType) : Essence(ObjectType::Item),
        itemType_(itemType), health_(effectHealth), strength_(effectStrength), armor_(effectArmor) {}

void Item::UseItem(Character* character) {
    character->SetHealth(character->GetHealth() + health_);
    character->SetStrength(character->GetStrength() + strength_);
    character->SetArmor(character->GetArmor() + armor_);
}

ItemType Item::GetItemType() const {
    return itemType_;
}

int Item::GetHealth() const {
    return health_;
}

int Item::GetStrength() const {
    return strength_;
}

int Item::GetArmor() const {
    return armor_;
}


//----------------------------class Stage----------------------------

Stage::Stage(Hero* player, int stageNumber) : player_(player), stageNumber_(stageNumber) {
    switch (ObjectTypeRandomSelector(stageNumber))
    {
        case ObjectType::Enemy:
            EnemyCreator(stageNumber);
            break;
        case ObjectType::Item:
            ItemCreator(stageNumber);
            break;
        case ObjectType::None:
            VoidCreator(stageNumber);
            break;
        default:
            break; // throw
    }
}

ObjectType Stage::StageObjectType() {
    return stageObject_->GetAppearance(); // throw
}

bool Stage::StageIsClear() const {
    return !stageObject_ || stageObject_->GetAppearance() == ObjectType::None; 
}

Essence* Stage::GetStageObject() const {
    return stageObject_.get();
}

void Stage::Fight() {
    Enemy* convertedToEnemy = dynamic_cast<Enemy*>(stageObject_.get());
    player_->Attack(convertedToEnemy); // throw
    if (convertedToEnemy->GetHealth() <= 0) {   
        player_->SetScore(player_->GetScore() + static_cast<int>(convertedToEnemy->GetEnemyType()));
        if (convertedToEnemy->GetEnemyType() == EnemyType::Hard_enemy) {
            this->stageObject_.reset();
            ItemCreator(GetStageNumber());
        } else {
            this->stageObject_.reset();
        }
        return;
    }
    convertedToEnemy->Attack(player_); // throw
}

int Stage::GetStageNumber() const {
    return stageNumber_;
}

void Stage::UseItem() {
    Item* convertedToItem = dynamic_cast<Item*>(stageObject_.get());
    convertedToItem->UseItem(player_);
    player_->SetScore(player_->GetScore() + static_cast<int>(convertedToItem->GetItemType()));
    this->stageObject_.reset();
}

ObjectType Stage::ObjectTypeRandomSelector(int stageNumber) {
    int randomValue = RandomNumberGenerator(100);

    if (0 < randomValue && randomValue <= CHANCE_SPAWN_ENEMY(stageNumber)) { // spawn enemy
        return ObjectType::Enemy;
    } else if (CHANCE_SPAWN_ENEMY(stageNumber) < randomValue &&
            randomValue <= CHANCE_SPAWN_ENEMY(stageNumber) + CHANCE_SPAWN_ITEM(stageNumber)) { // spawn item
        return ObjectType::Item;
    }
    else return ObjectType::None; // void
}

void Stage::EnemyCreator(int stageNumber) {
    int randomValue = RandomNumberGenerator(100);
    
    if (0 < randomValue && randomValue <= CHANCE_WEAK_ENEMY(stageNumber)) {
        stageObject_ = make_unique<Enemy>(WEAK_ENEMY_HEALTH(stageNumber), WEAK_ENEMY_STRENGTH(stageNumber),
                WEAK_ENEMY_ARMOR(stageNumber), EnemyType::Weak_enemy);
    } else if (CHANCE_WEAK_ENEMY(stageNumber) < randomValue &&
            randomValue <= CHANCE_WEAK_ENEMY(stageNumber)+CHANCE_MEDIUM_ENEMY(stageNumber)) {
        stageObject_ = make_unique<Enemy>(MEDIUM_ENEMY_HEALTH(stageNumber), MEDIUM_ENEMY_STRENGTH(stageNumber), 
                MEDIUM_ENEMY_ARMOR(stageNumber), EnemyType::Medium_enemy);
    } else {
        stageObject_ = make_unique<Enemy>(HARD_ENEMY_HEALTH(stageNumber), HARD_ENEMY_STRENGTH(stageNumber),
                HARD_ENEMY_ARMOR(stageNumber), EnemyType::Hard_enemy);
    }
}

void Stage::ItemCreator(int stageNumber) {
    int randomValue = RandomNumberGenerator(100);
    
    if (0 < randomValue && randomValue <= CHANCE_GOOD_POTION(stageNumber)) {
        stageObject_ = make_unique<Item>(GOOD_POTION_HEALTH(stageNumber), GOOD_POTION_STRENGTH(stageNumber),
                GOOD_POTION_ARMOR(stageNumber), ItemType::Potion_good);
    } else {
        stageObject_ = make_unique<Item>(EVIL_POTION_HEALTH(stageNumber), EVIL_POTION_STRENGTH(stageNumber),
                EVIL_POTION_ARMOR(stageNumber), ItemType::Potion_evil);
    }
}

void Stage::VoidCreator(int stageNumber) {
    stageObject_ = make_unique<Essence>(ObjectType::None);
}

int Stage::RandomNumberGenerator(int maxValue) {
    std::random_device rd;
    std::mt19937 gen(rd());
    return gen() % maxValue + 1;
}

//----------------------------class Game----------------------------

Game::Game() : player_(make_unique<Hero>()), stageNumber_(1),
        stage_(make_unique<Stage>(player_.get(), stageNumber_)), countOfSkip_(START_COUNT_SKIP),
        stagesForLoadSkip_(SKIP_STAGE_RELOAD) {}

Game::~Game() {
    stage_.reset();
    player_.reset();
}

int Game::GetStageNumber() const {
    return stageNumber_;
}

void Game::NextStage() {
    stage_.reset();
    player_->SetScore(player_->GetScore() + SCORE_FOR_STAGE);
    stageNumber_++;
    SetStagesForLoadSkip(GetStagesForLoadSkip() - 1);
    if (GetStagesForLoadSkip() == 0) {
        SetStagesForLoadSkip(SKIP_STAGE_RELOAD);
        SetCountOfSkip(GetCountOfSkip() + 1);
    }
    stage_ = make_unique<Stage>(player_.get(), stageNumber_);
}

const InfoPackage Game::GetHeroAndGameInfo() const {
    return {player_->GetHealth(), player_->GetStrength(), player_->GetArmor()};
}

const InfoPackage Game::GetStageInfo() const {
    const Enemy* convertedToEnemy = dynamic_cast<Enemy*>(stage_->GetStageObject());
    const Item* convertedToItem = dynamic_cast<Item*>(stage_->GetStageObject());
    if (convertedToEnemy) {
        return {convertedToEnemy->GetHealth(), convertedToEnemy->GetStrength(),
                convertedToEnemy->GetArmor(), ObjectType::Enemy, ItemType::None,
                convertedToEnemy->GetEnemyType()};
    } else if (convertedToItem) {
        return {convertedToItem->GetHealth(), convertedToItem->GetStrength(),
                convertedToItem->GetArmor(), ObjectType::Item, convertedToItem->GetItemType(),
                EnemyType::None};
    } else {
        return {};
    }
}

void Game::FightWithEnemy() {
    stage_->Fight();
    if (stage_->StageIsClear()) this->NextStage();
}

void Game::UseItem() {
    stage_->UseItem();
}

bool Game::TryNextStage() {
    if (stage_->StageIsClear()) {
        this->NextStage();
        return true;
    } else {
        return false;
    }
}

int Game::GetScore() const {
    return player_->GetScore();
}

bool Game::GameIsOver() {
    if (player_->GetHealth() <= 0) {
        return true;
    }
    else return false;
}

void Game::SkipStage() {
    stage_.reset();
    stageNumber_++;
    SetCountOfSkip(GetCountOfSkip() - 1);
    stage_ = make_unique<Stage>(player_.get(), stageNumber_);
}

bool Game::TrySkipStage() {
    if (GetCountOfSkip() > 0) {
        SkipStage();
        return true;
    } else {
        return false;
    }
}

int Game::GetCountOfSkip() const {
    return countOfSkip_;
}

void Game::SetCountOfSkip(int newCountOfSkip) {
    countOfSkip_ = newCountOfSkip;
}

void Game::SetStagesForLoadSkip(int newStageForLoadSkip) {
    stagesForLoadSkip_ = newStageForLoadSkip;
}

int Game::GetStagesForLoadSkip() const {
    return stagesForLoadSkip_;
}
