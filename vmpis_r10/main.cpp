#include <algorithm>
#include <cmath>
#include <cstdint>
#include <exception>
#include <iomanip>
#include <iostream>
#include <functional>
#include <map>
#include <numeric>
#include <random>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>

// Взвешенное неориентированное ребро.
struct Edge {
    int u;
    int v;
    int w;
};

// Система непересекающихся множеств (DSU, Union-Find) для алгоритма Краскала.
struct DSU {
    std::vector<int> parent;
    std::vector<int> rank;

    explicit DSU(int n) : parent(n), rank(n, 0) {
        std::iota(parent.begin(), parent.end(), 0);
    }

    int find(int x) {
        if (parent[x] != x) {
            parent[x] = find(parent[x]);
        }
        return parent[x];
    }

    bool unite(int a, int b) {
        a = find(a);
        b = find(b);
        if (a == b) {
            return false;
        }
        if (rank[a] < rank[b]) {
            std::swap(a, b);
        }
        parent[b] = a;
        if (rank[a] == rank[b]) {
            rank[a]++;
        }
        return true;
    }
};

// Канонический ключ неориентированного ребра: (min(u,v), max(u,v)).
static std::pair<int, int> edgeKey(int u, int v) {
    if (u > v) {
        std::swap(u, v);
    }
    return {u, v};
}

// Аргументы командной строки.
struct Args {
    int n = -1;
    int samples = 1000;
    double pWeighted = 0.35;
    double pUnweighted = 0.30;
    bool hasSeed = false;
    uint64_t seed = 0;
};

static void printHelp() {
    std::cout
        << "Usage:\n"
        << "  ./terver --n <int> [--samples <int>] [--p-weighted <double>] [--p-unweighted <double>] [--seed <int>]\n\n"
        << "Parameters:\n"
        << "  --n            Number of vertices (required)\n"
        << "  --samples      Monte Carlo samples count (default 1000)\n"
        << "  --p-weighted   Probability of extra edge in connected weighted graph for A,B,D (default 0.35)\n"
        << "  --p-unweighted Probability of edge in G(n,p) for E,G,H (default 0.30)\n"
        << "  --seed         RNG seed (optional)\n";
}

static Args parseArgs(int argc, char** argv) {
    Args args;
    for (int i = 1; i < argc; ++i) {
        std::string key = argv[i];
        auto requireValue = [&](const std::string& name) {
            if (i + 1 >= argc) {
                throw std::invalid_argument("Missing value for " + name);
            }
        };

        if (key == "--help" || key == "-h") {
            printHelp();
            std::exit(0);
        } else if (key == "--n") {
            requireValue(key);
            args.n = std::stoi(argv[++i]);
        } else if (key == "--samples") {
            requireValue(key);
            args.samples = std::stoi(argv[++i]);
        } else if (key == "--p-weighted") {
            requireValue(key);
            args.pWeighted = std::stod(argv[++i]);
        } else if (key == "--p-unweighted") {
            requireValue(key);
            args.pUnweighted = std::stod(argv[++i]);
        } else if (key == "--seed") {
            requireValue(key);
            args.hasSeed = true;
            args.seed = static_cast<uint64_t>(std::stoull(argv[++i]));
        } else {
            throw std::invalid_argument("Unknown argument: " + key);
        }
    }

    if (args.n < 1) {
        throw std::invalid_argument("--n must be >= 1");
    }
    if (args.samples < 1) {
        throw std::invalid_argument("--samples must be >= 1");
    }
    if (args.pWeighted < 0.0 || args.pWeighted > 1.0) {
        throw std::invalid_argument("--p-weighted must be in [0,1]");
    }
    if (args.pUnweighted < 0.0 || args.pUnweighted > 1.0) {
        throw std::invalid_argument("--p-unweighted must be in [0,1]");
    }
    return args;
}

