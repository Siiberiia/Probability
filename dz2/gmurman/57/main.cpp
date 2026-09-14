#include <iostream>
#include <random>
using namespace std;

int main() {
    // Вероятности нахождения детали в 4 ящиках: 0.6, 0.7, 0.8, 0.9
    // Ящики независимы
    // а) не более чем в трёх ящиках: 1 - P(во всех четырёх)
    // б) не менее чем в двух ящиках: сумма P(ровно k) для k = 2,3,4
    const double p[4] = {0.6, 0.7, 0.8, 0.9};
    const int TRIALS = 1000000;

    mt19937 rng(random_device{}());
    uniform_real_distribution<double> dist(0.0, 1.0);

    int hitA = 0, hitB = 0;

    for (int t = 0; t < TRIALS; t++) {
        int cnt = 0;
        for (int i = 0; i < 4; i++)
            if (dist(rng) < p[i]) cnt++;

        if (cnt <= 3) hitA++;
        if (cnt >= 2) hitB++;
    }

    // Аналитика
    double pAll = 0.6 * 0.7 * 0.8 * 0.9;
    double pA = 1.0 - pAll;

    double pB = 0.0;
    for (int mask = 0; mask < 16; mask++) {
        int bits = 0;
        double pr = 1.0;
        for (int i = 0; i < 4; i++) {
            if (mask & (1 << i)) { pr *= p[i]; bits++; }
            else                 { pr *= (1 - p[i]); }
        }
        if (bits >= 2) pB += pr;
    }

    cout.precision(6);
    cout << fixed;
    cout << "Task 57:" << endl;
    cout << "  a) emp = " << hitA / (double)TRIALS << "  theo = " << pA << endl;
    cout << "  b) emp = " << hitB / (double)TRIALS << "  theo = " << pB << endl;

    return 0;
}