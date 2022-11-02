

#include <iostream> 
#include <stdlib.h>
#include <vector>
#include <string>
#include <thread>        
#include <chrono>   
#include <windows.h>
#include <locale.h>

using namespace std;

struct IAttack {
    string name; 
    int damage; 
    bool infinite; 
    int usages; 
};

static std::vector<IAttack> characterAttacks = {
    { "Sencillo", 5, true, 0 },
    { "Combo", 70, false, 20 },
    { "RPG", 200, false, 5 },
}; 

class Character {
    public: 
        int health;
        bool isEnemy; 
        string name; 

        Character(int health, bool isEnemy, string name);

        bool isAlive();  

        void setHealth(int health); 
}; 

void Character::setHealth(int health) {
    this->health = health;

    if(this->health <= 0){
        cout << this->name << " ha muerto..." << endl; 
    }
}

Character::Character(int health, bool isEnemy, string name) {
    this->health = health; 
    this->isEnemy = isEnemy; 
    this->name = name; 
}

bool Character::isAlive() {
    if (this->health > 0) return true; 
       
    return false; 
}

string playerInput;

// Centered: https://stackoverflow.com/questions/45582095/c-how-can-i-center-text-output; 
void CoutCentered(std::string text) {

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    PCONSOLE_SCREEN_BUFFER_INFO lpScreenInfo = new CONSOLE_SCREEN_BUFFER_INFO();
    GetConsoleScreenBufferInfo(hConsole, lpScreenInfo);
    COORD NewSBSize = lpScreenInfo->dwSize;

    if (NewSBSize.X > text.size()) {
        int newpos = ((NewSBSize.X - text.size()) / 2);
        for (int i = 0; i < newpos; i++) std::cout << " ";
    }

    std::cout << text << std::endl;
}

void validateEnemyName(string firstValue, int firstHealth, string secondValue, int secondHealth) {
    cout << "Quieres golpear a " << firstValue << " o " << secondValue << "? ";
    cin >> playerInput; 

    bool alive = true; 

    if (playerInput == firstValue) {
        if (firstHealth > 0) {
            return; 
        } 

        alive = false; 
    } else if (playerInput == secondValue) {
        if (secondHealth > 0) {
            return; 
        }

        alive = false; 
    } 

    system("CLS");

    if (alive) {
        cout << "Valor introducido no válido, por favor, repite de nuevo..."; 
    }
    else {
        cout << "El personaje que has seleccionado golpear se encuentra muerto, por favor, selecciona otro";
    }

    std::this_thread::sleep_for(std::chrono::seconds(1));
    system("CLS");
    return validateEnemyName(firstValue, firstHealth, secondValue, secondHealth);
}

int selectAttack(string name) {
    string question = "Ataques disponibles: \n\n";

    // Unsigned deg if u want to know: https://es.stackoverflow.com/questions/396050/cu%C3%A1l-es-el-significado-de-unsigned-en-c; 

    for (unsigned int i = 0; i < characterAttacks.size(); i++) {
        question = question + "- " + characterAttacks[i].name + ": " + (
            (characterAttacks[i].usages > 0) ?
                std::to_string(characterAttacks[i].usages)
            : 
                ((characterAttacks[i].infinite == true) ? "Infinito" : "Agotado")    
        ) + "\n";
    }  

    cout << question << "\nIntroduce nombre de ataque: ";
    cin >> playerInput;

    bool founded = false; 

    system("CLS");

    for (unsigned int i = 0; i < characterAttacks.size(); i++) {
        if (playerInput == characterAttacks[i].name) { 
            if (characterAttacks[i].infinite) {
                founded = true;
                cout << "Has usado el ataque \033[31m" << characterAttacks[i].name << "\033[0m y has producido " << characterAttacks[i].damage << " a " << name << endl;
                return characterAttacks[i].damage;
                break;
            } 

            if (characterAttacks[i].usages > 0) {
                characterAttacks[i].usages--; 
                founded = true;
                cout << "Has usado el ataque \033[31m" << characterAttacks[i].name << "\033[0m y has producido " << characterAttacks[i].damage << " a " << name << endl;
                return characterAttacks[i].damage;
                break;
            } 
        }
    }

    if (!founded) {
        system("CLS");

        cout << "Valor introducido no válido, por favor, repite de nuevo...";
        std::this_thread::sleep_for(std::chrono::seconds(1));

        system("CLS");

        return selectAttack(name);
    }
}

int main()
{

    setlocale(LC_ALL, "spanish");
    system("color 00"); 
    system("CLS");

    cout << "Bienvenido, por favor, selecciona un nombre de usuario: ";
    cin >> playerInput; 

    Character playerSoldier(400, false, playerInput); 

    Character firstEnemy(200, true, "Creeper");
    Character secondEnemy(200, true, "Boss");

    system("CLS");

    cout << playerSoldier.name << " parece que te has metido en un buen lio eh ¿Vas a querer pelear? [y/n] : ";
    cin >> playerInput; 

    system("CLS");

    if (playerInput == "y") {
        while (playerSoldier.isAlive() && firstEnemy.isAlive() || secondEnemy.isAlive()) {
            validateEnemyName(firstEnemy.name, firstEnemy.health, secondEnemy.name, secondEnemy.health);

            system("CLS");

            if (playerInput == firstEnemy.name) {
                int attackedDamage = selectAttack(firstEnemy.name); 
                firstEnemy.setHealth(firstEnemy.health - attackedDamage); 
            } 
            else {
                int attackedDamage = selectAttack(secondEnemy.name);
                secondEnemy.setHealth(secondEnemy.health - attackedDamage);
            }

            system("CLS");

            int firstEnemyAttack = rand() % 50 + 2;
            int secondEnemyAttack = rand() % 50 + 2;

            if (firstEnemy.isAlive()) {
                playerSoldier.setHealth(playerSoldier.health - firstEnemyAttack);
                cout << firstEnemy.name << " te ha atacado y has sufrido \033[31m" << firstEnemyAttack << "\033[0m de daño, te quedan: \033[32m" << playerSoldier.health << "\033[0mHP" << endl;
            }

            if (secondEnemy.isAlive()) {
                playerSoldier.setHealth(playerSoldier.health - secondEnemyAttack);
                cout << secondEnemy.name << " te ha atacado y has sufrido \033[31m" << secondEnemyAttack << "\033[0m de daño, te quedan: \033[32m" << playerSoldier.health << "\033[0mHP" << endl;
            } 

            std::this_thread::sleep_for(std::chrono::seconds(7));

            system("CLS");
        }

        if (playerSoldier.isAlive()) {
            system("color 27");
            CoutCentered("Has ganado"); 
        }
        else {
            system("color 47");
            CoutCentered("Has perdido");  
        }

        std::this_thread::sleep_for(std::chrono::seconds(10));
    } 
    else {
        cout << playerSoldier.name << " una retirada a tiempo es una victoria no? o eso dicen... " << endl; 
    }
}