// Генерация связного взвешенного графа:
// 1) случайное остовное дерево (гарантирует связность),
// 2) добавление дополнительных рёбер независимо с вероятностью pExtra.
static std::vector<Edge> generateConnectedWeightedGraph(int n, double pExtra, std::mt19937_64& rng) {
    if (n <= 1) {
        return {};
    }

    std::uniform_int_distribution<int> wDist(1, 10);
    std::uniform_real_distribution<double> pDist(0.0, 1.0);

    std::map<std::pair<int, int>, int> edges;

    for (int v = 1; v < n; ++v) {
        std::uniform_int_distribution<int> parentDist(0, v - 1);
        int u = parentDist(rng);
        edges[edgeKey(u, v)] = wDist(rng);
    }

    for (int u = 0; u < n; ++u) {
        for (int v = u + 1; v < n; ++v) {
            auto key = edgeKey(u, v);
            if (edges.find(key) != edges.end()) {
                continue;
            }
            if (pDist(rng) < pExtra) {
                edges[key] = wDist(rng);
            }
        }
    }

    std::vector<Edge> result;
    result.reserve(edges.size());
    for (const auto& [k, w] : edges) {
        result.push_back({k.first, k.second, w});
    }
    return result;
}

// Генерация невзвешенного графа Эрдёша-Реньи G(n, p).
static std::set<std::pair<int, int>> generateUnweightedGraph(int n, double p, std::mt19937_64& rng) {
    std::set<std::pair<int, int>> edges;
    std::uniform_real_distribution<double> pDist(0.0, 1.0);
    for (int u = 0; u < n; ++u) {
        for (int v = u + 1; v < n; ++v) {
            if (pDist(rng) < p) {
                edges.insert({u, v});
            }
        }
    }
    return edges;
}

// Случайная величина A: вес минимального остовного дерева (Краскал).
static int mstWeightKruskal(int n, std::vector<Edge> edges) {
    if (n <= 1) {
        return 0;
    }
    std::sort(edges.begin(), edges.end(), [](const Edge& a, const Edge& b) {
        return a.w < b.w;
    });
    DSU dsu(n);
    int total = 0;
    int used = 0;
    for (const auto& e : edges) {
        if (dsu.unite(e.u, e.v)) {
            total += e.w;
            used++;
            if (used == n - 1) {
                break;
            }
        }
    }
    return total;
}

static std::vector<std::vector<std::pair<int, int>>> buildWeightedAdj(int n, const std::vector<Edge>& edges) {
    std::vector<std::vector<std::pair<int, int>>> adj(n);
    for (const auto& e : edges) {
        adj[e.u].push_back({e.v, e.w});
        adj[e.v].push_back({e.u, e.w});
    }
    return adj;
}

// Нормализация представления цикла, чтобы один и тот же цикл,
// найденный из разных стартов/направлений, хранился ровно один раз.
static std::vector<int> canonicalCycle(const std::vector<int>& cycleWithRepeat) {
    if (cycleWithRepeat.size() <= 1) {
        return {};
    }
    std::vector<int> base(cycleWithRepeat.begin(), cycleWithRepeat.end() - 1);
    if (base.empty()) {
        return {};
    }

    int minPos = 0;
    for (int i = 1; i < static_cast<int>(base.size()); ++i) {
        if (base[i] < base[minPos]) {
            minPos = i;
        }
    }

    std::vector<int> r1;
    r1.reserve(base.size());
    for (int i = 0; i < static_cast<int>(base.size()); ++i) {
        r1.push_back(base[(minPos + i) % static_cast<int>(base.size())]);
    }

    std::vector<int> r2 = r1;
    std::reverse(r2.begin(), r2.end());
    return (r1 <= r2) ? r1 : r2;
}

struct CycleInfo {
    std::vector<int> nodes;
    int weightSum;
};

