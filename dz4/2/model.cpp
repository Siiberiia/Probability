#include "model.h"
#include <cmath>
using namespace std;

Trajectory TrajectoryModel::run() {
    Trajectory tr;
    tr.points.reserve(n + 1);

    double x = 0.0, y = Y;
    tr.points.push_back({x, y});

    bool prevAbove = (y > 0);
    bool prevBelow = (y < 0);

    for (int i = 0; i < n; i++) {
        int s = dist.sample();
        x += h;
        y += s;
        tr.points.push_back({x, y});

        bool above = (y > 0);
        bool below = (y < 0);
        if ((prevAbove && below) || (prevBelow && above))
            tr.crossedAxis = true;

        if (l == 0.0 && y == 0.0)
            tr.touchedZero = true;

        if (l > 0.0 && fabs(y) <= l)
            tr.crossedAxis = true;

        prevAbove = above;
        prevBelow = below;
    }
    return tr;
}