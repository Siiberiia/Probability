#include <iostream>
#include <random>
using namespace std;

int main() {
    // Брошены три игральные кости
    // а) на каждой выпало 5 очков
    // б) на всех выпало одинаковое число очков
    const int TRIALS = 1000000;

    mt19937 rng(random_device{}());
    uniform_int_distribution<int> die(1, 6);

    int hitA = 0, hitB = 0;

    for (int t = 0; t < TRIALS; t++) {
        int a = die(rng), b = die(rng), c = die(rng);

        if (a == 5 && b == 5 && c == 5) hitA++;
        if (a == b && b == c) hitB++;
    }

    // Всего 6^3 = 216 исходов
    double pA = 1.0 / 216;
    double pB = 6.0 / 216;

    cout.precision(6);
    cout << fixed;
    cout << "Task 58:" << endl;
    cout << "  a) emp = " << hitA / (double)TRIALS << "  theo = " << pA << endl;
    cout << "  b) emp = " << hitB / (double)TRIALS << "  theo = " << pB << endl;

    return 0;
}