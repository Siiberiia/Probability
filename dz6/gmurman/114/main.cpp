#include <iostream>
#include <random>
using namespace std;

int main() {
    // Устройство из 3 основных элементов, p(отказ) = 0.1 каждый
    // Устройство отказывает, если работает менее трёх элементов
    // а) только 3 основных
    // б) 3 основных + 1 резервный (всего 4, нужно >= 3 рабочих)
    // в) 3 основных + 2 резервных (всего 5, нужно >= 3 рабочих)
    const int TRIALS = 1000000;

    mt19937 rng(random_device{}());
    uniform_real_distribution<double> dist(0.0, 1.0);

    int aOk = 0, bOk = 0, cOk = 0;

    for (int t = 0; t < TRIALS; t++) {
        // а) 3 элемента, все должны работать
        bool a = true;
        for (int i = 0; i < 3; i++)
            if (dist(rng) < 0.1) a = false;
        if (a) aOk++;

        // б) 4 элемента, нужно >= 3 рабочих
        int workB = 0;
        for (int i = 0; i < 4; i++)
            if (dist(rng) >= 0.1) workB++;
        if (workB >= 3) bOk++;

        // в) 5 элементов, нужно >= 3 рабочих
        int workC = 0;
        for (int i = 0; i < 5; i++)
            if (dist(rng) >= 0.1) workC++;
        if (workC >= 3) cOk++;
    }

    // Аналитика (p работы = 0.9, q = 0.1)
    // а) 0.9^3 = 0.729
    // б) P(>=3 из 4) = C(4,3)*0.9^3*0.1 + C(4,4)*0.9^4
    //               = 4*0.729*0.1 + 0.6561
    //               = 0.2916 + 0.6561 = 0.9477
    // в) P(>=3 из 5) = C(5,3)*0.9^3*0.1^2 + C(5,4)*0.9^4*0.1 + C(5,5)*0.9^5
    //               = 10*0.729*0.01 + 5*0.6561*0.1 + 0.59049
    //               = 0.0729 + 0.32805 + 0.59049 = 0.99144
    cout.precision(6);
    cout << fixed;
    cout << "a) 3 main only:        emp = " << aOk / (double)TRIALS
         << "  theo = " << 0.729 << endl;
    cout << "b) 3 main + 1 reserve: emp = " << bOk / (double)TRIALS
         << "  theo = " << 0.9477 << endl;
    cout << "c) 3 main + 2 reserve: emp = " << cOk / (double)TRIALS
         << "  theo = " << 0.99144 << endl;

    return 0;
}