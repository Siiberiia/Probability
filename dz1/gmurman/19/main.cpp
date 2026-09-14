#include <iostream>
#include <random>
using namespace std;

int main() {
    // На складе 15 кинескопов, 10 из них Львовского завода
    // Наудачу берутся 5 кинескопов
    // Событие: среди них ровно 3 Львовского завода
    // Теоретически P = C(10,3)*C(5,2)/C(15,5) = 1200/3003 ≈ 0.3996
    const int N = 15;
    const int K = 5;
    const int TRIALS = 1000000;

    int arr[N];
    mt19937 rng(random_device{}());
    int success = 0;

    for (int t = 0; t < TRIALS; t++) {
        // 1 Львовский, 0 другой завод
        for (int i = 0; i < N; i++) arr[i] = (i < 10) ? 1 : 0;

        // Перемешивание
        for (int i = N - 1; i > 0; i--) {
            int j = rng() % (i + 1);
            swap(arr[i], arr[j]);
        }

        // Считается, сколько Львовских в первых K
        int cnt = 0;
        for (int i = 0; i < K; i++)
            if (arr[i] == 1) cnt++;

        if (cnt == 3) success++;
    }

    cout << "Trials:      " << TRIALS << endl;
    cout << "Successes:   " << success << endl;
    cout << "Empirical P: " << (double)success / TRIALS << endl;
    cout << "Theoretical: " << 1200.0 / 3003 << endl;

    return 0;
}