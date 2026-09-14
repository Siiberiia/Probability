#include <iostream>
#include <random>
using namespace std;

int main() {
    // Брошены три игральные кости
    // а) на двух выпало одно очко, на третьей другое
    // б) на двух выпало одинаковое, на третьей другое
    // в) на всех разные
    const int TRIALS = 1000000;

    mt19937 rng(random_device{}());
    uniform_int_distribution<int> die(1, 6);

    int hitA = 0, hitB = 0, hitV = 0;

    for (int t = 0; t < TRIALS; t++) {
        int a = die(rng), b = die(rng), c = die(rng);

        // а) ровно две единицы и третья не единица
        int ones = (a == 1) + (b == 1) + (c == 1);
        if (ones == 2) hitA++;

        // б) ровно две одинаковые и третья другая
        bool twoEq = false;
        if (a == b && b != c) twoEq = true;
        if (a == c && a != b) twoEq = true;
        if (b == c && b != a) twoEq = true;
        if (twoEq) hitB++;

        // в) все разные
        if (a != b && b != c && a != c) hitV++;
    }

    // Аналитика: всего 216 исходов
    double pA = 15.0 / 216;   // C(3,2)*1*5
    double pB = 90.0 / 216;   // 6*C(3,2)*5
    double pV = 120.0 / 216;  // 6*5*4

    cout.precision(6);
    cout << fixed;
    cout << "Task 59:" << endl;
    cout << "  a) emp = " << hitA / (double)TRIALS << "  theo = " << pA << endl;
    cout << "  b) emp = " << hitB / (double)TRIALS << "  theo = " << pB << endl;
    cout << "  c) emp = " << hitV / (double)TRIALS << "  theo = " << pV << endl;

    return 0;
}