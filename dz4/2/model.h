#pragma once
#include <vector>
#include "distributions.h"

struct Point { double x, y; };

struct Trajectory {
    std::vector<Point> points;
    bool crossedAxis = false;   // пересечение оси абсцисс (смена знака)
    bool touchedZero = false;   // попадание ровно в y = 0 (при l = 0)
};

class TrajectoryModel {
public:
    double h = 1.0;   // шаг по x
    int n = 100;      // число шагов
    double Y = 5.0;   // начальная ордината
    double l = 0.0;   // порог попадания в полосу |y| <= l

    SDistribution dist;

    Trajectory run();
};