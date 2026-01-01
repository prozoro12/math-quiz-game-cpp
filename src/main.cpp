#include <iostream>
#include <windows.h>
#include <random>
#include <tuple>
#include <array>
#include <string>

using namespace std;

// -------------------- Player Class --------------------
class Player {
private:
    int maxHealth;
    int health;
    string name;

public:
    int getHealth() const { return health; }
    string getName() const { return name; }

    bool damage(int d) {
        health -= d;
        if (health <= 0) {
            cout << "Oops, you have killed " << name << "! Meany >:(" << endl;
            return true;
        }
        else {
            cout << "Oof!! Wrong answer :(" << endl;
            return false;
        }
    }

    void heal(int h) {
        health += h;
        if (health > maxHealth) {
            health = maxHealth;
            cout << "Max health attained! Cannot increase." << endl;
        }
        else {
            cout << "Yay!! Correct answer :)" << endl;
        }
    }

    void displayHealth() const {
        string displayHP = "";

        for (int i = 0; i < health; i++)
            displayHP += u8"♥";
        for (int i = 0; i < maxHealth - health; i++)
            displayHP += "-";

        cout << name << "'s HP: " << displayHP
            << " (" << health << "/" << maxHealth << ")" << endl;
    }

    Player(const string& name, int maxHealth = 5)
        : name(name), maxHealth(maxHealth), health(maxHealth) {
    }
};

// -------------------- Equation Generator --------------------
tuple<int, string, int, int> getEquation() {
    array<string, 4> ops = { "+", "-", "*", "/" };

    static random_device rd;
    static mt19937 gen(rd());

    uniform_int_distribution<> opDist(0, 3);
    uniform_int_distribution<> numDist(0, 20);

    string op = ops[opDist(gen)];
    int num1, num2, ans;

    do {
        num1 = numDist(gen);
        num2 = numDist(gen);
    } while (op == "/" && (num2 == 0 || num1 % num2 != 0));

    if (op == "+")       ans = num1 + num2;
    else if (op == "-")  ans = num1 - num2;
    else if (op == "*")  ans = num1 * num2;
    else                 ans = num1 / num2;

    return make_tuple(num1, op, num2, ans);
}

// -------------------- Main Game Loop --------------------
int main() {
    SetConsoleOutputCP(CP_UTF8); // Enable UTF-8 output for hearts

    cout << "Welcome to the Health Counter Game!" << endl;
    cout << "Please give a name to your player: ";

    string input;
    cin >> input;

    Player player(input);

    cout << "\nHello " << player.getName() << "!" << endl;
    cout << "Answer questions correctly to heal." << endl;
    cout << "Wrong answer = damage!" << endl;
    cout << "Type Q or QUIT to exit.\n" << endl;

    bool quit = false;

    while (!quit) {
        player.displayHealth();

        auto equation = getEquation();
        cout << "What is "
            << get<0>(equation) << " "
            << get<1>(equation) << " "
            << get<2>(equation) << " ? ";

        cin >> input;

        if (input == "q" || input == "Q" || input == "QUIT") {
            quit = true;
            continue;
        }

        int userAnswer;
        try {
            userAnswer = stoi(input);
        }
        catch (...) {
            cout << "Invalid input! Please enter a number." << endl;
            continue;
        }

        if (userAnswer != get<3>(equation)) {
            if (player.damage(1))
                quit = true;
        }
        else {
            player.heal(1);
        }

        cout << endl;
    }

    cout << "Exiting Program. | Bye bye |" << endl;
    return 0;
}
