#include <iostream>
#include <random>
using namespace std;

int main() {
    // В мешочке кубики с номерами 1..10
    // Извлекаются 3 по одному
    // Событие: последовательно появляются 1, 2, 3
    // а) без возвращения:  P = 1/(10*9*8) = 1/720
    // б) с возвращением:   P = 1/(10*10*10) = 1/1000
    const int N = 10;
    const int TRIALS = 1000000;

    mt19937 rng(random_device{}());
    int hitA = 0, hitB = 0;

    // а) без возвращения
    for (int t = 0; t < TRIALS; t++) {
        int urn[N];
        for (int i = 0; i < N; i++) urn[i] = i + 1;

        for (int i = N - 1; i > 0; i--) {
            int j = rng() % (i + 1);
            swap(urn[i], urn[j]);
        }

        if (urn[0] == 1 && urn[1] == 2 && urn[2] == 3) hitA++;
    }

    // б) с возвращением
    for (int t = 0; t < TRIALS; t++) {
        int a = 1 + rng() % N;
        int b = 1 + rng() % N;
        int c = 1 + rng() % N;

        if (a == 1 && b == 2 && c == 3) hitB++;
    }

    cout.precision(8);
    cout << fixed;
    cout << "Task 70:" << endl;
    cout << "  a) emp = " << hitA / (double)TRIALS << "  theo = " << 1.0 / 720 << endl;
    cout << "  b) emp = " << hitB / (double)TRIALS << "  theo = " << 1.0 / 1000 << endl;

    return 0;
}