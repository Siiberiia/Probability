#include <iostream>
#include <random>
using namespace std;

int main() {
    // В семье 5 детей, p(мальчик) = 0.51, q = 0.49
    // a) два мальчика
    // б) не более двух мальчиков
    // в) более двух мальчиков
    // г) не менее двух и не более трёх мальчиков
    const int TRIALS = 1000000;

    mt19937 rng(random_device{}());
    uniform_real_distribution<double> dist(0.0, 1.0);

    int cntA = 0, cntB = 0, cntV = 0, cntG = 0;

    for (int t = 0; t < TRIALS; t++) {
        int boys = 0;
        for (int i = 0; i < 5; i++)
            if (dist(rng) < 0.51) boys++;

        if (boys == 2) cntA++;
        if (boys <= 2) cntB++;
        if (boys > 2)  cntV++;
        if (boys >= 2 && boys <= 3) cntG++;
    }

    // Аналитика по формуле Бернулли: C(5,k) * p^k * q^(5-k)
    double p = 0.51, q = 0.49;
    auto C = [](int n, int k) {
        double r = 1;
        for (int i = 0; i < k; i++) r = r * (n - i) / (i + 1);
        return r;
    };
    auto P = [&](int k) { return C(5, k) * pow(p, k) * pow(q, 5 - k); };

    double pA = P(2);
    double pB = P(0) + P(1) + P(2);
    double pV = 1 - pB;
    double pG = P(2) + P(3);

    cout.precision(6);
    cout << fixed;
    cout << "a) exactly 2 boys:        emp = " << cntA / (double)TRIALS << "  theo = " << pA << endl;
    cout << "b) at most 2 boys:        emp = " << cntB / (double)TRIALS << "  theo = " << pB << endl;
    cout << "c) more than 2 boys:      emp = " << cntV / (double)TRIALS << "  theo = " << pV << endl;
    cout << "d) between 2 and 3 boys:  emp = " << cntG / (double)TRIALS << "  theo = " << pG << endl;

    return 0;
}