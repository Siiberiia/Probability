#include <iostream>
#include <vector>
#include <string>
#include <random>
using namespace std;

// Кластер - строка из n символов алфавита
using Cluster = string;

// Проверка: соединяемы ли два соседних кластера по паттерну
// Перебираются все разбиения k = a + b, a,b >= 1, a,b <= n
bool canConnect(const Cluster& left, const Cluster& right,
                const string& pattern, int n) {
    int k = (int)pattern.size();

    for (int a = 1; a <= k - 1; a++) {
        int b = k - a;
        if (a > n || b > n) continue;

        // Суффикс левого длины a
        string suff = left.substr(n - a, a);
        // Префикс правого длины b
        string pref = right.substr(0, b);

        if (suff + pref == pattern)
            return true;
    }
    return false;
}

int main() {
    // Параметры
    int M, n;
    string alphabet, pattern;
    int seed;

    cout << "M (number of clusters): ";
    cin >> M;
    cout << "n (cells per cluster): ";
    cin >> n;
    cout << "alphabet (e.g. 01): ";
    cin >> alphabet;
    cout << "k-pattern (e.g. 101): ";
    cin >> pattern;
    cout << "seed (0 for random): ";
    cin >> seed;

    // Генератор
    mt19937 rng(seed == 0 ? random_device{}() : (unsigned)seed);

    // Генерируется серия из M кластеров
    vector<Cluster> clusters(M);
    for (int i = 0; i < M; i++) {
        Cluster c(n, ' ');
        for (int j = 0; j < n; j++)
            c[j] = alphabet[rng() % alphabet.size()];
        clusters[i] = c;
    }

    // Вывод всех кластеров
    cout << "\n--- Clusters ---\n";
    for (int i = 0; i < M; i++)
        cout << "  " << i << ": " << clusters[i] << endl;

    // Перебор всех пар соседей
    cout << "\n--- Adjacent pairs ---\n";
    int totalPairs = 0;
    int connected = 0;

    for (int i = 0; i + 1 < M; i++) {
        totalPairs++;

        bool ok = canConnect(clusters[i], clusters[i + 1], pattern, n);

        cout << "  (" << clusters[i] << " | " << clusters[i + 1] << ") : "
             << (ok ? "connected" : "not connected") << endl;

        if (ok) connected++;
    }

    double p = totalPairs > 0 ? connected / (double)totalPairs : 0.0;

    // Итог
    cout << "\n--- Results ---\n";
    cout << "Total adjacent pairs: " << totalPairs << endl;
    cout << "Connected pairs:      " << connected << endl;
    cout << "Empirical probability: " << p << endl;

    // Список соединяемых и несоединяемых пар
    cout << "\n--- Connected pairs ---\n";
    for (int i = 0; i + 1 < M; i++)
        if (canConnect(clusters[i], clusters[i + 1], pattern, n))
            cout << "  " << i << ": " << clusters[i] << " | " << clusters[i + 1] << endl;

    cout << "\n--- Not connected pairs ---\n";
    for (int i = 0; i + 1 < M; i++)
        if (!canConnect(clusters[i], clusters[i + 1], pattern, n))
            cout << "  " << i << ": " << clusters[i] << " | " << clusters[i + 1] << endl;

    return 0;
}