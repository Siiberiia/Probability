#include <iostream>
#include <random>
using namespace std;

int main() {
    // В урне шары с номерами 1..5, извлекаются 3 без возвращения
    // а) последовательно появляются 1, 4, 5
    // б) извлечены шары 1, 4, 5 в любом порядке
    // Теоретически:
    // а) P = 1/(5*4*3) = 1/60
    // б) P = 3!/60 = 6/60 = 1/10
    const int N = 5;
    const int TRIALS = 1000000;

    mt19937 rng(random_device{}());
    int hitA = 0, hitB = 0;

    for (int t = 0; t < TRIALS; t++) {
        int urn[N];
        for (int i = 0; i < N; i++) urn[i] = i + 1;

        // Перемешивание
        for (int i = N - 1; i > 0; i--) {
            int j = rng() % (i + 1);
            swap(urn[i], urn[j]);
        }

        // Первые три шара
        int a = urn[0], b = urn[1], c = urn[2];

        if (a == 1 && b == 4 && c == 5) hitA++;

        // Проверка, что множество {1,4,5}
        bool has1 = (a == 1 || b == 1 || c == 1);
        bool has4 = (a == 4 || b == 4 || c == 4);
        bool has5 = (a == 5 || b == 5 || c == 5);
        if (has1 && has4 && has5) hitB++;
    }

    cout.precision(6);
    cout << fixed;
    cout << "Task 68:" << endl;
    cout << "  a) emp = " << hitA / (double)TRIALS << "  theo = " << 1.0 / 60 << endl;
    cout << "  b) emp = " << hitB / (double)TRIALS << "  theo = " << 6.0 / 60 << endl;

    return 0;
}