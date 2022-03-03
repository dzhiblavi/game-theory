#include <cassert>
#include <iostream>
#include <stack>
#include <tuple>

using namespace std;

#define MAX_SIDE 51

enum State {
  UNDEF,
  WIN,
  LOSS,
  INVALID,
  DRAW,
};

const int steps[8][2] = {
    {0, 1}, {1, 0}, {1, 1}, {0, -1}, {-1, 0}, {-1, -1}, {1, -1}, {-1, 1},
};

int n;
int m;
int swx, swy, sbx, sby;
bool changed = true;

// [x1][y1][x2][y2][white(0)|black(1)]
State states[MAX_SIDE][MAX_SIDE][MAX_SIDE][MAX_SIDE][2] = {};
bool visited[MAX_SIDE][MAX_SIDE][MAX_SIDE][MAX_SIDE][2] = {};
stack<std::tuple<int, int, int, int, int>> st;

void mark_same() {
  for (int x = 0; x < n; ++x) {
    for (int y = 0; y < n; ++y) {
      states[x][y][x][y][0] = states[x][y][x][y][1] = INVALID;
    }
  }
}

void mark_invalid(int x, int y) {
  for (int px = 0; px < n; ++px) {
    for (int py = 0; py < n; ++py) {
      states[x][y][px][py][0] = states[x][y][px][py][1] = INVALID;
      states[px][py][x][y][0] = states[px][py][x][y][1] = INVALID;
    }
  }
}

inline int distance(int x1, int y1, int x2, int y2) {
  return abs(x1 - x2) + abs(y1 - y2);
}

inline bool valid(int x) { return 0 <= x && x < n; }

inline bool valid(int x1, int y1, int x2, int y2) {
  return valid(x1) && valid(y1) && valid(x2) && valid(y2) &&
         states[x1][y1][x2][y2][0] != INVALID;
}

void solve() {
  auto [x1, y1, x2, y2, p] = st.top();

  if (states[x1][y1][x2][y2][p] != UNDEF) {
    st.pop();
    return;
  }

  if (distance(x1, y1, x2, y2) == 1) {
    states[x1][y1][x2][y2][p] = WIN;
    states[x1][y1][x2][y2][1 - p] = WIN;
    changed = true;
    st.pop();
    return;
  }

  int n_child = 0, n_win = 0;

  if (p == 0) {
    for (auto const &step : steps) {
      int xn = x1 + step[0];
      int yn = y1 + step[1];
      if (!valid(xn, yn, x2, y2)) {
        continue;
      }
      if (!visited[xn][yn][x2][y2][1]) {
        visited[xn][yn][x2][y2][1] = true;
        st.push(make_tuple(xn, yn, x2, y2, 1));
        return;
      } else {
        if (states[xn][yn][x2][y2][1] == LOSS) {
          states[x1][y1][x2][y2][0] = WIN;
          changed = true;
          st.pop();
          return;
        } else {
          n_win += states[xn][yn][x2][y2][1] == WIN;
        }
      }
      ++n_child;
    }
  } else {
    for (auto const &step : steps) {
      int xn = x2 + step[0];
      int yn = y2 + step[1];
      if (!valid(x1, y1, xn, yn)) {
        continue;
      }
      if (!visited[x1][y1][xn][yn][0]) {
        visited[x1][y1][xn][yn][0] = true;
        st.push(make_tuple(x1, y1, xn, yn, 0));
        return;
      } else {
        if (states[x1][y1][xn][yn][0] == LOSS) {
          states[x1][y1][x2][y2][1] = WIN;
          changed = true;
          st.pop();
          return;
        } else {
          n_win += states[x1][y1][xn][yn][0] == WIN;
        }
      }
      ++n_child;
    }
  }

  if (n_child == 0) {
    states[x1][y1][x2][y2][p] = DRAW;
    states[x1][y1][x2][y2][1 - p] = DRAW;
    changed = true;
  } else if (n_win == n_child) {
    states[x1][y1][x2][y2][p] = LOSS;
    changed = true;
  }

  st.pop();
}

int main() {
  cin >> n >> m;
  cin >> swx >> swy >> sbx >> sby;
  --swx;
  --swy;
  --sbx;
  --sby;

  mark_same();
  for (int i = 0, x, y; i < m; ++i) {
    cin >> x >> y;
    mark_invalid(x - 1, y - 1);
  }

  while (changed) {
    changed = false;
    std::fill(&visited[0][0][0][0][0],
              &visited[0][0][0][0][0] + sizeof(visited), 0);
    visited[swx][swy][sbx][sby][0] = true;
    st.push(make_tuple(swx, swy, sbx, sby, 0));
    while (!st.empty()) {
      solve();
    }
  }

  switch (states[swx][swy][sbx][sby][0]) {
  case WIN:
    cout << "White\n";
    break;
  case LOSS:
    cout << "Black\n";
    break;
  case INVALID:
    std::terminate();
  case DRAW:
  case UNDEF:
    cout << "Draw\n";
    break;
  }
  return 0;
}
