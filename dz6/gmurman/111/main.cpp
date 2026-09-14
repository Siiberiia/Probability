#include <iostream>
#include <random>
using namespace std;

int main() {
    // Равносильные противники, p = 0.5 на победу в партии
    // Что вероятнее:
    // а) выиграть одну партию из двух или две партии из четырёх
    // б) не менее двух партий из четырёх или не менее трёх из пяти
    // Ничьи не учитываются
    const int TRIALS = 1000000;

    mt19937 rng(random_device{}());
    uniform_real_distribution<double> dist(0.0, 1.0);

    int exact1of2 = 0, exact2of4 = 0;
    int atLeast2of4 = 0, atLeast3of5 = 0;

    for (int t = 0; t < TRIALS; t++) {
        // а) 2 партии
        int w2 = 0;
        for (int i = 0; i < 2; i++) if (dist(rng) < 0.5) w2++;
        if (w2 == 1) exact1of2++;

        // а) 4 партии
        int w4 = 0;
        for (int i = 0; i < 4; i++) if (dist(rng) < 0.5) w4++;
        if (w4 == 2) exact2of4++;
        if (w4 >= 2) atLeast2of4++;

        // б) 5 партий
        int w5 = 0;
        for (int i = 0; i < 5; i++) if (dist(rng) < 0.5) w5++;
        if (w5 >= 3) atLeast3of5++;
    }

    // Аналитика
    // а) P(ровно 1 из 2) = C(2,1)/2^2 = 2/4 = 0.5
    //    P(ровно 2 из 4) = C(4,2)/2^4 = 6/16 = 0.375
    // б) P(>=2 из 4) = 1 - P(0) - P(1) = 1 - (1+4)/16 = 11/16 = 0.6875
    //    P(>=3 из 5) = (C(5,3)+C(5,4)+C(5,5))/32 = (10+5+1)/32 = 16/32 = 0.5
    cout.precision(6);
    cout << fixed;
    cout << "a) P(exactly 1 of 2)   emp = " << exact1of2  / (double)TRIALS
         << "  theo = " << 0.5 << endl;
    cout << "   P(exactly 2 of 4)   emp = " << exact2of4  / (double)TRIALS
         << "  theo = " << 0.375 << endl;
    cout << "   => more likely: ";
    cout << (exact1of2 > exact2of4 ? "1 of 2" : "2 of 4") << endl;

    cout << "b) P(at least 2 of 4)  emp = " << atLeast2of4 / (double)TRIALS
         << "  theo = " << 0.6875 << endl;
    cout << "   P(at least 3 of 5)  emp = " << atLeast3of5 / (double)TRIALS
         << "  theo = " << 0.5 << endl;
    cout << "   => more likely: ";
    cout << (atLeast2of4 > atLeast3of5 ? "at least 2 of 4" : "at least 3 of 5") << endl;

    return 0;
}