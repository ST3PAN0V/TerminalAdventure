#include "WindowTools.h"
#include "GraphicArt.h"

#include <iostream>
#include <thread>

using namespace std;

//------------------------------------Blocker----------------------------------------

Window::Blocker::Blocker() : blockedFor_(false,0) {}

bool Window::Blocker::IsBlocked() {
    return blockedFor_.first;
}

void Window::Blocker::BlockFor(int milliseconds) {
    blockedFor_.first = true;
    blockedFor_.second = milliseconds;
    nodelay(stdscr, TRUE);
}

void Window::Blocker::UnBlock() {
    if (IsBlocked()) {
        napms(blockedFor_.second);
        blockedFor_.first = false;
        nodelay(stdscr, FALSE);
    }
}

//------------------------------------Window-----------------------------------------

Window::Window() : game_(make_unique<Game>()), printer_(make_unique<Printer>()),
        blocker_(make_unique<Blocker>()) {
    initscr();
    refresh();
    cbreak();
    noecho();
    start_color(); // Включаем поддержку цветов
    keypad(stdscr, TRUE); // Считывать специальные клавиши
    curs_set(0); // спрятать курсор

    Window::ClearScreen();
    this->WelcomeWindow();
    Window::RefreshScreen();
}

Window::~Window() {
    endwin();
}

void Window::RefreshScreen() {
    refresh();
}

void Window::ClearScreen() {
    clear();
}

void Window::WelcomeWindow() {
    printer_->WelcomeScreen();
    getch();
}

bool Window::GameIsOver() {
    return game_->GameIsOver();
}

int Window::GameOverWindow() {
    Window::ClearScreen();
    printer_->PrintFoundation();
    printer_->EndGameScreen(game_->GetScore(), game_->GetStageNumber());

    game_.reset();

    char pressedKey = getch();

    switch (pressedKey) {
        case 'q':
            this->~Window();
            return QUIT;
            break;  
        default:
            game_ = make_unique<Game>();
            mvprintw(TOP_INDENTATION + 16, SIDE_INDENTATION + 39, "New game is starting...");
            Window::RefreshScreen();
            blocker_->BlockFor(3000);
            return NEXT_GAME;
            break;
    }
}

void Window::GameInProcess() {
    blocker_->UnBlock();

    Window::ClearScreen();

    printer_->PrintFoundation();
    printer_->ScreenForHeroAndGame(game_->GetHeroAndGameInfo(), game_->GetScore(),
            game_->GetStageNumber());
    printer_->SkipStageScreen(game_->GetCountOfSkip(), game_->GetStagesForLoadSkip());
    switch (game_->GetStageInfo().objectType) {
        case ObjectType::Enemy:
            StageWithEnemy();
            break;
        case ObjectType::Item:
            StageWithItem();
            break;
        case ObjectType::None:
            StageWithVoid();
            break;
        default:
            break;
    }
}

void Window::StageWithEnemy() {
    printer_->ScreenForEnemy(game_->GetStageInfo());

    char pressedKey = getch();

    switch (pressedKey) {
        case 'a':
            game_->FightWithEnemy();
            break;
        case 'n':
            if (!game_->TryNextStage()) {
                mvprintw(TOP_INDENTATION + 22, SIDE_INDENTATION + 6, "Firstly clear this stage!");
                blocker_->BlockFor(1000);
            }
            break;
        case 's':
            if (!game_->TrySkipStage()) {
                mvprintw(TOP_INDENTATION + 22, SIDE_INDENTATION + 6, "You can't skip stage now!");
                blocker_->BlockFor(1000);
            }
            break;
        default:
            break;
    }
    Window::RefreshScreen();
}

