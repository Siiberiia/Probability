#include <iostream>
#include <random>
using namespace std;

int main() {
    // Отрезок AB длины a, брошены 5 точек
    // Событие: 2 точки на расстоянии < x от A, 3 точки на расстоянии > x
    // P(точка ближе x к A) = x/a, P(точка дальше x от A) = 1 - x/a
    const int TRIALS = 1000000;

    mt19937 rng(random_device{}());
    uniform_real_distribution<double> dist(0.0, 1.0);

    // Параметр: доля x/a
    double r;
    cout << "Enter x/a (0..1): ";
    cin >> r;

    int success = 0;

    for (int t = 0; t < TRIALS; t++) {
        int close = 0;
        for (int i = 0; i < 5; i++)
            if (dist(rng) < r) close++;

        if (close == 2) success++;
    }

    // Аналитика: C(5,2) * r^2 * (1-r)^3
    double theo = 10.0 * r * r * pow(1 - r, 3);

    cout.precision(6);
    cout << fixed;
    cout << "emp  = " << success / (double)TRIALS << endl;
    cout << "theo = " << theo << endl;

    return 0;
}