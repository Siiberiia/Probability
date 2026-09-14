#include <iostream>
#include <random>
using namespace std;

int main() {
    // Три стрелка, вероятности попадания: 0.6, 0.5, 0.4
    // Известно: ровно 2 пули поразили мишень
    // Найти P(третий попал | ровно 2 попадания)
    //
    // Аналитика:
    // P(ровно 2) = p1*p2*(1-p3) + p1*(1-p2)*p3 + (1-p1)*p2*p3
    //            = 0.6*0.5*0.6 + 0.6*0.5*0.4 + 0.4*0.5*0.4
    //            = 0.18 + 0.12 + 0.08 = 0.38
    // Случаи, где 3-й попал и всего ровно 2 попадания:
    //   p1*p2*(1-p3)  - 1 и 2 попали, 3 промахнулся   -> 3-й НЕ попал
    //   p1*(1-p2)*p3  - 1 и 3 попали, 2 промахнулся   -> 3-й попал
    //   (1-p1)*p2*p3  - 2 и 3 попали, 1 промахнулся   -> 3-й попал
    // Значит P(3-й попал и ровно 2) = 0.12 + 0.08 = 0.20
    // P(3-й попал | ровно 2) = 0.20 / 0.38 ≈ 0.5263
    const int TRIALS = 1000000;

    mt19937 rng(random_device{}());
    uniform_real_distribution<double> dist(0.0, 1.0);

    double p[3] = {0.6, 0.5, 0.4};

    int exactlyTwo = 0;             // ровно 2 попадания
    int exactlyTwoWithThird = 0;    // ровно 2, причём третий попал

    for (int t = 0; t < TRIALS; t++) {
        int hits = 0;
        bool thirdHit = false;

        for (int i = 0; i < 3; i++) {
            bool hit = (dist(rng) < p[i]);
            if (hit) {
                hits++;
                if (i == 2) thirdHit = true;
            }
        }

        if (hits == 2) {
            exactlyTwo++;
            if (thirdHit) exactlyTwoWithThird++;
        }
    }

    double emp = exactlyTwoWithThird / (double)exactlyTwo;

    // Аналитика
    double p1 = 0.6, p2 = 0.5, p3 = 0.4;
    double pTwo = p1*p2*(1-p3) + p1*(1-p2)*p3 + (1-p1)*p2*p3;
    double pTwoThird = p1*(1-p2)*p3 + (1-p1)*p2*p3;
    double theo = pTwoThird / pTwo;

    cout.precision(6);
    cout << fixed;
    cout << "Trials:                        " << TRIALS << endl;
    cout << "Exactly 2 hits:                " << exactlyTwo << endl;
    cout << "Exactly 2 hits with third hit: " << exactlyTwoWithThird << endl;
    cout << endl;
    cout << "P(third hit | exactly 2 hits):" << endl;
    cout << "  emp  = " << emp << endl;
    cout << "  theo = " << theo << endl;

    return 0;
}