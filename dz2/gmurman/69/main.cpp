#include <iostream>
#include <random>
using namespace std;

int main() {
    // Студент знает 20 из 25 вопросов
    // Ему задают 3 вопроса
    // Событие: знает все три
    // Теоретически P = C(20,3)/C(25,3) = 1140/2300 = 0.4956...
    const int N = 25;
    const int KNOW = 20;
    const int K = 3;
    const int TRIALS = 1000000;

    int arr[N];
    mt19937 rng(random_device{}());
    int success = 0;

    for (int t = 0; t < TRIALS; t++) {
        // 1 знает, 0 не знает
        for (int i = 0; i < N; i++) arr[i] = (i < KNOW) ? 1 : 0;

        // Перемешивание
        for (int i = N - 1; i > 0; i--) {
            int j = rng() % (i + 1);
            swap(arr[i], arr[j]);
        }

        // Проверяется, что все первые K известны
        bool ok = true;
        for (int i = 0; i < K; i++)
            if (arr[i] == 0) ok = false;

        if (ok) success++;
    }

    cout.precision(6);
    cout << fixed;
    cout << "Trials:      " << TRIALS << endl;
    cout << "Successes:   " << success << endl;
    cout << "Empirical P: " << success / (double)TRIALS << endl;
    cout << "Theoretical: " << 1140.0 / 2300 << endl;

    return 0;
}