# TerminalAdventure

An endless terminal adventure written in C++17. Move through increasingly difficult stages, fight enemies, use potions, and manage a limited supply of stage skips.

The interface uses ncurses; CMake builds the game, and a Dockerfile is included for Linux.

## Gameplay

Each stage contains an enemy, a potion, or an empty space. Enemies become stronger as you progress, while potions become more effective.

- **Enemies:** weak, medium, and strong types with different health, damage, and armor.
- **Potions:** beneficial potions restore attributes; harmful potions reduce them.
- **Stage skips:** bypass an encounter when a skip is available.
- **Score:** survive for as many stages as possible and start another run from the game-over screen.

The interface shows your health, strength, armor, stage, score, and available commands.

## Run with Docker

Install Docker, then build and launch the game with an interactive terminal:

```bash
git clone https://github.com/ST3PAN0V/TerminalAdventure.git
cd TerminalAdventure
docker build -t terminal-adventure .
docker run --rm -it --entrypoint /app/build/TerminalAdventure terminal-adventure
```

Use a large terminal window: the current interface uses a fixed layout.

## Build from source

Requirements:

- Linux.
- A C++17 compiler.
- CMake 3.11 or newer and a supported build tool.
- ncurses development headers and library.

From the repository root:

```bash
mkdir -p build
cd build
cmake ..
cmake --build .
./TerminalAdventure
```

## Controls

| Key | Action |
| --- | --- |
| `a` | Attack the enemy on the current stage |
| `u` | Use the potion on the current stage |
| `n` | Advance when the stage allows it |
| `s` | Skip the stage when a skip is available |
| `q` | Quit from the game-over screen |
| Any other key at game over | Start a new game |

Follow the command hints in the lower-left area of the interface.

## Project layout

| Path | Purpose |
| --- | --- |
| [`sources/main.cpp`](sources/main.cpp) | Main game loop |
| [`sources/GameLogic/`](sources/GameLogic/) | Game state, stages, enemies, and items |
| [`sources/WindowAndGraphics/`](sources/WindowAndGraphics/) | ncurses interface and keyboard handling |
| [`CMakeLists.txt`](CMakeLists.txt) | C++17 build configuration |
| [`Dockerfile`](Dockerfile) | Container build and game entry point |

## Possible extensions

Ideas for future development include attack animations, chests, swords, and additional entity types.

## License and contact

Licensed under the [BSD 3-Clause License](LICENSE).

Questions and suggestions: [Telegram](https://t.me/ArtemS101).

