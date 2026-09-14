#include <iostream>
#include <random>
using namespace std;

int main() {
    // В конверте 100 карточек, среди них одна нужная
    // Наудачу извлекаются 10 карточек
    // Событие: среди извлечённых оказалась нужная
    // Теоретически P = 10/100 = 0.1
    const int N = 100;
    const int K = 10;
    const int TRIALS = 1000000;

    int deck[N];
    mt19937 rng(random_device{}());
    int success = 0;

    for (int t = 0; t < TRIALS; t++) {
        for (int i = 0; i < N; i++) deck[i] = i; // 0 это нужная карточка

        // Перемешивание
        for (int i = N - 1; i > 0; i--) {
            int j = rng() % (i + 1);
            swap(deck[i], deck[j]);
        }

        // Проверяется, попала ли нужная карточка в первые K
        bool found = false;
        for (int i = 0; i < K; i++)
            if (deck[i] == 0) found = true;

        if (found) success++;
    }

    cout << "Trials:      " << TRIALS << endl;
    cout << "Successes:   " << success << endl;
    cout << "Empirical P: " << (double)success / TRIALS << endl;
    cout << "Theoretical: " << (double)K / N << endl;

    return 0;
}