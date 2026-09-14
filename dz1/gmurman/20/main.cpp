#include <iostream>
#include <random>
using namespace std;

int main() {
    // В группе 12 студентов, 8 из них отличники
    // Наудачу отобраны 9 студентов
    // Событие: среди отобранных ровно 5 отличников
    // Теоретически P = C(8,5)*C(4,4)/C(12,9) = 56/220 ≈ 0.2545
    const int N = 12;
    const int K = 9;
    const int TRIALS = 1000000;

    int arr[N];
    mt19937 rng(random_device{}());
    int success = 0;

    for (int t = 0; t < TRIALS; t++) {
        // 1 отличник, 0 не отличник
        for (int i = 0; i < N; i++) arr[i] = (i < 8) ? 1 : 0;

        // Перемешивание
        for (int i = N - 1; i > 0; i--) {
            int j = rng() % (i + 1);
            swap(arr[i], arr[j]);
        }

        // Считается, сколько отличников в первых K
        int cnt = 0;
        for (int i = 0; i < K; i++)
            if (arr[i] == 1) cnt++;

        if (cnt == 5) success++;
    }

    cout << "Trials:      " << TRIALS << endl;
    cout << "Successes:   " << success << endl;
    cout << "Empirical P: " << (double)success / TRIALS << endl;
    cout << "Theoretical: " << 56.0 / 220 << endl;

    return 0;
}