#include "distributions.h"
#include <vector>
using namespace std;

SDistribution::SDistribution() : rng(random_device{}()) {}

int SDistribution::sample() {
    switch (kind) {
        case DistKind::Uniform: {
            uniform_int_distribution<int> d(a, b);
            return d(rng);
        }
        case DistKind::Binomial: {
            binomial_distribution<int> d(nBin, p);
            return d(rng);
        }
        case DistKind::Geometric: {
            geometric_distribution<int> d(p);
            int val = d(rng) - 1;
            if (val > maxVal) val = maxVal;
            return val;
        }
        case DistKind::Triangular: {
            int lo = a, hi = b, peak = c;
            if (peak < lo) peak = lo;
            if (peak > hi) peak = hi;
            vector<int> vals;
            vector<double> w;
            for (int v = lo; v <= hi; v++) {
                double weight;
                if (v <= peak)
                    weight = (peak == lo) ? 1.0
                                          : (double)(v - lo + 1) / (peak - lo + 1);
                else
                    weight = (hi == peak) ? 1.0
                                          : (double)(hi - v + 1) / (hi - peak + 1);
                vals.push_back(v);
                w.push_back(weight);
            }
            discrete_distribution<int> d(w.begin(), w.end());
            return vals[d(rng)];
        }
    }
    return 0;
}