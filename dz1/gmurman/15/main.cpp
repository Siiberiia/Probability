#include <iostream>
#include <random>
using namespace std;

int main() {
    // Устройство из 5 элементов, 2 из них изношены (3 исправны)
    // Случайно включаются 2 элемента
    // Событие: оба включённых элемента исправны
    // Теоретически P = C(3,2)/C(5,2) = 3/10 = 0.3
    const int N = 5;
    const int K = 2;
    const int TRIALS = 1000000;

    int arr[N];
    mt19937 rng(random_device{}());
    int success = 0;

    for (int t = 0; t < TRIALS; t++) {
        // 1 исправный, 0 изношенный
        for (int i = 0; i < N; i++) arr[i] = (i < 3) ? 1 : 0;

        // Перемешивание
        for (int i = N - 1; i > 0; i--) {
            int j = rng() % (i + 1);
            swap(arr[i], arr[j]);
        }

        // Проверяется, что первые K элементов исправны
        bool ok = true;
        for (int i = 0; i < K; i++)
            if (arr[i] == 0) ok = false;

        if (ok) success++;
    }

    cout << "Trials:      " << TRIALS << endl;
    cout << "Successes:   " << success << endl;
    cout << "Empirical P: " << (double)success / TRIALS << endl;
    cout << "Theoretical: " << 3.0 / 10 << endl;

    return 0;
}