void Window::StageWithItem() {
    printer_->ScreenForItem(game_->GetStageInfo());

    char pressedKey = getch();

    switch (pressedKey) {
        case 'u':
            game_->UseItem();
            break;
        case 'n':
            if (!game_->TryNextStage()) {
                mvprintw(TOP_INDENTATION + 22, SIDE_INDENTATION + 6, "Firstly clear this stage!");
                blocker_->BlockFor(1000);
            }
            break;
        case 's':
            if (!game_->TrySkipStage()) {
                mvprintw(TOP_INDENTATION + 22, SIDE_INDENTATION + 6, "You can't skip stage now!");
                blocker_->BlockFor(1000);
            }
            break;
        default:
            break;
    }
    Window::RefreshScreen();
}

void Window::StageWithVoid() {
    printer_->ScreenForVoid();

    char pressedKey = getch();

    switch (pressedKey) {
        case 'n':
            game_->TryNextStage();
            break;
        case 's':
            if (!game_->TrySkipStage()) {
                mvprintw(TOP_INDENTATION + 22, SIDE_INDENTATION + 6, "You can't skip stage now!");
                blocker_->BlockFor(1000);
            }
            break;
        default:
            break;
    }
    Window::RefreshScreen();
}

//------------------------------------Printer-----------------------------------------

string Printer::GetStrFromEnumEnemy(EnemyType enemyType) const {
        switch (enemyType) {
        case EnemyType::Weak_enemy:
            return "weak";
            break;
        case EnemyType::Medium_enemy:
            return "medium";
            break;
        case EnemyType::Hard_enemy:
            return "hard";
            break;
        default:
            return "None";
            break;
    }
}

string Printer::GetStrFromEnumItem(ItemType itemType) const {
        switch (itemType) {
        case ItemType::Potion_good:
            return "good";
            break;
        case ItemType::Potion_evil:
            return "evil";
            break;
        default:
            return "None";
            break;
    }
}

void Printer::PrintFoundation() const {
    for (size_t i = 0; i < foundationGRAPHIC.size(); i++) {
        mvprintw(TOP_INDENTATION+i, SIDE_INDENTATION,"%s\n", foundationGRAPHIC[i].c_str());  
    }
    Window::RefreshScreen();
}

void Printer::WelcomeScreen() const {
    mvprintw(TOP_INDENTATION + 10, SIDE_INDENTATION + 31, "Hi! It's a TerminalAdventure!");
    mvprintw(TOP_INDENTATION + 12, SIDE_INDENTATION + 12, "Complete as many stages as possible and get as many points as possible.");
    mvprintw(TOP_INDENTATION + 14, SIDE_INDENTATION + 19, "You can see hints and commands in the lower left corner.");

    mvprintw(TOP_INDENTATION + 16, SIDE_INDENTATION + 34, "Press any key to start!");
}

void Printer::EndGameScreen(int score, int stage) const {
    mvprintw(TOP_INDENTATION + 10, SIDE_INDENTATION + 40, "_.-*GAME IS OVER*-._");
    mvprintw(TOP_INDENTATION + 12, SIDE_INDENTATION + 38, "You died on the %d stage.", stage);
    mvprintw(TOP_INDENTATION + 14, SIDE_INDENTATION + 41, "Your score is %d!",score);

    mvprintw(TOP_INDENTATION + 24, SIDE_INDENTATION + 6, "Commands:");
    mvprintw(TOP_INDENTATION + 26, SIDE_INDENTATION + 6, "Press 'q' to quit");
    mvprintw(TOP_INDENTATION + 27, SIDE_INDENTATION + 6, "Press any key to start new game");
}

