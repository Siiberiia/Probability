#include <iostream>
#include <random>
using namespace std;

int main() {
    // Отрезок OA длины L, точка B ставится наудачу
    // Событие: меньший из OB и BA больше L/3
    // Теоретически P = 1/3
    const double L = 1.0;
    const int TRIALS = 1000000;

    mt19937 rng(random_device{}());
    uniform_real_distribution<double> dist(0.0, L);
    int success = 0;

    for (int t = 0; t < TRIALS; t++) {
        // Случайная точка на отрезке
        double x = dist(rng);

        // Меньший из отрезков OB и BA
        double mn = min(x, L - x);

        if (mn > L / 3) success++;
    }

    cout << "Trials:      " << TRIALS << endl;
    cout << "Successes:   " << success << endl;
    cout << "Empirical P: " << (double)success / TRIALS << endl;
    cout << "Theoretical: " << 1.0 / 3 << endl;

    return 0;
}