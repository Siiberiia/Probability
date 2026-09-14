#include <iostream>
#include <vector>
#include <random>
#include <iomanip>

int main() {
    // Куб 10x10x10 распилен на 1000 кубиков
    // Позиция (i, j, k), i,j,k = 0..9
    // Грань окрашена, если координата равна 0 или 9
    const int N = 10;
    const int TOTAL = 1000;
    const long long TRIALS = 1000000;

    // Мешок кубиков: хранится число окрашенных граней
    std::vector<int> box;
    box.reserve(TOTAL);

    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            for (int k = 0; k < N; ++k) {
                int painted = 0;
                if (i == 0 || i == N - 1) ++painted;
                if (j == 0 || j == N - 1) ++painted;
                if (k == 0 || k == N - 1) ++painted;
                box.push_back(painted);
            }

    // Считается, сколько кубиков с 0,1,2,3 окрашенными гранями
    long long cnt[4] = {0, 0, 0, 0};
    for (int p : box) cnt[p]++;

    std::cout << "Bag composition (theoretical):\n";
    for (int p = 0; p <= 3; ++p)
        std::cout << "  faces=" << p << ": " << cnt[p]
                  << "  P=" << cnt[p] / (double)TOTAL << "\n";
    std::cout << "----------------------------------\n";

    // Эксперимент: извлекается случайный кубик, считается число граней
    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> dist(0, TOTAL - 1);

    long long hits[4] = {0, 0, 0, 0};

    for (long long t = 0; t < TRIALS; ++t) {
        int idx = dist(rng);
        hits[box[idx]]++;
    }

    std::cout << "Empirical results (" << TRIALS << " trials):\n";
    std::cout << std::fixed << std::setprecision(4);
    for (int k = 1; k <= 3; ++k) {
        double emp  = hits[k] / (double)TRIALS;
        double theo = cnt[k]  / (double)TOTAL;
        std::cout << "  k=" << k << ": emp P=" << emp
                  << "  theo P=" << theo << "\n";
    }

    return 0;
}