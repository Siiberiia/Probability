#include <iostream>
#include <random>
using namespace std;

int main() {
    // В коробке 6 пронумерованных кубиков (номера 1..6)
    // Извлекаются все 6 по одному наудачу, получается перестановка
    // Событие: номера появились в возрастающем порядке
    // Теоретически P = 1/6! = 1/720
    const int N = 6;
    const int TRIALS = 10000000;

    int box[N];
    mt19937 rng(random_device{}());
    int success = 0;

    for (int t = 0; t < TRIALS; t++) {
        for (int i = 0; i < N; i++) box[i] = i + 1;

        // Перемешивание
        for (int i = N - 1; i > 0; i--) {
            int j = rng() % (i + 1);
            swap(box[i], box[j]);
        }

        // Проверяется, что порядок возрастающий
        bool ok = true;
        for (int i = 1; i < N; i++)
            if (box[i] < box[i - 1]) ok = false;

        if (ok) success++;
    }

    cout << "Trials:      " << TRIALS << endl;
    cout << "Successes:   " << success << endl;
    cout << "Empirical P: " << (double)success / TRIALS << endl;
    cout << "Theoretical: " << 1.0 / 720 << endl;

    return 0;
}