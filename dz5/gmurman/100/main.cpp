#include <iostream>
#include <random>
using namespace std;

int main() {
    // Две перфораторщицы набили одинаковые комплекты перфокарт
    // P(ошибка у первой) = 0.05
    // P(ошибка у второй) = 0.1
    // Ошибки независимы
    // При сверке обнаружена ошибка (хотя бы одна из двух допустила ошибку)
    // Найти P(ошиблась именно первая | есть ошибка)
    //
    // Аналитика:
    // P(A) = P(ошибка у 1-й) = 0.05
    // P(B) = P(ошибка у 2-й) = 0.1
    // P(A и B) = 0.05 * 0.1 = 0.005   (независимы)
    // P(есть ошибка) = P(A ∪ B) = 0.05 + 0.1 - 0.005 = 0.145
    // P(A | A ∪ B) = 0.05 / 0.145 ≈ 0.3448
    const int TRIALS = 1000000;

    mt19937 rng(random_device{}());
    uniform_real_distribution<double> dist(0.0, 1.0);

    int errFirst = 0;      // ошибка у первой
    int errSecond = 0;     // ошибка у второй
    int hasError = 0;      // хотя бы одна ошибка

    for (int t = 0; t < TRIALS; t++) {
        bool A = dist(rng) < 0.05;
        bool B = dist(rng) < 0.1;

        if (A) errFirst++;
        if (B) errSecond++;
        if (A || B) hasError++;
    }

    // Условная вероятность: среди опытов с ошибкой - доля тех, где ошибалась первая
    int firstAndError = 0;
    for (int t = 0; t < TRIALS; t++) {
        bool A = dist(rng) < 0.05;
        bool B = dist(rng) < 0.1;
        if ((A || B) && A) firstAndError++;
    }

    // Аналитика
    double pA = 0.05;
    double pB = 0.1;
    double pAB = pA * pB;
    double pUnion = pA + pB - pAB;
    double theo = pA / pUnion;

    cout.precision(6);
    cout << fixed;
    cout << "Trials:               " << TRIALS << endl;
    cout << "Error at first:       " << errFirst << endl;
    cout << "Error at second:      " << errSecond << endl;
    cout << "At least one error:   " << hasError << endl;
    cout << endl;
    cout << "P(first erred | error detected):" << endl;
    cout << "  emp  = " << firstAndError / (double)hasError << endl;
    cout << "  theo = " << theo << endl;

    return 0;
}