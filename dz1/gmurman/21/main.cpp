#include <iostream>
#include <random>
using namespace std;

int main() {
    // В коробке 5 изделий, 3 из них окрашены
    // Наудачу извлекаются 2 изделия
    // а) ровно одно окрашенное: P = C(3,1)*C(2,1)/C(5,2) = 6/10 = 0.6
    // б) два окрашенных:       P = C(3,2)/C(5,2)         = 3/10 = 0.3
    // в) хотя бы одно:         P = 1 - C(2,2)/C(5,2)     = 9/10 = 0.9
    const int N = 5;
    const int K = 2;
    const int TRIALS = 1000000;

    int arr[N];
    mt19937 rng(random_device{}());
    int hitA = 0, hitB = 0, hitV = 0;

    for (int t = 0; t < TRIALS; t++) {
        // 1 окрашенное, 0 не окрашенное
        for (int i = 0; i < N; i++) arr[i] = (i < 3) ? 1 : 0;

        // Перемешивание
        for (int i = N - 1; i > 0; i--) {
            int j = rng() % (i + 1);
            swap(arr[i], arr[j]);
        }

        // Считается, сколько окрашенных в первых K
        int cnt = 0;
        for (int i = 0; i < K; i++)
            if (arr[i] == 1) cnt++;

        if (cnt == 1) hitA++;
        if (cnt == 2) hitB++;
        if (cnt >= 1) hitV++;
    }

    cout << "Trials:      " << TRIALS << endl;
    cout << "a) emp = " << (double)hitA / TRIALS << "  theo = 0.6" << endl;
    cout << "b) emp = " << (double)hitB / TRIALS << "  theo = 0.3" << endl;
    cout << "c) emp = " << (double)hitV / TRIALS << "  theo = 0.9" << endl;

    return 0;
}