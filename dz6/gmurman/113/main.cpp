#include <iostream>
#include <random>
using namespace std;

int main() {
    // а) Событие A появляется не менее 3 раз из 4, p = 0.4
    // б) Событие B: A наступит не менее 3 раз из 5, p = 0.8
    const int TRIALS = 1000000;

    mt19937 rng(random_device{}());
    uniform_real_distribution<double> dist(0.0, 1.0);

    int aSuccess = 0, bSuccess = 0;

    for (int t = 0; t < TRIALS; t++) {
        // а) 4 испытания, p = 0.4
        int a = 0;
        for (int i = 0; i < 4; i++) if (dist(rng) < 0.4) a++;
        if (a >= 3) aSuccess++;

        // б) 5 испытаний, p = 0.8
        int b = 0;
        for (int i = 0; i < 5; i++) if (dist(rng) < 0.8) b++;
        if (b >= 3) bSuccess++;
    }

    // Аналитика
    // а) P(>=3 из 4) при p=0.4:
    //    C(4,3)*0.4^3*0.6 + C(4,4)*0.4^4
    //    = 4*0.064*0.6 + 1*0.0256
    //    = 0.1536 + 0.0256 = 0.1792
    // б) P(>=3 из 5) при p=0.8:
    //    = C(5,3)*0.8^3*0.2^2 + C(5,4)*0.8^4*0.2 + C(5,5)*0.8^5
    //    = 10*0.512*0.04 + 5*0.4096*0.2 + 0.32768
    //    = 0.2048 + 0.4096 + 0.32768 = 0.94208
    cout.precision(6);
    cout << fixed;
    cout << "a) P(>=3 of 4), p=0.4: emp = " << aSuccess / (double)TRIALS
         << "  theo = " << 0.1792 << endl;
    cout << "b) P(>=3 of 5), p=0.8: emp = " << bSuccess / (double)TRIALS
         << "  theo = " << 0.94208 << endl;

    return 0;
}