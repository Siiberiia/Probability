// Задача 61
#include <iostream>
#include <random>
#include <cmath>
using namespace std;

int main() {
    // Вероятность попадания при одном выстреле p = 0.8
    // Найти минимальное n, при котором P(ни одного промаха) < 0.4
    // P(нет промахов) = p^n
    // Условие: 0.8^n < 0.4  =>  n > ln(0.4)/ln(0.8)
    const double p = 0.8;
    const int TRIALS = 1000000;

    // Аналитика: находим n перебором
    int nTheory = 1;
    while (pow(p, nTheory) >= 0.4) nTheory++;

    cout << "Theory: n = " << nTheory
         << ", p^n = " << pow(p, nTheory) << endl;

    // Эмпирика: для каждого n моделируем серию из n выстрелов,
    // проверяем что не было ни одного промаха
    mt19937 rng(random_device{}());
    uniform_real_distribution<double> dist(0.0, 1.0);

    for (int n = 1; n <= nTheory + 2; n++) {
        int success = 0;
        for (int t = 0; t < TRIALS; t++) {
            bool allHit = true;
            for (int s = 0; s < n; s++)
                if (dist(rng) >= p) { allHit = false; break; }
            if (allHit) success++;
        }
        double emp = success / (double)TRIALS;
        cout << "n = " << n << ":  emp = " << emp
             << "  theo = " << pow(p, n) << endl;
    }

    return 0;
}