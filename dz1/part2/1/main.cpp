#include <iostream>
#include <random>
using namespace std;

int main() {
    // N - мощность алфавита, C - число экспериментов
    int N, C;
    cout << "Enter N and C: ";
    cin >> N >> C;

    mt19937 rng(random_device{}());

    // Счётчики для случая "индексы не изменяются" (а)
    int hitA1 = 0, hitA2 = 0, hitA12 = 0;

    // Счётчики для случая "индексы изменяются" (б)
    int hitB1 = 0, hitB2 = 0, hitB12 = 0;

    for (int t = 0; t < C; t++) {
        // Первый выбор: равномерно из N символов, индексы 1..N
        int first = 1 + rng() % N;

        // Второй выбор: из оставшихся, индекс 1..N кроме first
        int idx = rng() % (N - 1);
        int second = idx + 1;
        if (second >= first) second++;

        // Случай а: чётность определяется исходной индексацией 1..N
        bool a1 = (first % 2 == 0);
        bool a2 = (second % 2 == 0);
        if (a1) hitA1++;
        if (a2) hitA2++;
        if (a1 && a2) hitA12++;

        // Случай б: после первой выборки индексы оставшихся пересчитываются
        // То есть второй символ имеет новый индекс 1..N-1
        int newSecond = (second < first) ? second : second - 1;
        bool b1 = (first % 2 == 0);
        bool b2 = (newSecond % 2 == 0);
        if (b1) hitB1++;
        if (b2) hitB2++;
        if (b1 && b2) hitB12++;
    }

    cout << "N = " << N << ", C = " << C << endl;

    // Аналитика для случая а (исходные индексы 1..N)
    double pA1 = (N / 2) / (double)N;
    double pA2 = 0.0;
    {
        int even = N / 2, odd = N - even;
        if (even > 0) pA2 += (even / (double)N) * ((even - 1) / (double)(N - 1));
        if (odd > 0)  pA2 += (odd  / (double)N) * (even       / (double)(N - 1));
    }
    double pA12 = (N / 2) / (double)N * (((N / 2) - 1) / (double)(N - 1));

    // Аналитика для случая б (после первой выборки индексы сжимаются)
    double pB1 = (N / 2) / (double)N;
    double pB2 = ((N - 1) / 2) / (double)(N - 1);
    double pB12 = 0.0;
    {
        for (int first = 1; first <= N; first++) {
            if (first % 2 != 0) continue;
            int cntEven = 0;
            int k = 0;
            for (int s = 1; s <= N; s++) {
                if (s == first) continue;
                k++;
                if (k % 2 == 0) cntEven++;
            }
            pB12 += cntEven / (double)(N - 1);
        }
        pB12 /= N;
    }

    cout.precision(6);
    cout << fixed;
    cout << "Case a (indices unchanged):" << endl;
    cout << "  a) first even:  emp = " << hitA1  / (double)C << "  theo = " << pA1  << endl;
    cout << "  b) second even: emp = " << hitA2  / (double)C << "  theo = " << pA2  << endl;
    cout << "  c) both even:   emp = " << hitA12 / (double)C << "  theo = " << pA12 << endl;

    cout << "Case b (indices renumbered):" << endl;
    cout << "  a) first even:  emp = " << hitB1  / (double)C << "  theo = " << pB1  << endl;
    cout << "  b) second even: emp = " << hitB2  / (double)C << "  theo = " << pB2  << endl;
    cout << "  c) both even:   emp = " << hitB12 / (double)C << "  theo = " << pB12 << endl;

    return 0;
}