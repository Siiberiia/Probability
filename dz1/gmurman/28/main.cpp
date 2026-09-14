#include <iostream>
#include <random>
using namespace std;

int main() {
    // В круг радиуса R брошена точка
    // Событие: точка попала в меньший круг радиуса r
    // Теоретически P = (r/R)^2
    const double R = 1.0;
    const double r = 0.5;
    const int TRIALS = 1000000;

    mt19937 rng(random_device{}());
    uniform_real_distribution<double> dx(-R, R), dy(-R, R);

    int inBig = 0;   // точек попало в большой круг
    int inSmall = 0; // из них попало в малый круг

    while (inBig < TRIALS) {
        double x = dx(rng), y = dy(rng);

        // Точка попала в большой круг
        if (x * x + y * y <= R * R) {
            inBig++;
            // Проверяется попадание в малый круг
            if (x * x + y * y <= r * r) inSmall++;
        }
    }

    cout << "Trials:      " << TRIALS << endl;
    cout << "Successes:   " << inSmall << endl;
    cout << "Empirical P: " << (double)inSmall / inBig << endl;
    cout << "Theoretical: " << (r / R) * (r / R) << endl;

    return 0;
}