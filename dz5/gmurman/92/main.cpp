#include <iostream>
#include <random>
using namespace std;

int main() {
    // В пирамиде 5 винтовок, 3 из них с оптическим прицелом
    // P(попасть | с оптикой)   = 0.95
    // P(попасть | без оптики)  = 0.7
    // Винтовка берётся наудачу
    // Теоретически P(попасть) = (3/5)*0.95 + (2/5)*0.7 = 0.57 + 0.28 = 0.85
    const int TRIALS = 1000000;

    mt19937 rng(random_device{}());
    uniform_int_distribution<int> rifle(0, 4);      // 0..4, 0..2 - с оптикой
    uniform_real_distribution<double> dist(0.0, 1.0);

    int success = 0;

    for (int t = 0; t < TRIALS; t++) {
        // Берётся случайная винтовка
        int r = rifle(rng);

        // Вероятность попадания зависит от винтовки
        double pHit = (r < 3) ? 0.95 : 0.7;

        if (dist(rng) < pHit) success++;
    }

    double emp  = success / (double)TRIALS;
    double theo = (3.0 / 5) * 0.95 + (2.0 / 5) * 0.7;

    cout.precision(6);
    cout << fixed;
    cout << "Trials:      " << TRIALS << endl;
    cout << "Successes:   " << success << endl;
    cout << "Empirical P: " << emp << endl;
    cout << "Theoretical: " << theo << endl;

    return 0;
}