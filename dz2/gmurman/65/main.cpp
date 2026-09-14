#include <iostream>
#include <random>
using namespace std;

int main() {
    // Среди 100 билетов 5 выигрышных
    // Наудачу выбираются 2 билета
    // Событие: оба выигрышные
    // Теоретически P = C(5,2)/C(100,2) = 10/4950 = 1/495
    const int N = 100;
    const int WIN = 5;
    const int K = 2;
    const int TRIALS = 1000000;

    int arr[N];
    mt19937 rng(random_device{}());
    int success = 0;

    for (int t = 0; t < TRIALS; t++) {
        // 1 выигрышный, 0 проигрышный
        for (int i = 0; i < N; i++) arr[i] = (i < WIN) ? 1 : 0;

        // Перемешивание
        for (int i = N - 1; i > 0; i--) {
            int j = rng() % (i + 1);
            swap(arr[i], arr[j]);
        }

        // Считается, сколько выигрышных в первых K
        int cnt = 0;
        for (int i = 0; i < K; i++)
            if (arr[i] == 1) cnt++;

        if (cnt == 2) success++;
    }

    cout << "Trials:      " << TRIALS << endl;
    cout << "Successes:   " << success << endl;
    cout << "Empirical P: " << (double)success / TRIALS << endl;
    cout << "Theoretical: " << 10.0 / 4950 << endl;

    return 0;
}