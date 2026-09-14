#include "distribution.h"
#include <fstream>
#include <sstream>
#include <numeric>

using namespace std;

DiscreteRV::DiscreteRV() : rng(random_device{}()) {}

bool DiscreteRV::load(const string& filename, string& error) {
    ifstream f(filename);
    if (!f) {
        error = "Cannot open file";
        return false;
    }

    values.clear();
    probs.clear();

    string line;
    while (getline(f, line)) {
        if (line.empty()) continue;
        istringstream ss(line);
        double v, p;
        if (!(ss >> v >> p)) {
            error = "Bad line: " + line;
            return false;
        }
        if (p < 0) {
            error = "Negative probability";
            return false;
        }
        values.push_back(v);
        probs.push_back(p);
    }
    if (values.empty()) {
        error = "Empty distribution";
        return false;
    }
    normalize();
    return true;
}

void DiscreteRV::normalize() {
    double s = 0;
    for (double p : probs) s += p;
    if (s <= 0) return;
    for (double& p : probs) p /= s;
}

double DiscreteRV::sample() {
    discrete_distribution<int> d(probs.begin(), probs.end());
    return values[d(rng)];
}