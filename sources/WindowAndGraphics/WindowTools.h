#pragma once

#include "../GameLogic/GameObjects.h"

#include <ncurses.h>
#include <string>
#include <vector>
#include <memory>

#define SIDE_INDENTATION 52
#define TOP_INDENTATION 10

#define QUIT 1
#define NEXT_GAME 2

//----------------------------forward declaration----------------------------

class Window;
class Printer;

//---------------------------------------------------------------------------

enum class Graphic {
    Hero,
    Foundation,
    Weak_enemy,
    Medium_enemy,
    Hard_enemy,
    Void,
    Good_potion,
    Evil_potion
};

class Window
{
public:
    Window();
    ~Window();
    static void RefreshScreen();
    static void ClearScreen();
    void GameInProcess();
    bool GameIsOver();
    int GameOverWindow();
    void WelcomeWindow();

private:
    class Blocker {
    public:
        Blocker();

        bool IsBlocked();
        void BlockFor(int milliseconds);
        void UnBlock();

    private:
        std::pair<bool,int> blockedFor_;
    };

    std::unique_ptr<Game> game_;
    std::unique_ptr<Printer> printer_;
    std::unique_ptr<Blocker> blocker_;

    void StageWithEnemy();
    void StageWithItem();
    void StageWithVoid();

};


class Printer {
public:
    Printer() = default;

    void PrintFoundation() const;

    void ScreenForVoid() const;
    void ScreenForHeroAndGame(const InfoPackage package, const int score, const int stage) const;
    void ScreenForEnemy(const InfoPackage package) const;
    void ScreenForItem(const InfoPackage package) const;
    void PrintGraphic(int shiftY, int shiftX, Graphic graphicObject) const;
    void EndGameScreen(int score, int stage) const;
    void WelcomeScreen() const;
    void SkipStageScreen(int skipStageCount, int stagesForLoadSkip) const;

private:
    std::string GetStrFromEnumEnemy(const EnemyType enemyType) const;
    std::string GetStrFromEnumItem(const ItemType itemType) const;
    std::vector<std::string>& GetGraphicFromEnumGraphic(const Graphic graphicObject) const;
    Graphic GetEnumGraphicFromEnumObject(const ObjectType objectT, const EnemyType enemyT, const ItemType itemT) const;
};