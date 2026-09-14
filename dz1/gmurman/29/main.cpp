#include <iostream>
#include <random>
using namespace std;

int main() {
    // Плоскость разграфлена параллельными прямыми на расстоянии 2a
    // Брошена монета радиуса r < a
    // Событие: монета не пересечёт ни одной прямой
    // Теоретически P = 1 - r/a
    const double a = 1.0;
    const double r = 0.4;
    const int TRIALS = 1000000;

    mt19937 rng(random_device{}());
    uniform_real_distribution<double> dist(0.0, 2 * a);
    int success = 0;

    for (int t = 0; t < TRIALS; t++) {
        // Случайное положение центра монеты между прямыми
        double y = dist(rng);

        // Расстояние до ближайшей прямой
        double d = min(y, 2 * a - y);

        if (d > r) success++;
    }

    cout << "Trials:      " << TRIALS << endl;
    cout << "Successes:   " << success << endl;
    cout << "Empirical P: " << (double)success / TRIALS << endl;
    cout << "Theoretical: " << 1 - r / a << endl;

    return 0;
}