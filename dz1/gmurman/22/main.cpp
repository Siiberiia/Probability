#include <iostream>
#include <random>
using namespace std;

int main() {
    // Замок из 4 дисков, каждый имеет 5 секторов
    // Диски ставятся произвольно, всего 5^4 = 625 комбинаций
    // Замок открывается только при одной конкретной комбинации
    // Теоретически P = 1/625 = 0.0016
    const int DISKS = 4;
    const int SECTORS = 5;
    const int TRIALS = 1000000;

    mt19937 rng(random_device{}());
    int success = 0;

    // Загаданная комбинация, например 0 0 0 0
    int secret[DISKS] = {0, 0, 0, 0};

    for (int t = 0; t < TRIALS; t++) {
        // Случайная установка дисков
        int code[DISKS];
        for (int i = 0; i < DISKS; i++)
            code[i] = rng() % SECTORS;

        // Проверяется, совпала ли с загаданной
        bool ok = true;
        for (int i = 0; i < DISKS; i++)
            if (code[i] != secret[i]) ok = false;

        if (ok) success++;
    }

    cout << "Trials:      " << TRIALS << endl;
    cout << "Successes:   " << success << endl;
    cout << "Empirical P: " << (double)success / TRIALS << endl;
    cout << "Theoretical: " << 1.0 / 625 << endl;

    return 0;
}