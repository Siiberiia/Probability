#include <iostream>
#include <random>
#include <cstdlib>
using namespace std;

int main(int argc, char* argv[]) {
    // Параметры: n - число ключей, K - число экспериментов
    if (argc < 3) {
        cout << "Usage: " << argv[0] << " n K" << endl;
        return 1;
    }

    int n = atoi(argv[1]);   // число ключей
    int K = atoi(argv[2]);   // число экспериментов

    mt19937 rng(random_device{}());

    // Счётчики исходов: на каком испытании закончился опыт
    // count[k] - сколько раз процесс завершился на (k+1)-м испытании
    int* count = new int[n]();

    for (int t = 0; t < K; t++) {
        // Мешок ключей, метка 1 у подходящего
        int* keys = new int[n];
        for (int i = 0; i < n; i++) keys[i] = 0;
        keys[0] = 1; // один подходящий

        // Перемешивание
        for (int i = n - 1; i > 0; i--) {
            int j = rng() % (i + 1);
            swap(keys[i], keys[j]);
        }

        // Проверяем ключи по одному, пока не найдём подходящий
        int found = 0;
        for (int i = 0; i < n; i++) {
            if (keys[i] == 1) {
                found = i + 1; // номер испытания
                break;
            }
        }

        count[found - 1]++;

        // Показывается, чем закончился эксперимент
        cout << "Experiment " << t + 1 << ": finished at trial " << found << endl;

        delete[] keys;
    }

    // Эмпирические вероятности
    cout << "\n--- Empirical probabilities ---" << endl;
    cout.precision(6);
    cout << fixed;
    for (int k = 0; k < n; k++) {
        cout << "  Outcome " << k + 1 << ": P = " << count[k] / (double)K << endl;
    }

    cout << "\nTheoretical: P = " << 1.0 / n << " for each outcome" << endl;

    delete[] count;
    return 0;
}