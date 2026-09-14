#include <iostream>
#include <random>
using namespace std;

int main() {
    // В ящике 10 деталей, 6 окрашенных
    // Извлекаются 4 детали
    // Событие: все 4 окрашенные
    // Теоретически P = C(6,4)/C(10,4) = 15/210 = 1/14
    const int N = 10;
    const int K = 6;
    const int M = 4;
    const int TRIALS = 1000000;

    int arr[N];
    mt19937 rng(random_device{}());
    int success = 0;

    for (int t = 0; t < TRIALS; t++) {
        // 1 окрашенная, 0 неокрашенная
        for (int i = 0; i < N; i++) arr[i] = (i < K) ? 1 : 0;

        // Перемешивание
        for (int i = N - 1; i > 0; i--) {
            int j = rng() % (i + 1);
            swap(arr[i], arr[j]);
        }

        // Проверяется, что все первые M окрашенные
        bool ok = true;
        for (int i = 0; i < M; i++)
            if (arr[i] == 0) ok = false;

        if (ok) success++;
    }

    cout << "Trials:      " << TRIALS << endl;
    cout << "Successes:   " << success << endl;
    cout << "Empirical P: " << (double)success / TRIALS << endl;
    cout << "Theoretical: " << 15.0 / 210 << endl;

    return 0;
}