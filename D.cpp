#include <iostream>
#include <vector>

using namespace std;

int mex(vector<int> const& v) {
  static int max_g = 1000000;
  static vector<bool> used(1 + max_g);
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

vector<int> grandy(int s) {
  vector<int> result(1 + s);
  vector<int> child_grandy;
  for (int i = 2; i <= s; ++i) {
    int max_cut = i / 2;
    child_grandy.resize(max_cut); 
    for (int c = 1; c <= max_cut; ++c) {
      child_grandy[c - 1] = result[i - c];
    }
    result[i] = mex(child_grandy);
  }
  return result;
}

int main() {
  int d[3] = {0};
  int r[3] = {0};
  int start_grandy[3] = {0};
  bool win = false;
  
  cin >> d[0] >> d[1] >> d[2];
  auto grandy_v = grandy(max(d[0], max(d[1], d[2])));

  for (int i = 0; i < 3; ++i) {
    start_grandy[i] = grandy_v[d[i]];
    r[i] = d[i];
  }

  for (int i = 0; i < 3 && !win; ++i) {
    int max_cut = d[i] / 2;
    int other_xor = 0;
    for (int j = 0; j < 3; ++j) {
      if (j != i) {
        other_xor ^= start_grandy[j];
      }
    }
    for (int c = 1; c <= max_cut; ++c) {
      int res_size = d[i] - c;
      if ((other_xor ^ grandy_v[res_size]) == 0) {
        r[i] = res_size;
        win = true;
        break;
      }
    }
  }

  if (win) {
    cout << "YES\n";
    for (int i = 0; i < 3; ++i) {
      cout << r[i] << ' ';
    }
  } else {
    cout << "NO";
  }
 
  return 0;
}
