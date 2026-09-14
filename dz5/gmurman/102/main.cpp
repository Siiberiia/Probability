#include <iostream>
#include <random>
using namespace std;

int main() {
    // Изделие попадает к 1-му товароведу с P = 0.55, ко 2-му с P = 0.45
    // P(признано стандартным | 1-й) = 0.9
    // P(признано стандартным | 2-й) = 0.98
    // Известно: изделие признано стандартным
    // Найти P(проверил 2-й | признано стандартным)
    //
    // Аналитика (формула Байеса):
    // P(станд.) = 0.55 * 0.9 + 0.45 * 0.98 = 0.495 + 0.441 = 0.936
    // P(2-й | станд.) = 0.441 / 0.936 ≈ 0.4712
    const int TRIALS = 1000000;

    mt19937 rng(random_device{}());
    uniform_real_distribution<double> dist(0.0, 1.0);

    int accepted = 0;        // признано стандартным
    int acceptedBySecond = 0; // из них проверено вторым

    for (int t = 0; t < TRIALS; t++) {
        // Кто проверяет
        bool second = (dist(rng) >= 0.55);   // 0.55 - первый, иначе второй

        double pAccept = second ? 0.98 : 0.9;

        // Признано стандартным?
        if (dist(rng) < pAccept) {
            accepted++;
            if (second) acceptedBySecond++;
        }
    }

    double emp = acceptedBySecond / (double)accepted;

    // Аналитика
    double p1 = 0.55, p2 = 0.45;
    double a1 = 0.9, a2 = 0.98;
    double pAccept = p1 * a1 + p2 * a2;
    double theo = (p2 * a2) / pAccept;

    cout.precision(6);
    cout << fixed;
    cout << "Trials:                     " << TRIALS << endl;
    cout << "Accepted as standard:       " << accepted << endl;
    cout << "Accepted by second:         " << acceptedBySecond << endl;
    cout << endl;
    cout << "P(second | accepted):" << endl;
    cout << "  emp  = " << emp << endl;
    cout << "  theo = " << theo << endl;

    return 0;
}