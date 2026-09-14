#include <iostream>
#include <random>
#include <cmath>
using namespace std;

int main() {
    // n монет: k с орлом на обеих сторонах, n-k честных
    // Наудачу выбирается монета и подбрасывается 4 раза
    // Найти P(орёл на 4-м | первые три броска — орлы)
    //
    // Аналитика:
    // H1 - двусторонняя (P = k/n),    P(3 орла | H1) = 1
    // H2 - честная      (P = (n-k)/n), P(3 орла | H2) = 1/8
    // P(3 орла) = k/n + (n-k)/n * 1/8
    // P(4 орла) = k/n + (n-k)/n * 1/16
    // Ответ = P(4 орла) / P(3 орла)

    int n, k;
    cout << "Enter n (total coins) and k (double-headed): ";
    cin >> n >> k;

    if (n < 1 || k < 0 || k > n) {
        cout << "Bad parameters" << endl;
        return 1;
    }

    const int TRIALS = 1000000;

    mt19937 rng(random_device{}());
    uniform_int_distribution<int> coinIdx(0, n - 1);
    uniform_real_distribution<double> dist(0.0, 1.0);

    int threeHeads = 0;
    int fourHeads = 0;

    for (int t = 0; t < TRIALS; t++) {
        int c = coinIdx(rng);
        bool doubleHeaded = (c < k);

        bool h[4];
        for (int i = 0; i < 4; i++) {
            if (doubleHeaded) h[i] = true;
            else              h[i] = (dist(rng) < 0.5);
        }

        // Отбираются серии, где ПЕРВЫЕ ТРИ броска — орлы
        if (h[0] && h[1] && h[2]) {
            threeHeads++;
            if (h[3]) fourHeads++;
        }
    }

    double emp = fourHeads / (double)threeHeads;

    // Аналитика
    double p3 = (double)k / n + (double)(n - k) / n * (1.0 / 8);
    double p4 = (double)k / n + (double)(n - k) / n * (1.0 / 16);
    double theo = p4 / p3;

    cout.precision(6);
    cout << fixed;
    cout << "Trials:        " << TRIALS << endl;
    cout << "First 3 heads: " << threeHeads << endl;
    cout << "All 4 heads:   " << fourHeads << endl;
    cout << endl;
    cout << "P(head on 4th | first 3 heads):" << endl;
    cout << "  emp  = " << emp << endl;
    cout << "  theo = " << theo << endl;

    return 0;
}