// Перебор всех простых циклов в неориентированном графе (на основе DFS).
static std::vector<CycleInfo> allSimpleCyclesUndirected(int n, const std::vector<Edge>& edges) {
    auto adj = buildWeightedAdj(n, edges);
    std::vector<CycleInfo> cycles;
    std::set<std::vector<int>> seen;

    std::vector<bool> visited(n, false);
    std::vector<int> path;

    std::function<void(int, int, int)> dfs = [&](int start, int cur, int wsum) {
        for (const auto& [nxt, w] : adj[cur]) {
            if (nxt == start && path.size() >= 3) {
                std::vector<int> cycle = path;
                cycle.push_back(start);
                auto canon = canonicalCycle(cycle);
                if (!canon.empty() && seen.insert(canon).second) {
                    cycles.push_back({canon, wsum + w});
                }
                continue;
            }
            if (nxt < start || visited[nxt]) {
                continue;
            }
            visited[nxt] = true;
            path.push_back(nxt);
            dfs(start, nxt, wsum + w);
            path.pop_back();
            visited[nxt] = false;
        }
    };

    for (int start = 0; start < n; ++start) {
        std::fill(visited.begin(), visited.end(), false);
        path.clear();
        visited[start] = true;
        path.push_back(start);
        dfs(start, start, 0);
    }

    return cycles;
}

// Случайные величины:
// B = длина максимального цикла (по числу рёбер),
// D = число рёбер цикла с максимальной суммой весов.
static std::pair<int, int> longestCycleAndHeaviestCycleEdges(int n, const std::vector<Edge>& edges) {
    auto cycles = allSimpleCyclesUndirected(n, edges);
    if (cycles.empty()) {
        return {0, 0};
    }
    int longest = 0;
    int maxWeight = std::numeric_limits<int>::min();
    int edgesInHeaviest = 0;
    for (const auto& c : cycles) {
        int len = static_cast<int>(c.nodes.size());
        longest = std::max(longest, len);
        if (c.weightSum > maxWeight) {
            maxWeight = c.weightSum;
            edgesInHeaviest = len;
        } else if (c.weightSum == maxWeight) {
            edgesInHeaviest = std::max(edgesInHeaviest, len);
        }
    }
    return {longest, edgesInHeaviest};
}

// Поиск компонент связности итеративным DFS.
static std::vector<std::set<int>> connectedComponents(int n, const std::set<std::pair<int, int>>& edges) {
    std::vector<std::vector<int>> adj(n);
    for (const auto& [u, v] : edges) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    std::vector<bool> vis(n, false);
    std::vector<std::set<int>> comps;

    for (int s = 0; s < n; ++s) {
        if (vis[s]) {
            continue;
        }
        std::vector<int> st = {s};
        vis[s] = true;
        std::set<int> comp;
        while (!st.empty()) {
            int cur = st.back();
            st.pop_back();
            comp.insert(cur);
            for (int nxt : adj[cur]) {
                if (!vis[nxt]) {
                    vis[nxt] = true;
                    st.push_back(nxt);
                }
            }
        }
        comps.push_back(std::move(comp));
    }
    return comps;
}

// Случайная величина E: число изолированных вершин.
static int countIsolatedVertices(int n, const std::set<std::pair<int, int>>& edges) {
    std::vector<int> deg(n, 0);
    for (const auto& [u, v] : edges) {
        deg[u]++;
        deg[v]++;
    }
    int cnt = 0;
    for (int d : deg) {
        if (d == 0) {
            cnt++;
        }
    }
    return cnt;
}

// Случайная величина G: число компонент связности.
static int countComponents(int n, const std::set<std::pair<int, int>>& edges) {
    return static_cast<int>(connectedComponents(n, edges).size());
}

// Случайная величина H: число компонент, являющихся кликами.
static int countCliqueComponents(int n, const std::set<std::pair<int, int>>& edges) {
    auto comps = connectedComponents(n, edges);
    int cnt = 0;
    for (const auto& comp : comps) {
        if (comp.size() <= 1) {
            cnt++;
            continue;
        }
        std::vector<int> v(comp.begin(), comp.end());
        int actual = 0;
        for (int i = 0; i < static_cast<int>(v.size()); ++i) {
            for (int j = i + 1; j < static_cast<int>(v.size()); ++j) {
                if (edges.find(edgeKey(v[i], v[j])) != edges.end()) {
                    actual++;
                }
            }
        }
        int need = static_cast<int>(v.size() * (v.size() - 1) / 2);
        if (actual == need) {
            cnt++;
        }
    }
    return cnt;
}

