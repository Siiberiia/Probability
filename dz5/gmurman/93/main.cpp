#include <iostream>
#include <random>
using namespace std;

int main() {
    // Детали: 12 с завода 1, 20 с завода 2, 18 с завода 3
    // Всего 50 деталей
    // P(отличное | завод 1) = 0.9
    // P(отличное | завод 2) = 0.6
    // P(отличное | завод 3) = 0.9
    // Теоретически P(отличное) = (12/50)*0.9 + (20/50)*0.6 + (18/50)*0.9
    //                          = 0.216 + 0.24 + 0.324 = 0.78
    const int TRIALS = 1000000;

    mt19937 rng(random_device{}());
    uniform_int_distribution<int> idx(0, 49);   // индекс детали
    uniform_real_distribution<double> dist(0.0, 1.0);

    int success = 0;

    for (int t = 0; t < TRIALS; t++) {
        int k = idx(rng);

        // Определяется, с какого завода деталь
        double pGood;
        if (k < 12)       pGood = 0.9;   // завод 1
        else if (k < 32)  pGood = 0.6;   // завод 2 (12..31)
        else              pGood = 0.9;   // завод 3 (32..49)

        if (dist(rng) < pGood) success++;
    }

    double emp  = success / (double)TRIALS;
    double theo = (12.0 / 50) * 0.9 + (20.0 / 50) * 0.6 + (18.0 / 50) * 0.9;

    cout.precision(6);
    cout << fixed;
    cout << "Trials:      " << TRIALS << endl;
    cout << "Successes:   " << success << endl;
    cout << "Empirical P: " << emp << endl;
    cout << "Theoretical: " << theo << endl;

    return 0;
}