#include <iostream>
#include <random>
using namespace std;

int main() {
    // Четыре независимо работающих лампы
    // Вероятности отказа: p1=0.1, p2=0.2, p3=0.3, p4=0.4
    // Известно: ровно 2 лампы отказали
    // Найти P(отказали первая и вторая | ровно 2 отказа)
    //
    // Аналитика: перебираются все C(4,2)=6 пар отказавших
    // Веса пар (произведения вероятностей отказа * вероятностей работы остальных):
    // 1,2: 0.1*0.2*0.7*0.6 = 0.0084
    // 1,3: 0.1*0.8*0.3*0.6 = 0.0144
    // 1,4: 0.1*0.8*0.7*0.4 = 0.0224
    // 2,3: 0.9*0.2*0.3*0.6 = 0.0324
    // 2,4: 0.9*0.2*0.7*0.4 = 0.0504
    // 3,4: 0.9*0.8*0.3*0.4 = 0.0864
    // Сумма = 0.2144
    // P(пара 1,2 | ровно 2) = 0.0084 / 0.2144 ≈ 0.03918
    const int TRIALS = 1000000;

    mt19937 rng(random_device{}());
    uniform_real_distribution<double> dist(0.0, 1.0);

    double p[4] = {0.1, 0.2, 0.3, 0.4};

    int exactlyTwo = 0;
    int exactlyTwoFirstSecond = 0;

    for (int t = 0; t < TRIALS; t++) {
        bool fail[4];
        int cnt = 0;

        for (int i = 0; i < 4; i++) {
            fail[i] = (dist(rng) < p[i]);
            if (fail[i]) cnt++;
        }

        if (cnt == 2) {
            exactlyTwo++;
            if (fail[0] && fail[1]) exactlyTwoFirstSecond++;
        }
    }

    double emp = exactlyTwoFirstSecond / (double)exactlyTwo;

    // Аналитика
    double q[4] = {1 - p[0], 1 - p[1], 1 - p[2], 1 - p[3]};
    double sum = 0.0;
    for (int i = 0; i < 4; i++)
        for (int j = i + 1; j < 4; j++) {
            double w = 1.0;
            for (int k = 0; k < 4; k++) {
                if (k == i || k == j) w *= p[k];
                else w *= q[k];
            }
            sum += w;
        }
    double w12 = p[0] * p[1] * q[2] * q[3];
    double theo = w12 / sum;

    cout.precision(6);
    cout << fixed;
    cout << "Trials:                   " << TRIALS << endl;
    cout << "Exactly two failures:     " << exactlyTwo << endl;
    cout << "Failures 1 and 2:         " << exactlyTwoFirstSecond << endl;
    cout << endl;
    cout << "P(first and second | exactly two):" << endl;
    cout << "  emp  = " << emp << endl;
    cout << "  theo = " << theo << endl;

    return 0;
}