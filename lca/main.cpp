#include <iostream>
#include <vector>
#include <algorithm>


using namespace std;

using ulli = unsigned long long int;
using lli = long long int;
using ld = long double;


vector<int> logs;
vector<int> tin;
vector<int> tout;


void dfs(int cur, int &t, vector<int> &p, vector<vector<int>> &g) {
    tin[cur] = t++;

    for (int &e : g[cur]) {
        if (e != p[cur]) {
            p[e] = cur;
            dfs(e, t, p, g);
        }
    }

    tout[cur] = t;
}


inline bool is_upper(int a, int b) {
    return tin[a] <= tin[b] && tout[b] <= tout[a];
}


int lca(int a, int b, vector<int> up[]) {
    if (is_upper(a, b))
        return a;

    if (is_upper(b, a))
        return b;

    for (int j = logs.back(); j >= 0; --j) {
        if (!is_upper(up[a][j], b))
            a = up[a][j];
    }

    return up[a][0];
}


int new_lca(int a, int b, int n_r, vector<int> up[]) {
    int x = lca(a, n_r, up);
    int y = lca(b, n_r, up);
    int z = lca(a, b, up);

    if (x == y) {
        return z;
    }

    if (x == z) {
        return y;
    }

    return x;
}


int main() {
    // freopen("input.txt", "r", stdin);
    std::ios::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);


    int n, m;
    int u, v;

    while (cin >> n) {
        if (n == 0) continue;

        logs.resize(n + 1);
        logs[0] = 0;
        logs[1] = 0;

        for (int i = 2; i <= n; ++i) {
            logs[i] = logs[i / 2] + 1;
        }


        vector<vector<int>> g(n);

        for (int i = 0; i < n - 1; ++i) {
            cin >> u >> v;
            --u;
            --v;

            g[u].push_back(v);
            g[v].push_back(u);
        }


        tin.resize(n);
        tin.assign(n, 0);
        tout.resize(n);
        tout.assign(n, 0);
        vector<int> p(n);
        int timer = 0;

        dfs(0, timer, p, g);


        vector<int> up[n];
        // up[i][j] means ancestor, which is 2^j positions higher than vertex i

        for (int i = 0; i < n; ++i) {
            up[i].resize(logs[n] + 1);

            up[i][0] = p[i];
        }

        for (int j = 1; j <= logs[n]; ++j) {
            for (int i = 0; i < n; ++i) {
                up[i][j] = up[up[i][j - 1]][j - 1];
            }
        }


        cin >> m;

        char req;
        int new_root = 0;

        for (int i = 0; i < m; ++i) {
            cin >> req;

            if (req == '?') {
                cin >> u >> v;
                --u;
                --v;

                cout << new_lca(u, v, new_root, up) + 1 << '\n';
            } else {
                cin >> u;

                new_root = u - 1;
            }
        }
    }


    return 0;
}