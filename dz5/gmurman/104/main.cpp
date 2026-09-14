#include <iostream>
#include <random>
using namespace std;

int main() {
    // Гипотезы B1, B2, B3 образуют полную группу
    // После появления A: P_A(B1) = 0.6, P_A(B2) = 0.3
    // Найти P_A(B3) = ?
    //
    // Аналитика: сумма апостериорных вероятностей = 1
    // P_A(B3) = 1 - 0.6 - 0.3 = 0.1
    //
    // Моделирование: подбираются априорные вероятности и условные P(A|Bi),
    // при которых апостериорные совпадают с заданными.
    // Например, возьмём P(A|B1)=0.6, P(A|B2)=0.3, P(A|B3)=0.1,
    // а априорные P(Bi) = 1/3 для всех. Тогда:
    // P(A) = (1/3)*(0.6 + 0.3 + 0.1) = 1/3
    // P_A(Bi) = P(Bi)*P(A|Bi)/P(A) = P(A|Bi)
    // Значит апостериорные совпадут с условными 0.6, 0.3, 0.1.
    const int TRIALS = 1000000;

    mt19937 rng(random_device{}());
    uniform_int_distribution<int> hyp(0, 2);   // 0,1,2 соответствуют B1,B2,B3
    uniform_real_distribution<double> dist(0.0, 1.0);

    // Условные P(A|Bi)
    double pA[3] = {0.6, 0.3, 0.1};

    int countA[3] = {0, 0, 0};
    int totalA = 0;

    for (int t = 0; t < TRIALS; t++) {
        int b = hyp(rng);   // какая гипотеза реализовалась

        // Появилось ли A при этой гипотезе
        if (dist(rng) < pA[b]) {
            countA[b]++;
            totalA++;
        }
    }

    cout.precision(6);
    cout << fixed;
    cout << "Total A occurrences: " << totalA << endl;
    for (int i = 0; i < 3; i++) {
        double emp = countA[i] / (double)totalA;
        cout << "  P_A(B" << i + 1 << ") = " << emp
             << "  (given: " << pA[i] << ")" << endl;
    }

    double pB3 = 1.0 - 0.6 - 0.3;
    cout << endl;
    cout << "Theoretical P_A(B3) = 1 - P_A(B1) - P_A(B2) = "
         << pB3 << endl;

    return 0;
}