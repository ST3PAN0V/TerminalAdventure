#include "./WindowAndGraphics/WindowTools.h"
#include "./GameLogic/GameObjects.h"

using namespace std;

int main() {
    Window window;
    while(true)
    {
        if (!window.GameIsOver()) {
        window.GameInProcess();
        } else {
            if (window.GameOverWindow() == QUIT) {
                break;
            } else continue;
        }
    }
    return 0;
}
