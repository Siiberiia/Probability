#include <iostream>
#include <random>
using namespace std;

int main() {
    // Отрезок L = 20 см, на нём помещен меньший отрезок l = 10 см
    // Точка ставится наудачу на большой отрезок
    // Событие: точка попала также и на меньший отрезок
    // Теоретически P = l/L = 10/20 = 0.5
    const double L = 20.0;
    const double l = 10.0;
    const int TRIALS = 1000000;

    // Меньший отрезок расположен, например, от 5 до 15
    const double a = 5.0;
    const double b = a + l;

    mt19937 rng(random_device{}());
    uniform_real_distribution<double> dist(0.0, L);

    int success = 0;

    for (int t = 0; t < TRIALS; t++) {
        // Случайная точка на большом отрезке
        double x = dist(rng);

        // Проверяется, попала ли она на меньший отрезок
        if (x >= a && x <= b) success++;
    }

    cout << "Trials:      " << TRIALS << endl;
    cout << "Successes:   " << success << endl;
    cout << "Empirical P: " << (double)success / TRIALS << endl;
    cout << "Theoretical: " << l / L << endl;

    return 0;
}