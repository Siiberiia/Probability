#include <iostream>
#include <random>
using namespace std;

int main() {
    // Отрезок разделён на три равные части
    // Брошены три точки
    // Событие: на каждую часть попала ровно одна точка
    // Теоретически P = 3! / 3^3 = 6/27 = 2/9
    const int TRIALS = 1000000;

    mt19937 rng(random_device{}());
    uniform_real_distribution<double> dist(0.0, 3.0);

    int success = 0;

    for (int t = 0; t < TRIALS; t++) {
        int cnt[3] = {0, 0, 0};

        for (int k = 0; k < 3; k++) {
            double x = dist(rng);
            int part = (int)x;   // 0, 1 или 2
            if (part > 2) part = 2;
            cnt[part]++;
        }

        if (cnt[0] == 1 && cnt[1] == 1 && cnt[2] == 1) success++;
    }

    double p = 6.0 / 27.0;

    cout.precision(6);
    cout << fixed;
    cout << "Trials:      " << TRIALS << endl;
    cout << "Successes:   " << success << endl;
    cout << "Empirical P: " << success / (double)TRIALS << endl;
    cout << "Theoretical: " << p << endl;

    return 0;
}