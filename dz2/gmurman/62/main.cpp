#include <iostream>
#include <random>
#include <cmath>
using namespace std;

int main() {
    // В круг радиуса R вписан правильный треугольник
    // Внутри круга брошены 4 точки
    // а) все 4 попали в треугольник
    // б) одна в треугольник, три в каждый из малых сегментов
    const double R = 1.0;
    const int TRIALS = 1000000;

    // Площадь круга
    double SCircle = M_PI * R * R;

    // Сторона правильного вписанного треугольника
    double side = R * sqrt(3.0);

    // Площадь правильного треугольника со стороной a
    double STri = side * side * sqrt(3.0) / 4.0;

    // Площадь одного малого сегмента = (площадь круга - площадь треугольника) / 3
    double SSeg = (SCircle - STri) / 3.0;

    double pTri = STri / SCircle;   // попадание в треугольник
    double pSeg = SSeg / SCircle;   // попадание в один малый сегмент

    // а) все четыре в треугольник
    double pA = pTri * pTri * pTri * pTri;

    // б) одна в треугольник и по одной в каждый из трёх сегментов
    //    учитываем 4! = 24 способа распределить точки по областям
    double pB = 24.0 * pTri * pSeg * pSeg * pSeg;

    cout.precision(6);
    cout << fixed;
    cout << "Area circle = " << SCircle << endl;
    cout << "Area triangle = " << STri << endl;
    cout << "Area segment = " << SSeg << endl;
    cout << "pTri = " << pTri << ", pSeg = " << pSeg << endl;

    // Эмпирика: генерируется точка равномерно в круге (через отбор в квадрате)
    mt19937 rng(random_device{}());
    uniform_real_distribution<double> dx(-R, R), dy(-R, R);

    // Вершины вписанного правильного треугольника
    // Расстояние от центра до вершины = R
    double vx[3], vy[3];
    for (int k = 0; k < 3; k++) {
        double ang = 2 * M_PI * k / 3;
        vx[k] = R * cos(ang);
        vy[k] = R * sin(ang);
    }

    // Проверка: точка внутри треугольника через барицентрические координаты
    auto inTriangle = [&](double x, double y) {
        double d1 = (x - vx[0]) * (vy[1] - vy[0]) - (y - vy[0]) * (vx[1] - vx[0]);
        double d2 = (x - vx[1]) * (vy[2] - vy[1]) - (y - vy[1]) * (vx[2] - vx[1]);
        double d3 = (x - vx[2]) * (vy[0] - vy[2]) - (y - vy[2]) * (vx[0] - vx[2]);
        bool neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
        bool pos = (d1 > 0) || (d2 > 0) || (d3 > 0);
        return !(neg && pos);
    };

    // Определяется зона точки: 0 - треугольник, 1..3 - сегмент, -1 - вне круга
    auto zone = [&](double x, double y) -> int {
        double r2 = x * x + y * y;
        if (r2 > R * R) return -1;
        if (inTriangle(x, y)) return 0;
        // Определяется, какой сегмент: по углу
        double ang = atan2(y, x);
        if (ang < 0) ang += 2 * M_PI;
        // Сегменты против стороны треугольника
        // Пронумеруем сегменты по секторам между вершинами
        int sector = (int)(ang / (2 * M_PI / 3));
        return 1 + sector;
    };

    int hitA = 0, hitB = 0;
    int accepted = 0;

    while (accepted < TRIALS) {
        double xs[4], ys[4];
        int zones[4];
        bool ok = true;

        for (int k = 0; k < 4; k++) {
            double x, y;
            // Отбор точки внутри круга
            do {
                x = dx(rng);
                y = dy(rng);
            } while (x * x + y * y > R * R);

            xs[k] = x; ys[k] = y;
            zones[k] = zone(x, y);
            if (zones[k] < 0) ok = false;
        }
        if (!ok) continue;

        accepted++;

        // а) все в треугольнике
        if (zones[0] == 0 && zones[1] == 0 && zones[2] == 0 && zones[3] == 0)
            hitA++;

        // б) одна в треугольнике, и три в разных сегментах (1, 2, 3)
        int tri = 0, seg[4] = {0, 0, 0, 0};
        for (int k = 0; k < 4; k++) {
            if (zones[k] == 0) tri++;
            else seg[zones[k]]++;
        }
        if (tri == 1 && seg[1] == 1 && seg[2] == 1 && seg[3] == 1)
            hitB++;
    }

    cout << "a) emp = " << hitA / (double)accepted << "  theo = " << pA << endl;
    cout << "b) emp = " << hitB / (double)accepted << "  theo = " << pB << endl;

    return 0;
}