void Printer::ScreenForHeroAndGame(const InfoPackage package, const int score,
        const int stage) const {
    mvprintw(TOP_INDENTATION + 2, SIDE_INDENTATION + 6, "|Your health:");
    if (package.health <= CRITICAL_HEALTH) {
        init_pair(4, COLOR_RED, COLOR_BLACK);
        attron(COLOR_PAIR(4));
        mvprintw(TOP_INDENTATION + 2, SIDE_INDENTATION + 20, "%d", package.health);
        attroff(COLOR_PAIR(4));
    } else mvprintw(TOP_INDENTATION + 2, SIDE_INDENTATION + 20, "%d", package.health);
    mvprintw(TOP_INDENTATION + 4, SIDE_INDENTATION + 6, "|Your strength: %d", package.strenght);
    mvprintw(TOP_INDENTATION + 6, SIDE_INDENTATION + 6, "|Your armor: %d", package.armor);

    mvprintw(TOP_INDENTATION + 8, SIDE_INDENTATION + 6, "|Score: %d", score);

    mvprintw(TOP_INDENTATION + 8, SIDE_INDENTATION + 46, "Stage: %d", stage);

    PrintGraphic(33, 10, Graphic::Hero);
}

void Printer::ScreenForEnemy(const InfoPackage package) const {
    mvprintw(TOP_INDENTATION + 2, SIDE_INDENTATION + 76, "|Enemy type:");
    switch (package.enemyType)
    {
    case EnemyType::Weak_enemy:
        init_pair(3, COLOR_CYAN, COLOR_BLACK);
        attron(COLOR_PAIR(3));
        mvprintw(TOP_INDENTATION + 2, SIDE_INDENTATION + 89, "%s", GetStrFromEnumEnemy(package.enemyType).c_str());
        attroff(COLOR_PAIR(3));
        break;
    case EnemyType::Medium_enemy:
        init_pair(2, COLOR_BLUE, COLOR_BLACK);
        attron(COLOR_PAIR(2));
        mvprintw(TOP_INDENTATION + 2, SIDE_INDENTATION + 89, "%s", GetStrFromEnumEnemy(package.enemyType).c_str());
        attroff(COLOR_PAIR(2));
        break;
    case EnemyType::Hard_enemy:
        init_pair(1, COLOR_MAGENTA, COLOR_BLACK);
        attron(COLOR_PAIR(1));
        mvprintw(TOP_INDENTATION + 2, SIDE_INDENTATION + 89, "%s", GetStrFromEnumEnemy(package.enemyType).c_str());
        attroff(COLOR_PAIR(1));
        break;
    default:
        mvprintw(TOP_INDENTATION + 2, SIDE_INDENTATION + 89, "%s", GetStrFromEnumEnemy(package.enemyType).c_str());
        break;
    }
    mvprintw(TOP_INDENTATION + 4, SIDE_INDENTATION + 76, "|Enemy health: %d", package.health);
    mvprintw(TOP_INDENTATION + 6, SIDE_INDENTATION + 76, "|Enemy strength: %d", package.strenght);
    mvprintw(TOP_INDENTATION + 8, SIDE_INDENTATION + 76, "|Enemy armor: %d", package.armor);

    mvprintw(TOP_INDENTATION + 24, SIDE_INDENTATION + 6, "Commands:");
    mvprintw(TOP_INDENTATION + 26, SIDE_INDENTATION + 6, "Press 'a' to attack");
    mvprintw(TOP_INDENTATION + 27, SIDE_INDENTATION + 6, "Press 'n' to next stage");
    
    PrintGraphic(50,10, GetEnumGraphicFromEnumObject(package.objectType, package.enemyType, package.itemType));
}

void Printer::ScreenForItem(const InfoPackage package) const {
    mvprintw(TOP_INDENTATION + 2, SIDE_INDENTATION + 76, "|Potion type: %s",
            GetStrFromEnumItem(package.itemType).c_str());
    mvprintw(TOP_INDENTATION + 4, SIDE_INDENTATION + 76, "|Potion health: %d", package.health);
    mvprintw(TOP_INDENTATION + 6, SIDE_INDENTATION + 76, "|Potion strength: %d", package.strenght);
    mvprintw(TOP_INDENTATION + 8, SIDE_INDENTATION + 76, "|Potion armor: %d", package.armor);

    mvprintw(TOP_INDENTATION + 24, SIDE_INDENTATION + 6, "Commands:");
    mvprintw(TOP_INDENTATION + 26, SIDE_INDENTATION + 6, "Press 'u' to use item");
    mvprintw(TOP_INDENTATION + 27, SIDE_INDENTATION + 6, "Press 'n' to next stage");

    PrintGraphic(50,10, GetEnumGraphicFromEnumObject(package.objectType, package.enemyType, package.itemType));
}