// Случайная величина F (детерминирована при фиксированном n):
// число неизоморфных деревьев на n вершинах.
static int countUnlabeledTrees(int n) {
    static const std::unordered_map<int, int> table = {
        {1, 1},   {2, 1},   {3, 1},    {4, 2},     {5, 3},      {6, 6},      {7, 11},
        {8, 23},  {9, 47},  {10, 106}, {11, 235},  {12, 551},   {13, 1301},  {14, 3159},
        {15, 7741}, {16, 19320}, {17, 48629}, {18, 123867}, {19, 317955}, {20, 823065}
    };
    auto it = table.find(n);
    if (it == table.end()) {
        throw std::invalid_argument("For F, supported n range is [1..20]");
    }
    return it->second;
}

static double mean(const std::vector<double>& x) {
    if (x.empty()) {
        return 0.0;
    }
    double s = 0.0;
    for (double v : x) {
        s += v;
    }
    return s / static_cast<double>(x.size());
}

static double variance(const std::vector<double>& x) {
    if (x.empty()) {
        return 0.0;
    }
    double m = mean(x);
    double s = 0.0;
    for (double v : x) {
        double d = v - m;
        s += d * d;
    }
    return s / static_cast<double>(x.size());
}

int main(int argc, char** argv) {
    try {
        Args args = parseArgs(argc, argv);
        std::mt19937_64 rng;
        if (args.hasSeed) {
            rng.seed(args.seed);
        } else {
            std::random_device rd;
            rng.seed((static_cast<uint64_t>(rd()) << 32) ^ rd());
        }

        std::unordered_map<std::string, std::vector<double>> data = {
            {"A", {}}, {"B", {}}, {"D", {}}, {"E", {}}, {"F", {}}, {"G", {}}, {"H", {}}
        };
        for (auto& [k, v] : data) {
            v.reserve(args.samples);
        }

        const double fConst = static_cast<double>(countUnlabeledTrees(args.n));

        // Цикл Монте-Карло: на каждом шаге генерируем случайные графы
        // и получаем по одному наблюдению для A, B, D, E, F, G, H.
        for (int i = 0; i < args.samples; ++i) {
            auto weighted = generateConnectedWeightedGraph(args.n, args.pWeighted, rng);
            int A = mstWeightKruskal(args.n, weighted);
            auto [B, D] = longestCycleAndHeaviestCycleEdges(args.n, weighted);

            auto unweighted = generateUnweightedGraph(args.n, args.pUnweighted, rng);
            int E = countIsolatedVertices(args.n, unweighted);
            int G = countComponents(args.n, unweighted);
            int H = countCliqueComponents(args.n, unweighted);

            data["A"].push_back(static_cast<double>(A));
            data["B"].push_back(static_cast<double>(B));
            data["D"].push_back(static_cast<double>(D));
            data["E"].push_back(static_cast<double>(E));
            data["F"].push_back(fConst);
            data["G"].push_back(static_cast<double>(G));
            data["H"].push_back(static_cast<double>(H));
        }

        std::cout << std::fixed << std::setprecision(6);
        std::cout << "Параметры запуска:\n";
        std::cout << "  n = " << args.n << "\n";
        std::cout << "  samples = " << args.samples << "\n";
        std::cout << "  p_weighted = " << args.pWeighted << "\n";
        std::cout << "  p_unweighted = " << args.pUnweighted << "\n";
        if (args.hasSeed) {
            std::cout << "  seed = " << args.seed << "\n";
        }
        std::cout << "\n";

        std::cout << "Оценки (матожидание, дисперсия):\n";
        for (const char* name : {"A", "B", "D", "E", "F", "G", "H"}) {
            double m = mean(data[name]);
            double d = variance(data[name]);
            std::cout << "  " << name << ": M ~= " << m << ", D ~= " << d << "\n";
        }

        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n\n";
        printHelp();
        return 1;
    }
}
