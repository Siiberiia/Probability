#include <iostream>
#include <random>
using namespace std;

int main() {
    // Одна пуля попадает в бочку с вероятностью p
    // При 1 попадании бочка взрывается с вероятностью p1
    // При 2 и более попаданиях бочка взрывается наверняка
    // Найти P(бочка взорвётся при n выстрелах)
    //
    // Аналитика:
    // k - число попаданий из n (биномиальное распределение)
    // P(взрыв) = P(k >= 2) + P(k == 1) * p1
    //          = 1 - P(k=0) - P(k=1) + P(k=1) * p1
    //          = 1 - (1-p)^n - n*p*(1-p)^(n-1) * (1 - p1)

    double p, p1;
    int n;
    cout << "Enter p, p1, n: ";
    cin >> p >> p1 >> n;

    if (n < 1 || p < 0 || p > 1 || p1 < 0 || p1 > 1) {
        cout << "Bad parameters" << endl;
        return 1;
    }

    const int TRIALS = 1000000;

    mt19937 rng(random_device{}());
    uniform_real_distribution<double> dist(0.0, 1.0);

    int success = 0;

    for (int t = 0; t < TRIALS; t++) {
        // Считается число попаданий
        int hits = 0;
        for (int i = 0; i < n; i++)
            if (dist(rng) < p) hits++;

        // Разыгрывается взрыв
        bool exploded = false;
        if (hits >= 2) {
            exploded = true;
        } else if (hits == 1) {
            if (dist(rng) < p1) exploded = true;
        }

        if (exploded) success++;
    }

    // Аналитика
    double pNoHit   = pow(1 - p, n);
    double pOneHit  = n * p * pow(1 - p, n - 1);
    double theo     = 1 - pNoHit - pOneHit + pOneHit * p1;

    double emp = success / (double)TRIALS;

    cout.precision(6);
    cout << fixed;
    cout << "Trials:      " << TRIALS << endl;
    cout << "Successes:   " << success << endl;
    cout << "Empirical P: " << emp << endl;
    cout << "Theoretical: " << theo << endl;

    return 0;
}