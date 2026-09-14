#include <iostream>
#include <random>
using namespace std;

int main() {
    // Отрезок разделён на 4 равные части
    // Брошены 8 точек, каждая равновероятно попадает в одну из 4 частей
    // Событие: в каждую часть попало ровно 2 точки
    // Полиномиальное распределение
    const int TRIALS = 1000000;

    mt19937 rng(random_device{}());
    uniform_int_distribution<int> part(0, 3);

    int success = 0;

    for (int t = 0; t < TRIALS; t++) {
        int cnt[4] = {0, 0, 0, 0};
        for (int i = 0; i < 8; i++)
            cnt[part(rng)]++;

        if (cnt[0] == 2 && cnt[1] == 2 && cnt[2] == 2 && cnt[3] == 2)
            success++;
    }

    // Аналитика: 8! / (2!^4) / 4^8 = 2520 / 65536
    double theo = 2520.0 / 65536.0;

    cout.precision(6);
    cout << fixed;
    cout << "emp  = " << success / (double)TRIALS << endl;
    cout << "theo = " << theo << endl;

    return 0;
}