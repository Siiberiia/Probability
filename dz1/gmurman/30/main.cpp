#include <iostream>
#include <random>
using namespace std;

int main() {
    // Плоскость с сеткой квадратов со стороной a
    // Брошена монета радиуса r < a/2
    // Событие: монета не пересечёт ни одной стороны квадрата
    // Теоретически P = (a - 2r)^2 / a^2
    const double a = 1.0;
    const double r = 0.3;
    const int TRIALS = 1000000;

    mt19937 rng(random_device{}());
    uniform_real_distribution<double> dist(0.0, a);
    int success = 0;

    for (int t = 0; t < TRIALS; t++) {
        // Случайное положение центра монеты внутри квадрата
        double x = dist(rng), y = dist(rng);

        // Расстояние до ближайшей стороны по каждой оси
        double dx = min(x, a - x);
        double dy = min(y, a - y);

        if (dx > r && dy > r) success++;
    }

    cout << "Trials:      " << TRIALS << endl;
    cout << "Successes:   " << success << endl;
    cout << "Empirical P: " << (double)success / TRIALS << endl;
    cout << "Theoretical: " << (a - 2 * r) * (a - 2 * r) / (a * a) << endl;

    return 0;
}