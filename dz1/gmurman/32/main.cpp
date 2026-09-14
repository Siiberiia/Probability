#include <iostream>
#include <random>
using namespace std;

int main() {
    // Две концентрические окружности радиусов r = 5 и R = 10
    // Точка брошена наудачу в большой круг
    // Событие: точка попала в кольцо между окружностями
    // Теоретически P = (R^2 - r^2) / R^2 = (100 - 25) / 100 = 0.75
    const double R = 10.0;
    const double r = 5.0;
    const int TRIALS = 1000000;

    mt19937 rng(random_device{}());
    uniform_real_distribution<double> dx(-R, R), dy(-R, R);

    int inBig = 0;    // точек попало в большой круг
    int inRing = 0;   // из них попало в кольцо

    while (inBig < TRIALS) {
        double x = dx(rng), y = dy(rng);
        double d2 = x * x + y * y;

        // Точка попала в большой круг
        if (d2 <= R * R) {
            inBig++;
            // Проверяется попадание в кольцо
            if (d2 > r * r) inRing++;
        }
    }

    cout << "Trials:      " << TRIALS << endl;
    cout << "Successes:   " << inRing << endl;
    cout << "Empirical P: " << (double)inRing / inBig << endl;
    cout << "Theoretical: " << (R * R - r * r) / (R * R) << endl;

    return 0;
}