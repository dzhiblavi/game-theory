#include <iostream>
#include <vector>

using namespace std;

enum Clr {
  WHITE,
  GRAY,
  BLACK
};

enum Type {
  UNDEF,
  WIN,
  LOSS
};

int n, m;
vector<vector<int>> graph;
vector<Clr> color;
vector<bool> on_cycle;
vector<Type> type;
vector<int> parent;
bool changed = true;

void solve(int v) {
  if (type[v] != UNDEF) {
    return;
  }

  color[v] = GRAY;
  int n_children = graph[v].size();
  int child_undef = 0;
  int child_loss = 0;
  int child_win = 0;
  int child_cycle = 0;

  if (graph[v].empty()) {
    type[v] = LOSS;
  } else {
    for (int u : graph[v]) {
      if (color[u] == GRAY) {
        on_cycle[u] = true;
        int z = v;
        while (z != u) {
          on_cycle[z] = true;
          z = parent[z];
        }
      } else if (color[u] == BLACK) {
        // skip
      } else {
        parent[u] = v;
        solve(u);
      }

      child_win += type[u] == WIN;
      child_loss += type[u] == LOSS;
      child_undef += type[u] == UNDEF;
      child_cycle += on_cycle[u];
    }
  }

  if (child_loss > 0) {
    type[v] = WIN;
    changed = true;
  } else if (child_win == n_children) {
    type[v] = LOSS;
    changed = true;
  }

  color[v] = BLACK;
}

int main() {
  cin >> n >> m;
  graph.resize(n);
  on_cycle.resize(n);
  color.resize(n, WHITE);
  type.resize(n);
  parent.resize(n, -1);
  for (int i = 0, x, y; i < m; ++i) {
    cin >> x >> y;
    graph[x - 1].push_back(y - 1);
  }
  changed = true;
  while (changed) {
    changed = false;
    fill(color.begin(), color.end(), WHITE);
    for (int i = 0; i < n; ++i) {
      if (type[i] == UNDEF && color[i] == WHITE) {
        solve(i);
      }
    }
  }
  for (int i = 0; i < n; ++i) {
    switch (type[i]) {
      case WIN:
        cout << "Win\n";
        break;
      case LOSS:
        cout << "Loss\n";
        break;
      case UNDEF:
        cout << "Draw\n";
        break;
    } 
  }
  return 0;
}
