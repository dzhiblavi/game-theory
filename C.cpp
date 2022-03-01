#include <iostream>
#include <vector>

using namespace std;

int n, m;
vector<vector<int>> graph;
vector<int> grandy;
int max_g = 1000000;
vector<bool> used(1 + max_g);

int mex(vector<int> const& v) {
  int size = v.size();

  for (int i = 0; i < size; ++i) {
    if (v[i] <= max_g) {
      used[v[i]] = true;
    }
  }

  int result;
  for (int i = 0; ; ++i) {
    if (!used[i]) {
      result = i;
      break;
    }
  }

  for (int i = 0; i < size; ++i) {
    if (v[i] <= max_g) {
      used[v[i]] = false;
    }
  }

  return result;
}

void solve(int v) {
  if (grandy[v] != -1) {
    return;
  }

  if (graph[v].empty()) {
    grandy[v] = 0;
  } else {
    vector<int> ch_grandy;
    ch_grandy.reserve(graph[v].size());
    for (int u : graph[v]) {
      solve(u);
      ch_grandy.push_back(grandy[u]);
    }

    grandy[v] = mex(ch_grandy);
  }
}

int main() {
  cin >> n >> m;
  graph.resize(n);
  grandy.resize(n, -1);
  for (int i = 0, x, y; i < m; ++i) {
    cin >> x >> y;
    graph[x - 1].push_back(y - 1);
  }

  for (int i = 0; i < n; ++i) {
    solve(i);
  }

  for (int i = 0; i < n; ++i) {
    cout << grandy[i] << '\n';
  }
  return 0;
}
