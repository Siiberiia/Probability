#include <iostream>
#include <random>
using namespace std;

int main() {
    // 10 винтовок, 4 с оптикой, 6 без
    // P(попасть | с оптикой)   = 0.95
    // P(попасть | без оптики)  = 0.8
    // Известно, что попадание произошло. Что вероятнее: стреляли с оптикой или без?
    //
    // Формула Байеса:
    // P(оптика | попал) = (0.4 * 0.95) / (0.4 * 0.95 + 0.6 * 0.8)
    //                   = 0.38 / (0.38 + 0.48) = 0.38 / 0.86 ≈ 0.4419
    // P(без   | попал) = 0.48 / 0.86 ≈ 0.5581
    // Значит вероятнее, что стреляли БЕЗ оптики
    const int TRIALS = 1000000;

    mt19937 rng(random_device{}());
    uniform_int_distribution<int> rifle(0, 9);   // 0..3 - с оптикой, 4..9 - без
    uniform_real_distribution<double> dist(0.0, 1.0);

    int hitWithOptic = 0;   // попаданий с оптикой
    int hitWithout   = 0;   // попаданий без оптики
    int totalHits    = 0;

    for (int t = 0; t < TRIALS; t++) {
        int r = rifle(rng);
        bool withOptic = (r < 4);

        double pHit = withOptic ? 0.95 : 0.8;

        // Учитываются только попадания
        if (dist(rng) < pHit) {
            totalHits++;
            if (withOptic) hitWithOptic++;
            else           hitWithout++;
        }
    }

    double pWith    = hitWithOptic / (double)totalHits;
    double pWithout = hitWithout   / (double)totalHits;

    // Аналитика (формула Байеса)
    double a = 0.4 * 0.95;   // P(оптика и попал)
    double b = 0.6 * 0.8;    // P(без и попал)
    double theoWith    = a / (a + b);
    double theoWithout = b / (a + b);

    cout.precision(6);
    cout << fixed;
    cout << "Total hits:  " << totalHits << endl;
    cout << "Hits with optic:    " << hitWithOptic << endl;
    cout << "Hits without optic: " << hitWithout << endl;
    cout << endl;
    cout << "P(with optic | hit):    emp = " << pWith
         << "  theo = " << theoWith << endl;
    cout << "P(without optic | hit): emp = " << pWithout
         << "  theo = " << theoWithout << endl;
    cout << endl;
    if (pWithout > pWith)
        cout << "More likely: WITHOUT optic" << endl;
    else
        cout << "More likely: WITH optic" << endl;

    return 0;
}