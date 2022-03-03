#include <iostream>
#include <vector>
#include <unordered_set>
#include <unordered_map>

using namespace std;

enum Clr {
  BLACK,
  GRAY,
  WHITE,
};

int n, m;
int s = 0;
vector<unordered_map<int, int>> graph;
vector<int> parent;
vector<Clr> color;
vector<bool> is_deleted;

#ifdef LOCAL
void print_graph() {
  cout << "Vertices: " << n << endl;
  cout << "Root:     " << s << endl;
  for (int v = 0; v < n; ++v) {
    if (is_deleted[v]) {
      continue;
    }
    cout << v << " -> [ ";
    for (auto u : graph[v]) {
      if (!is_deleted[u.first]) {
        cout << "{ " << u.first << ':' << u.second << " } ";
      }
    }
    cout << "]" << endl;
  }
}
#else
#define print_graph()
#endif

template <typename T, typename V>
bool contains(T const& c, V const& value) {
  return c.find(value) != c.end();
}

void add_edge_one_direction(int v, int u, int c = 1) {
  auto& g = graph[v];
  if (g.find(u) == g.end()) {
    g[u] = 0;
  }
  g[u] += c;
}

void add_edge(int v, int u, int c = 1) {
  add_edge_one_direction(v, u, c);
  if (v != u) {
    add_edge_one_direction(u, v, c);
  }
}

void remove_edge_fully(int v, int u) {
  graph[v].erase(u);
  graph[u].erase(v);
}

int new_vertex() {
  graph.emplace_back();
  is_deleted.push_back(false);
  color.push_back(WHITE);
  parent.push_back(-1);
  return n++;
}

void delete_vertex(int v, int replaced) {
  is_deleted[v] = true;
  if (v == s) {
    s = replaced;
  }
  graph[v].clear();
}

uint64_t tree_gr(int v, int p = -1) {
  uint64_t value = 0;
  for (auto&& [u, c] : graph[v]) {
    if (is_deleted[u] || u == p) {
      continue;
    }
    value ^= tree_gr(u, v) + 1;
  }
  return value;
}

// No multiedges or loops on this stage!
int cyc_v;
void fc_dfs(int v, int p = -1) {
  parent[v] = p;
  color[v] = GRAY;
  for (auto&& [u, count] : graph[v]) {
    if (cyc_v != -1) {
      break;
    }
    if (is_deleted[u] || u == p) {
      continue;
    }
    if (color[u] == GRAY) {
      cyc_v = u;
      parent[u] = v;
      return;
    } else if (color[u] == WHITE) {
      fc_dfs(u, v);
    }
  }
  color[v] = BLACK;
}

int loop_v;
bool find_loop() {
  for (int i = 0; i < n; ++i) {
    if (!is_deleted[i] && contains(graph[i], i)) {
      loop_v = i;
#ifdef LOCAL
      cout << "Loop found: " << i << endl;
#endif
      return true;
    }
  }
  return false;
}

int me_v, me_u;
bool find_multiedge() {
  for (int v = 0; v < n; ++v) {
    if (is_deleted[v]) {
      continue;
    }
    for (auto&& [u, c] : graph[v]) {
      if (is_deleted[u]) {
        continue;
      }
      if (c > 1) {
        me_v = v;
        me_u = u;
#ifdef LOCAL
        cout << "Multiedge found: " << me_v << ' ' << me_u << endl;
#endif
        return true;
      }
    }
  }
  return false;
}

void remove_loop() {
  int c = graph[loop_v].find(loop_v)->second;
  remove_edge_fully(loop_v, loop_v);
  for (int i = 0; i < c; ++i) {
    add_edge(loop_v, new_vertex());
  }
}

void remove_multiedge() {
  int num = graph[me_v].find(me_u)->second;
  for (auto&& [z, c] : graph[me_u]) {
    if (z != me_v) {
      add_edge(me_v, z, c);
    }
  }
  remove_edge_fully(me_v, me_u);
  delete_vertex(me_u, me_v);
  for (int i = 0; i < num; ++i) {
    add_edge(me_v, new_vertex());
  }
}

void remove_cycle() {
  vector<int> on_cycle = {cyc_v};
  {  // collect cycle
    int v = parent[cyc_v];
    while (v != -1 && v != cyc_v) {
      on_cycle.push_back(v);
      v = parent[v];
    }
  }

#ifdef LOCAL
  {
    cout << "Found cycle. Base: " << cyc_v << " Size: " << on_cycle.size() << endl;
    cout << "[ ";
    for (auto v : on_cycle) {
      cout << v << ' ';
    }
    cout << "]" << endl;
  }
#endif

  {  // get all edges to cyc_v
    for (int v : on_cycle) {
      if (v == cyc_v) {
        continue;
      }
      for (auto&& [u, c] : graph[v]) {
        add_edge(cyc_v, u, c);
      }
    }
  }

  int num_leaves = 0;
  {  // remove all vertices except cyc_v
    for (int v : on_cycle) {
      if (v == cyc_v) {
        continue;
      }
      num_leaves += graph[v][cyc_v];
      delete_vertex(v, cyc_v);
    }
  }
}

bool find_and_remove_cycle() {
  while (find_loop()) {
    remove_loop();
    print_graph();
  }

  while (find_multiedge()) {
    remove_multiedge();
    print_graph();
  }

  cyc_v = -1;
  fill(parent.begin(), parent.end(), -1);
  fill(color.begin(), color.end(), WHITE);
  for (int i = 0; i < n; ++i) {
    if (!is_deleted[i] && color[i] == WHITE) {
      fc_dfs(i);
    }
    if (cyc_v != -1) {
      remove_cycle();
      print_graph();
      return true;
    }
  }
  return false;
}

void remove_cycles() {
  while (find_and_remove_cycle());
}

int main() {
  cin >> n >> m;
  graph.resize(n);
  is_deleted.resize(n);
  color.resize(n);
  parent.resize(n);
  for (int i = 0, x, y; i < m; ++i) {
    cin >> x >> y;
    --x;
    --y;
    add_edge(x, y);
  }

  print_graph();
  remove_cycles();

  if (tree_gr(s) != 0) {
    cout << "First\n";
  } else {
    cout << "Second\n";
  }
  return 0;
}