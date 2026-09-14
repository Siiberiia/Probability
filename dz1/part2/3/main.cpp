#include <iostream>
#include <random>
using namespace std;

int main() {
    // Монета бросается до тех пор, пока 2 раза подряд не выпадет одна сторона
    // a) P(опыт закончится до k-го броска) = 1 - P(первые k-1 бросков чередуются)
    //    P(чередование в n бросках) = 2/2^n = 1/2^(n-1)
    //    Значит P(до k) = 1 - 1/2^(k-2) для k >= 2
    // б) P(чётное число бросков) = 2/3
    int k;
    cout << "Enter k: ";
    cin >> k;

    const int TRIALS = 1000000;

    mt19937 rng(random_device{}());
    uniform_int_distribution<int> coin(0, 1);

    int successA = 0;   // опыт закончился до k-го броска
    int successB = 0;   // потребовалось чётное число бросков
    int evenCount = 0;  // сколько опытов закончилось за чётное число

    // Сумма для чётности (только для завершённых опытов)
    int totalA = 0;

    for (int t = 0; t < TRIALS; t++) {
        int prev = coin(rng);
        int cur = coin(rng);
        int throws = 2;

        // Бросаем пока две подряд стороны не совпадут
        while (cur == prev) {
            prev = cur;
            cur = coin(rng);
            throws++;
        }

        // Событие а: завершилось до k-го броска
        if (throws < k) successA++;

        // Событие б: чётное число бросков
        if (throws % 2 == 0) successB++;
    }

    // Аналитика
    double pA = (k >= 3) ? 1.0 - 1.0 / (1 << (k - 2)) : 0.0;
    if (k < 2) pA = 0.0;
    double pB = 2.0 / 3.0;

    cout.precision(6);
    cout << fixed;
    cout << "Trials:      " << TRIALS << endl;
    cout << "a) P(end < k):  emp = " << successA / (double)TRIALS
         << "  theo = " << pA << endl;
    cout << "b) P(even):     emp = " << successB / (double)TRIALS
         << "  theo = " << pB << endl;

    return 0;
}