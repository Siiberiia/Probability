#include "model.h"
#include <fstream>

using namespace std;

EpidemicModel::EpidemicModel() : rng(random_device{}()) {}

bool EpidemicModel::load(const string& filename) {
    ifstream fin(filename);
    if (!fin) return false;

    int M;
    fin >> N >> M;
    if (N <= 0) return false;

    adj.assign(N, {});
    for (int k = 0; k < M; k++) {
        int a, b;
        fin >> a >> b;
        if (a < 0 || a >= N || b < 0 || b >= N) continue;
        adj[a].push_back(b);
        adj[b].push_back(a);
    }

    startIndex = 0;
    fin >> startIndex;
    if (startIndex < 0 || startIndex >= N) startIndex = 0;

    reset();
    return true;
}

void EpidemicModel::reset() {
    state.assign(N, HEALTHY);
    if (N > 0) state[startIndex] = SICK;
}

void EpidemicModel::step() {
    vector<int> next = state;
    uniform_real_distribution<double> dist(0.0, 1.0);

    for (int v = 0; v < N; v++) {
        if (state[v] == SICK) {
            // Заражение соседей
            for (int u : adj[v]) {
                if (state[u] == HEALTHY && dist(rng) < p1)
                    next[u] = SICK;
            }
            // Выздоровление
            if (dist(rng) < p2)
                next[v] = RECOVERED;
        }
    }
    state = next;
}

int EpidemicModel::count(State s) const {
    int c = 0;
    for (int v : state) if (v == s) c++;
    return c;
}

bool EpidemicModel::hasSick() const {
    for (int v : state) if (v == SICK) return true;
    return false;
}

vector<int> EpidemicModel::neverSick() const {
    vector<int> res;
    for (int v = 0; v < N; v++)
        if (state[v] == HEALTHY) res.push_back(v);
    return res;
}

vector<int> EpidemicModel::recovered() const {
    vector<int> res;
    for (int v = 0; v < N; v++)
        if (state[v] == RECOVERED) res.push_back(v);
    return res;
}

vector<int> EpidemicModel::recoveredWithNotRecoveredNeighbours() const {
    vector<int> res;
    for (int v = 0; v < N; v++) {
        if (state[v] != RECOVERED) continue;
        bool allRec = true;
        for (int u : adj[v])
            if (state[u] != RECOVERED) { allRec = false; break; }
        if (!allRec) res.push_back(v);
    }
    return res;
}

vector<int> EpidemicModel::healthyWithAllSickNeighbours() const {
    vector<int> res;
    for (int v = 0; v < N; v++) {
        if (state[v] != HEALTHY) continue;
        if (adj[v].empty()) continue;
        bool allAffected = true;
        for (int u : adj[v])
            if (state[u] == HEALTHY) { allAffected = false; break; }
        if (allAffected) res.push_back(v);
    }
    return res;
}