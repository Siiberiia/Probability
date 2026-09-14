#include <iostream>
#include <random>
#include <cmath>
using namespace std;

int main() {
    // x и y равномерно на [0,1]
    // Событие: x + y <= 1 и x*y >= 0.09
    // Теоретически P = интеграл от 0.1 до 0.9 (1 - x - 0.09/x) dx
    //              = 0.4 - 0.09 * ln(9)
    const int TRIALS = 1000000;

    mt19937 rng(random_device{}());
    uniform_real_distribution<double> dist(0.0, 1.0);
    int success = 0;

    for (int t = 0; t < TRIALS; t++) {
        double x = dist(rng);
        double y = dist(rng);

        if (x + y <= 1.0 && x * y >= 0.09) success++;
    }

    double theo = 0.4 - 0.09 * log(9.0);

    cout << "Trials:      " << TRIALS << endl;
    cout << "Successes:   " << success << endl;
    cout << "Empirical P: " << (double)success / TRIALS << endl;
    cout << "Theoretical: " << theo << endl;

    return 0;
}