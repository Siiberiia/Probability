#include <iostream>
#include <vector>
using namespace std;

// Рекурсивный перебор перестановок
void permute(vector<int>& p, int k, int N, int i, int j,
             long long& total, long long& cntAi, long long& cntAj,
             long long& cntSum, long long& cntProd,
             bool printAll, int& printedSum, int& printedProd) {
    // База: перестановка построена
    if (k == N) {
        total++;

        bool Ai = (p[i - 1] == i);
        bool Aj = (p[j - 1] == j);

        if (Ai) cntAi++;
        if (Aj) cntAj++;
        if (Ai || Aj) {
            cntSum++;
            if (printAll && printedSum < 15) {
                cout << "  sum: ";
                for (int t = 0; t < N; t++) cout << p[t] << " ";
                cout << endl;
                printedSum++;
            }
        }
        if (Ai && Aj) {
            cntProd++;
            if (printAll && printedProd < 15) {
                cout << "  prod: ";
                for (int t = 0; t < N; t++) cout << p[t] << " ";
                cout << endl;
                printedProd++;
            }
        }
        return;
    }

    // На позицию k ставится каждый из оставшихся элементов
    for (int v = 1; v <= N; v++) {
        bool used = false;
        for (int t = 0; t < k; t++)
            if (p[t] == v) used = true;
        if (used) continue;

        p[k] = v;
        permute(p, k + 1, N, i, j, total, cntAi, cntAj,
                cntSum, cntProd, printAll, printedSum, printedProd);
    }
}

int main() {
    // N - число элементов множества
    int N;
    cout << "Enter N: ";
    cin >> N;

    // Фиксированные индексы для проверки теоремы сложения
    const int i = 1;
    const int j = 2;

    vector<int> p(N, 0);

    long long total = 0;
    long long cntAi = 0, cntAj = 0, cntSum = 0, cntProd = 0;

    bool printAll = (N <= 6);
    int printedSum = 0, printedProd = 0;

    permute(p, 0, N, i, j, total, cntAi, cntAj,
            cntSum, cntProd, printAll, printedSum, printedProd);

    cout << "Total permutations: " << total << endl;
    cout << "Empirical:" << endl;
    cout << "  P(Ai)       = " << (double)cntAi   / total << endl;
    cout << "  P(Aj)       = " << (double)cntAj   / total << endl;
    cout << "  P(Ai + Aj)  = " << (double)cntSum  / total << endl;
    cout << "  P(Ai * Aj)  = " << (double)cntProd / total << endl;

    // Аналитика
    double pAi = 1.0 / N;
    double pAj = 1.0 / N;
    double pProd = 1.0 / ((double)N * (N - 1));
    double pSum = pAi + pAj - pProd;

    cout.precision(6);
    cout << fixed;
    cout << "Theoretical:" << endl;
    cout << "  P(Ai)       = " << pAi   << endl;
    cout << "  P(Aj)       = " << pAj   << endl;
    cout << "  P(Ai * Aj)  = " << pProd << endl;
    cout << "  P(Ai + Aj)  = P(Ai) + P(Aj) - P(Ai*Aj) = " << pSum << endl;

    return 0;
}