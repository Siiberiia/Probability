#include <iostream>
#include <random>
using namespace std;

int main() {
    // В каждой урне 6 чёрных и 4 белых шара
    // Из 1-й урны берут шар и кладут во 2-ю
    // Затем из 2-й берут шар и кладут в 3-ю
    // Найти P(шар из 3-й урны — белый)
    const int TRIALS = 1000000;

    // 1 - белый, 0 - чёрный
    mt19937 rng(random_device{}());
    int success = 0;

    for (int t = 0; t < TRIALS; t++) {
        // Урны: массив шаров
        int urn1[10], urn2[10], urn3[10];
        for (int i = 0; i < 10; i++) {
            urn1[i] = (i < 4) ? 1 : 0;  // 4 белых, 6 чёрных
            urn2[i] = (i < 4) ? 1 : 0;
            urn3[i] = (i < 4) ? 1 : 0;
        }

        // Шаг 1: из urn1 берём случайный шар
        int i1 = rng() % 10;
        int ball = urn1[i1];

        // Убираем из urn1, добавляем в urn2 (сдвигаем)
        for (int i = i1; i < 9; i++) urn1[i] = urn1[i + 1];
        urn2[10] = ball;   // условно: добавляем 11-й
        int size2 = 11;

        // Шаг 2: из urn2 (теперь 11 шаров) берём случайный
        int i2 = rng() % size2;
        int ball2 = urn2[i2];

        // Шаг 3: добавляем в urn3 (11 шаров)
        urn3[10] = ball2;
        int size3 = 11;

        // Шаг 4: из urn3 берём случайный шар
        int i3 = rng() % size3;
        if (urn3[i3] == 1) success++;
    }

    double emp = success / (double)TRIALS;

    // Аналитика:
    // После шага 1: во 2-й урне 5 белых + 6 чёрных или 4 белых + 7 чёрных
    // P(белый из 1-й) = 0.4, P(чёрный из 1-й) = 0.6
    // P(белый из 2-й после) =
    //   0.4 * (5/11) + 0.6 * (4/11) = (2 + 2.4)/11 = 4.4/11 = 0.4
    // Значит P(белый во 2-й) = 0.4, независимо от первого шага
    // Аналогично для 3-й урны: P(белый) = 0.4
    double theo = 0.4;

    cout.precision(6);
    cout << fixed;
    cout << "Trials:      " << TRIALS << endl;
    cout << "Successes:   " << success << endl;
    cout << "Empirical P: " << emp << endl;
    cout << "Theoretical: " << theo << endl;

    return 0;
}