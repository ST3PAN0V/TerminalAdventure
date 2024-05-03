#pragma once

#include <vector>
#include <string>
#include <random>
#include <memory>
#include <unordered_map>

// score
#define SCORE_FOR_STAGE 5

// chance
#define CHANCE_SPAWN_ENEMY(stage) (20 + stage/5)
#define CHANCE_SPAWN_ITEM(stage) (100) / (15 + stage/7)
#define CHANCE_WEAK_ENEMY(stage) (38)
#define CHANCE_MEDIUM_ENEMY(stage) (34)
#define CHANCE_HARD_ENEMY(stage) (28)
#define CHANCE_GOOD_POTION(stage) (60)
#define CHANCE_EVIL_POTION(stage) (40)

// characteristics
#define WEAK_ENEMY_HEALTH(stage) (10 + stage/5)
#define WEAK_ENEMY_STRENGTH(stage) (1 + stage/15)
#define WEAK_ENEMY_ARMOR(stage) (0)

#define MEDIUM_ENEMY_HEALTH(stage) (15 + stage/4)
#define MEDIUM_ENEMY_STRENGTH(stage) (2 + stage/13)
#define MEDIUM_ENEMY_ARMOR(stage) (0 + stage/25)

#define HARD_ENEMY_HEALTH(stage) (20 + stage/3)
#define HARD_ENEMY_STRENGTH(stage) (3 + stage/11)
#define HARD_ENEMY_ARMOR(stage) (3 + stage/25*2)

#define GOOD_POTION_HEALTH(stage) (10 + stage/5*2)
#define GOOD_POTION_STRENGTH(stage) (1 + stage/15)
#define GOOD_POTION_ARMOR(stage) (1 + stage/20)

#define EVIL_POTION_HEALTH(stage) ((10 + stageNumber/5)*(-1))
#define EVIL_POTION_STRENGTH(stage) ((1 + stageNumber/20)*(-1))
#define EVIL_POTION_ARMOR(stage) (0)

#define HERO_HEALTH 100
#define HERO_STRENGTH 3
#define HERO_ARMOR 10

// armor calculation formula
#define ARMOR_FORMULA(damage, armor) (damage > 0 ? static_cast<int>(armor*0.7) : armor)
#define HEALTH_FORMULA(xp, damage, armor) (xp - (armor > 0 ? (damage - ((armor/2)+1)) : (damage)))

// skip stage
#define START_COUNT_SKIP 3
#define SKIP_STAGE_RELOAD 10

//----------------------------forward declaration----------------------------

class Essence;
class Character;
class Hero;
class Enemy;
class Item;
class Stage;
class Game;

//---------------------------------------------------------------------------

enum class ObjectType {
    Enemy = 1,
    Item = 2,
    None = 0
};

enum class EnemyType {
    Weak_enemy = 8,
    Medium_enemy = 12,
    Hard_enemy = 20,
    None = 0
};

enum class ItemType {
    Potion_evil = 10,
    Potion_good = 7,
    None = 0
};

struct InfoPackage {
    int health = 0;
    int strenght = 0;
    int armor = 0;
    ObjectType objectType = ObjectType::None;
    ItemType itemType = ItemType::None;
    EnemyType enemyType = EnemyType::None;
};

class Essence {
public:
    Essence(ObjectType type);
    ObjectType GetAppearance() const;

    virtual ~Essence();

private:
    ObjectType appearance_;

};

class Character {
public:
    Character(int health, int strength, int armor);
    void Attack(Character* opponent);

    void SetArmor(int newArmor);
    void SetStrength(int newStrength);
    void SetHealth(int newHealth);

    int GetStrength() const;
    int GetArmor() const;
    int GetHealth() const;

    virtual ~Character();
    
private:
    int health_;
    int strength_;
    int armor_;

};

class Hero : public Character{
public:
    Hero();

    void SetScore(int newScore);

    int GetScore() const;

private:
    int score_;
};

class Enemy : public Essence, public Character {
public:
    Enemy(int health, int strength, int armor, EnemyType enemyType);

    EnemyType GetEnemyType() const;

private:
    EnemyType enemyType_;

};

class Item : public Essence {
public:
    Item(int effectHealth, int effectStrength, int effectArmor, ItemType itemType);

    void UseItem(Character* character);

    ItemType GetItemType() const;
    int GetHealth() const;
    int GetStrength() const;
    int GetArmor() const;

private:
    ItemType itemType_;
    int health_;
    int strength_;
    int armor_;
};

class Stage {
public:
    Stage(Hero* player, int stageNumber);

    ObjectType StageObjectType();
    void Fight();
    void UseItem();

    bool StageIsClear() const;
    int GetStageNumber() const;
    Essence* GetStageObject() const;

private:
    std::unique_ptr<Essence> stageObject_;
    Hero* player_;
    int stageNumber_;
    
    ObjectType ObjectTypeRandomSelector(int stageNumber);
    void EnemyCreator(int stageNumber);
    void ItemCreator(int stageNumber);
    void VoidCreator(int stageNumber);
    int RandomNumberGenerator(int maxValue);
};

class Game {
public:
    Game();
    ~Game();

    void FightWithEnemy();
    void UseItem();
    bool TryNextStage();
    bool TrySkipStage();
    bool GameIsOver();
    void SetCountOfSkip(int newCountOfSkip);
    void SetStagesForLoadSkip(int newStagesForLoadSkip);

    const InfoPackage GetHeroAndGameInfo() const;
    const InfoPackage GetStageInfo() const;
    int GetStageNumber() const;
    int GetScore() const;
    int GetCountOfSkip() const;
    int GetStagesForLoadSkip() const;

private:
    std::unique_ptr<Hero> player_;
    std::unique_ptr<Stage> stage_;
    int stageNumber_;
    int countOfSkip_;
    int stagesForLoadSkip_;

    void NextStage();
    void SkipStage();
};