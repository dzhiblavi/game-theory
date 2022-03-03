#include <iostream>
#include <vector>
#include <cmath>
#include <cassert>
 
using namespace std;
 
#if 0
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
 
vector<int> sq_nim_naive(int s) {
  vector<int> g(1 + s);
  g[1] = 1;
  vector<int> ch;
  for (int i = 2; i <= s; ++i) {
    int max_take = sqrt(i);
    ch.resize(max_take);
    for (int take = 1; take <= max_take; ++take) {
      ch[take - 1] = g[i - take];
    }
    g[i] = mex(ch);
  }
  return g;
}
#endif
 
uint64_t sq_nim(uint64_t s) {
  uint64_t x = sqrt(s);
  for (;;) {
    uint64_t val = s - x * x;
    if ((val % (x + 1)) == 0) {
      return x;
    } else {
      s -= val / (x + 1) + 1;
      x -= 1;
    }
  }
}
 
 
int main() {
#if 0
  auto vg = sq_nim_naive(1000000);
  for (int i = 1; i <= 1000000; ++i) {
    assert(vg[i] == sq_nim(i));
  } 
  return 0;
#endif
 
  uint64_t n, xor_gr = 0;
  cin >> n;
 
  for (uint64_t i = 0, sz; i < n; ++i) {
    cin >> sz;
    xor_gr ^= sq_nim(sz);
  }
 
  if (xor_gr) {
    cout << "First\n";
  } else {
    cout << "Second\n";
  }
  return 0;
}
