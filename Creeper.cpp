
#include <iostream> 
#include <stdlib.h>
#include <vector> 
#include <string> 
#include <thread>        
#include <chrono>  

using namespace std;

// @Declarations
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

struct IEnemies {
    string name; 
    int health;  
};

static std::vector<IEnemies> enemies = {
    { "Frescura", 100 },
    { "Reventao", 100 },
}; 

string heroName = "Xavier"; 
int heroHealth = 200; 

string playerInput;

// @Functions
bool anyEnemyAlive() { 
    for (int index = 0; index < enemies.size(); index++) {
        if (enemies[index].health > 0) {
            return true; 
        }
    }
    
    return false; 
}

string getFormattedEnemies() {
    string formattedString = "";

    for (unsigned int index = 0; index < enemies.size(); index++) {
        formattedString = formattedString + "[" + to_string(index) + "] " + enemies[index].name + " - " + to_string(enemies[index].health) + "HP\n";
    }

    return formattedString; 
}

int validateEnemyValues() {
    cin >> playerInput;

    for (unsigned int index = 0; index < enemies.size(); index++) {
        if (enemies[index].name == playerInput) {
            if (enemies[index].health > 0) {
                return index; 
            }
            else {
                cout << "Este enemigo esta muerto, introduce otro nombre: ";
                return validateEnemyValues();
            }
        }
    }
    
    cout << "Nombre no valido, por favor, introduce de nuevo: "; 
    return validateEnemyValues();
}

void attackSomeone(string attackerName, int &defenderHealth, int damage) {
    cout << attackerName << " acaba de atacar y ha hecho " << damage << " de daño.\n"; 
    defenderHealth = defenderHealth - damage; 

    if (defenderHealth < 0) {
        defenderHealth = 0; 
    }

    return; 
}

int selectAttack() {
    system("CLS");
    string question = "Ataques disponibles: \n\n";

    for (unsigned int i = 0; i < characterAttacks.size(); i++) {
        question = question + "[" + characterAttacks[i].name + "] Usos: " + (
            (characterAttacks[i].usages > 0) ?
                std::to_string(characterAttacks[i].usages)
            :
                ((characterAttacks[i].infinite == true) ? "Infinito" : "Agotado")
        ) + "\n";
    }

    question = question + "\nIntroduce nombre de ataque: "; 

    cout << question; 
    cin >> playerInput;  

    for (unsigned int i = 0; i < characterAttacks.size(); i++) {
        if (playerInput == characterAttacks[i].name) {
            if (characterAttacks[i].infinite) { 
                return characterAttacks[i].damage; 
            }

            if (characterAttacks[i].usages > 0) {
                characterAttacks[i].usages--; 
                return characterAttacks[i].damage; 
            }
        }
    }

    cout << "Valor introducido no válido, por favor, repite de nuevo..." << endl;
    return selectAttack();
}

// @Main thread
int main()
{
    while (heroHealth > 0 && anyEnemyAlive()) {
        cout << heroName << " tienes el turno de ataque, selecciona tu contrincante \n\n" << getFormattedEnemies() << "\nIntroduce el nombre: ";
        int toAttackId = validateEnemyValues(); 
        int attackDamage = selectAttack(); 

        system("CLS");
 
        attackSomeone(heroName, enemies[toAttackId].health, attackDamage);

        std::this_thread::sleep_for(std::chrono::seconds(3));
        system("CLS");

        for (unsigned int index = 0; index < enemies.size(); index++) {
            if (enemies[index].health > 0) {
                attackSomeone(enemies[index].name, heroHealth, (rand() % 50 + 2)); 
            }  
        }

        std::this_thread::sleep_for(std::chrono::seconds(3));
        system("CLS");
    }

    cout << "El juego ha terminado"; 
}
