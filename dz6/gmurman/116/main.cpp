#include <iostream>
#include <random>
using namespace std;

int main() {
    // Отрезок AB разделён точкой C в отношении 2:1
    // То есть AC : CB = 2 : 1, значит AC = 2/3 AB, CB = 1/3 AB
    // Брошены 4 точки, каждая равномерно на AB
    // Событие: 2 точки левее C и 2 правее C
    // P(точка левее C) = 2/3, P(правее C) = 1/3
    const int TRIALS = 1000000;

    mt19937 rng(random_device{}());
    uniform_real_distribution<double> dist(0.0, 1.0);

    int success = 0;
    const double p = 2.0 / 3.0;

    for (int t = 0; t < TRIALS; t++) {
        int left = 0;
        for (int i = 0; i < 4; i++)
            if (dist(rng) < p) left++;

        if (left == 2) success++;
    }

    // Аналитика: C(4,2) * (2/3)^2 * (1/3)^2 = 6 * 4/9 * 1/9 = 24/81
    double theo = 6.0 * (2.0/3) * (2.0/3) * (1.0/3) * (1.0/3);

    cout.precision(6);
    cout << fixed;
    cout << "emp  = " << success / (double)TRIALS << endl;
    cout << "theo = " << theo << endl;

    return 0;
}