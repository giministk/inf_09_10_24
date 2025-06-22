#include <iostream>
#include <vector>
#include <algorithm>
#include <locale>

using namespace std;

int main() {
    setlocale(LC_ALL, "Russian");

    int n;
    int m;
    std::vector<int> a_id;
    std::vector<int> b_id;

    std::cin >> n;

    std::vector<int> a(n);
    for (int i = 0; i < n; ++i) {
        std::cin >> a[i];
    }

    std::cin >> m;

    std::vector<int> b(m);
    for (int i = 0; i < m; ++i) {
        std::cin >> b[i];
    }

    std::vector<std::vector<int>> dp(n + 1, std::vector<int>(m + 1, 0));

    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= m; ++j) {
            dp[i][j] = std::max(dp[i - 1][j], dp[i][j - 1]);
            if (a[i - 1] == b[j - 1]) {
                dp[i][j] = std::max(dp[i][j], dp[i - 1][j - 1] + 1);
            }
        }
    }

    int i = n, j = m;
    while (i > 0 && j > 0) {
        if (a[i - 1] == b[j - 1]) {
            a_id.push_back(i - 1);
            b_id.push_back(j - 1);
            i--;
            j--;
        }
        else if (dp[i - 1][j] > dp[i][j - 1]) {
            i--;
        }
        else {
            j--;
        }
    };

    reverse(a_id.begin(), a_id.end());
    reverse(b_id.begin(), b_id.end());

    if (dp[n][m] == 0) {
        cout << "Совместимость нулевая :( общих элементов 0" << endl;
    }
    else {
        cout << "Длина НОП: " << dp[n][m] << std::endl;
        for (int idx : a_id) cout << idx << " ";
        cout << endl;
        for (int idx : b_id) cout << idx << " ";
        cout << endl;
    }
    return 0;
}