# TerminalAdventure
A simple terminal game!

# How to run game
```bash
git clone https://github.com/ST3PAN0V/TerminalAdventure.git
cd TerminalAdventure
mkdir build && cd build
cmake ..
cmake --build .
./TerminalAdventure
```
*Have fun!*

## **How does the game work?**
There are endless floors and two types of creatures: potions, enemies. With each floor, the likelihood of trouble increases, enemies become stronger, and potions become more effective. There are empty floors that you can just go through, there are also floors with enemies that prevent you from going further and floors with potions. You can skip floors if you realize you can't defeat an enemy or use a bad potion.

## **Entities**
1. **Potions:**
   - There are good and bad potions. It's simple. It is better to skip bad potions, they reduce all parameters. Good potions restore all parameters. The effectiveness of potions also increases with each new floor.
   - The potion itself will drop with an initial chance of 15%, then the chance increases. good potions drop with a 60% chance, and bad ones with a 40% chance. this ratio does not change throughout the game.
2. **Enemies:**
   - There are weak, medium and strong enemies. weak enemies have no defense, and their damage and lives grow very slowly. Medium enemies' damage and health grow faster and have weaker armor.
   - Strong enemies have good armor, health and damage, which grow quickly with each floor. After killing, the potion will drop with a 100% chance
3. **Void:**
   - Void.

## **Plans and updates**
If I continue to develop the game, I will add attack animations and new entities, such as chests and swords.

Add Docker for simple building the game.

If you have any suggestions or recommendations, please write to **_@ArtemS101_** in telegram!
