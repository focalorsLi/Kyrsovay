#include "MaxFlowSolver.h"
#include <algorithm>
#include <climits>
#include <stdexcept>

using namespace std;

Edge::Edge(int f, int t, long long c)
    : from(f), to(t), cap(c), flow(0) {
}

MaxFlowSolver::MaxFlowSolver(int vertices) : n(vertices) {
    if (vertices <= 0) {
        throw invalid_argument("Kolichestvo vershin dolzhno byt' polozhitelnym");
    }
    capacity.assign(n, vector<long long>(n, 0));
    originalCap.assign(n, vector<long long>(n, 0));
    adj.assign(n, vector<int>());
}

long long MaxFlowSolver::bfs(int s, int t, vector<int>& parent) {
    fill(parent.begin(), parent.end(), -1);
    parent[s] = -2;

    queue<pair<int, long long>> q;
    q.push({ s, LLONG_MAX });

    while (!q.empty()) {
        int cur = q.front().first;
        long long curFlow = q.front().second;
        q.pop();

        for (int next : adj[cur]) {
            if (parent[next] == -1 && capacity[cur][next] > 0) {
                parent[next] = cur;
                long long newFlow = min(curFlow, capacity[cur][next]);

                if (next == t) {
                    return newFlow;
                }

                q.push({ next, newFlow });
            }
        }
    }

    return 0;
}

bool MaxFlowSolver::addEdge(int from, int to, long long cap) {
    if (from < 0 || from >= n) {
        cerr << "Oshibka: nachalnaya vershina " << from
            << " vne dopustimogo diapazona [0, " << n - 1 << "]" << endl;
        return false;
    }

    if (to < 0 || to >= n) {
        cerr << "Oshibka: konechnaya vershina " << to
            << " vne dopustimogo diapazona [0, " << n - 1 << "]" << endl;
        return false;
    }

    if (from == to) {
        cerr << "Oshibka: petli ne dopuskayutsya (vershina " << from << " -> " << to << ")" << endl;
        return false;
    }

    if (cap < 0) {
        cerr << "Oshibka: propusknaya sposobnost' ne mozhet byt' otritsatelnoy (" << cap << ")" << endl;
        return false;
    }

    if (originalCap[from][to] > 0) {
        cerr << "Preduprezhdenie: rebro " << from << " -> " << to
            << " uzhe suschestvuet, znachenie obnovleno" << endl;

        for (Edge& e : edges) {
            if (e.from == from && e.to == to) {
                e.cap = cap;
                e.flow = 0;
                break;
            }
        }
    }
    else {
        edges.push_back(Edge(from, to, cap));
        adj[from].push_back(to);
        adj[to].push_back(from);
    }

    capacity[from][to] = cap;
    originalCap[from][to] = cap;

    return true;
}

long long MaxFlowSolver::getMaxFlow(int s, int t) {
    if (s < 0 || s >= n || t < 0 || t >= n) {
        throw invalid_argument("Istok ili stok vne diapazona vershin");
    }
    if (s == t) {
        throw invalid_argument("Istok i stok ne mogut sovpadat");
    }

    long long maxFlow = 0;
    vector<int> parent(n);
    long long augmentingFlow;

    int iteration = 0;
    cout << "=== Zapusk algoritma Edmondsa-Karpa ===" << endl;
    cout << "Istok: " << s << ", Stok: " << t << endl << endl;

    while ((augmentingFlow = bfs(s, t, parent)) > 0) {
        maxFlow += augmentingFlow;
        iteration++;

        vector<int> path;
        int cur = t;
        while (cur != s) {
            path.push_back(cur);
            cur = parent[cur];
        }
        path.push_back(s);
        reverse(path.begin(), path.end());

        cout << "Iteratsiya " << iteration << ": nayden put' ";
        for (size_t i = 0; i < path.size(); ++i) {
            cout << path[i];
            if (i < path.size() - 1) cout << " -> ";
        }
        cout << ", potok = " << augmentingFlow << endl;

        cur = t;
        while (cur != s) {
            int prev = parent[cur];
            capacity[prev][cur] -= augmentingFlow;
            capacity[cur][prev] += augmentingFlow;
            cur = prev;
        }
    }

    cout << endl << "Algoritm zavershyon za " << iteration << " iteratsiy." << endl;

    for (Edge& e : edges) {
        e.flow = originalCap[e.from][e.to] - capacity[e.from][e.to];
    }

    return maxFlow;
}

void MaxFlowSolver::printFlow(ostream& out) const {
    out << endl << "=== Itogovyy potok po ryobram ===" << endl;
    out << "Rebro\tPotok\tProp. spos.\tNasyschenie" << endl;
    out << "-------------------------------------------" << endl;

    for (const Edge& e : edges) {
        double saturation = (e.cap > 0) ? (100.0 * e.flow / e.cap) : 0.0;
        out << e.from << " -> " << e.to << "\t"
            << e.flow << "\t" << e.cap << "\t\t"
            << saturation << "%" << endl;
    }
}

void MaxFlowSolver::printAdjacencyMatrix(ostream& out) const {
    out << endl << "=== Matritsa smezhnosti (iskhodnye propusknye sposobnosti) ===" << endl;

    out << "    ";
    for (int j = 0; j < n; ++j) {
        out << j << "\t";
    }
    out << endl;

    for (int i = 0; i < n; ++i) {
        out << i << " | ";
        for (int j = 0; j < n; ++j) {
            out << originalCap[i][j] << "\t";
        }
        out << endl;
    }
}

int MaxFlowSolver::getVertexCount() const {
    return n;
}

int MaxFlowSolver::getEdgeCount() const {
    return edges.size();
}

bool loadFromFile(const string& filename, MaxFlowSolver& solver, int& s, int& t) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Oshibka: ne udalos' otkryt' fail \"" << filename << "\"" << endl;
        return false;
    }

    int vertices, edgesCount;
    if (!(file >> vertices >> edgesCount)) {
        cerr << "Oshibka: nevernyy format faila (ozhidaetsya kolichestvo vershin i ryober)" << endl;
        file.close();
        return false;
    }

    if (!(file >> s >> t)) {
        cerr << "Oshibka: nevernyy format faila (ozhidayutsya istok i stok)" << endl;
        file.close();
        return false;
    }

    solver = MaxFlowSolver(vertices);

    for (int i = 0; i < edgesCount; ++i) {
        int from, to;
        long long cap;
        if (!(file >> from >> to >> cap)) {
            cerr << "Oshibka: nevernyy format faila v stroke rebra " << i + 1 << endl;
            file.close();
            return false;
        }
        solver.addEdge(from, to, cap);
    }

    file.close();
    cout << "Graf uspeshno zagruzhen iz faila \"" << filename << "\": "
        << vertices << " vershin, " << edgesCount << " ryober" << endl;
    return true;
}