#include <iostream>
#include <vector>
#include <utility>

using namespace std;

void read(vector<vector<int>>& v) {
  for (int i = 0; i < v.size(); ++i) {
    for (int j = 0; j < v[0].size(); ++j) {
      cin >> v[i][j];
    }
  }
}

int main() {
  int m, n;
  cin >> m >> n;

  vector<vector<int>> a(m, vector<int>(n)), b(m, vector<int>(n));
  read(a);
  read(b);

  vector<int> amr(m), bmr(m), amc(n), bmc(n);
  for (int i = 0; i < m; ++i) {
    amr[i] = bmr[i] = -1e7; 
    for (int j = 0; j < n; ++j) {
      amr[i] = max(amr[i], a[i][j]);
      bmr[i] = max(bmr[i], b[i][j]);
    }
  }
  for (int j = 0; j < n; ++j) {
    amc[j] = bmc[j] = -1e7;
    for (int i = 0; i < m; ++i) {
      amc[j] = max(amc[j], a[i][j]);
      bmc[j] = max(bmc[j], b[i][j]);
    }
  }

  vector<pair<int, int>> res;
  for (int s1 = 0; s1 < m; ++s1) {
    for (int s2 = 0; s2 < n; ++s2) {
      if (a[s1][s2] == amc[s2] && b[s1][s2] == bmr[s1]) {
        res.emplace_back(s1, s2);
      }
    }
  }

  cout << res.size() << '\n';
  for (auto&& [i, j] : res) {
    cout << i + 1 << ' ' << j + 1 << '\n';
  }

  return 0;
}