void Printer::ScreenForVoid() const {
    mvprintw(TOP_INDENTATION + 2, SIDE_INDENTATION + 76, "|Void");

    mvprintw(TOP_INDENTATION + 24, SIDE_INDENTATION + 6, "Commands:");
    mvprintw(TOP_INDENTATION + 26, SIDE_INDENTATION + 6, "Press 'n' to next stage");

    PrintGraphic(50, 10, GetEnumGraphicFromEnumObject(ObjectType::None, EnemyType::None, ItemType::None));
}

void Printer::PrintGraphic(int shiftX, int shiftY, Graphic graphicObject) const {
    vector<string>& printObject = GetGraphicFromEnumGraphic(graphicObject);
    for(size_t row = 0; row < printObject.size(); row++)
    {
        mvprintw(TOP_INDENTATION + shiftY+row, SIDE_INDENTATION + shiftX, "%s", printObject[row].c_str());
    }
}

vector<string>& Printer::GetGraphicFromEnumGraphic(const Graphic graphicObject) const {
    switch (graphicObject) {
    case Graphic::Foundation:
        return foundationGRAPHIC;
        break;
    case Graphic::Weak_enemy:
        return weakEnemyGRAPHIC;
        break;
    case Graphic::Medium_enemy:
        return mediumEnemyGRAPHIC;
        break;
    case Graphic::Hard_enemy:
        return hardEnemyGRAPHIC;
        break;
    case Graphic::Hero:
        return heroGRAPHIC;
        break;
    case Graphic::Evil_potion:
        return evilPotionGRAPHIC;
        break;
    case Graphic::Good_potion:
        return goodPotionGRAPHIC;
        break;
    case Graphic::Void:
        return VoidGRAPHIC;
        break;
    default:
        return VoidGRAPHIC;
        break;
    }
}

Graphic Printer::GetEnumGraphicFromEnumObject(const ObjectType objectT, const EnemyType enemyT,
        const ItemType itemT) const {
    switch (objectT) {
        case ObjectType::Enemy:
            switch (enemyT) {
                case EnemyType::Weak_enemy:
                    return Graphic::Weak_enemy;
                    break;
                case EnemyType::Medium_enemy:
                    return Graphic::Medium_enemy;
                    break;
                case EnemyType::Hard_enemy:
                    return Graphic::Hard_enemy;
                    break;
                default:
                    return Graphic::Void;
                    break;
            }
            break;

        case ObjectType::Item:
            switch (itemT) {
                case ItemType::Potion_good:
                    return Graphic::Good_potion;
                    break;
                case ItemType::Potion_evil:
                    return Graphic::Evil_potion;
                    break;
                default:
                    return Graphic::Void;
                    break;
            }
            break;

        case ObjectType::None:
            return Graphic::Void;
            break;

        default:
            return Graphic::Void; 
            break;
    }
}

void Printer::SkipStageScreen(int skipStageCount, int stagesForLoadSkip) const {
    mvprintw(TOP_INDENTATION + 14, SIDE_INDENTATION + 6, "Skip stage: %d", skipStageCount);
    mvprintw(TOP_INDENTATION + 15, SIDE_INDENTATION + 6, "+1 skip after: %d", stagesForLoadSkip);
    if (skipStageCount > 0) mvprintw(TOP_INDENTATION + 28, SIDE_INDENTATION + 6, "Press 's' to skip this stage");
}
