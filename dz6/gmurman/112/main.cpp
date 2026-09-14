#include <iostream>
#include <random>
using namespace std;

int main() {
    // Монета брошена 7 раз, p(герб) = 0.5
    // а) герб выпадет менее двух раз
    // б) герб выпадет не менее двух раз
    const int TRIALS = 1000000;

    mt19937 rng(random_device{}());
    uniform_real_distribution<double> dist(0.0, 1.0);

    int less2 = 0, atLeast2 = 0;

    for (int t = 0; t < TRIALS; t++) {
        int heads = 0;
        for (int i = 0; i < 7; i++) if (dist(rng) < 0.5) heads++;

        if (heads < 2) less2++;
        if (heads >= 2) atLeast2++;
    }

    // Аналитика
    // а) P(0 гербов) + P(1 герб) = (1 + 7) / 128 = 8/128 = 1/16 = 0.0625
    // б) 1 - 0.0625 = 0.9375
    cout.precision(6);
    cout << fixed;
    cout << "a) P(<2 heads)  emp = " << less2    / (double)TRIALS
         << "  theo = " << 0.0625 << endl;
    cout << "b) P(>=2 heads) emp = " << atLeast2 / (double)TRIALS
         << "  theo = " << 0.9375 << endl;

    return 0;
}