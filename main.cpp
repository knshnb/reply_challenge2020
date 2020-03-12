#include <bits/stdc++.h>  // clang-format off
using namespace std;
using Int = long long;
#define REP_(i, a_, b_, a, b, ...) for (int i = (a), lim_i = (b); i < lim_i; i++)
#define REP(i, ...) REP_((i), __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
struct SetupIO { SetupIO() { cin.tie(nullptr), ios::sync_with_stdio(false), cout << fixed << setprecision(13); } } setup_io;
#ifndef _MY_DEBUG
#define dump(...)
#endif  // clang-format on

struct Developer {
    int company, bonus;
    vector<int> skills;
};

struct Manager {
    int company, bonus;
};

int W, H;
vector<string> board;
vector<Developer> developers;
vector<Manager> managers;

void read_input() {
    cin >> W >> H;
    board.resize(H);
    for (int i = 0; i < H; i++) cin >> board[i];
    int n;
    cin >> n;
    map<string, int> company_map;
    map<string, int> skill_map;
    vector<string> companies;
    vector<vector<string>> skills;
    companies.resize(n);
    skills.resize(n);
    for (int i = 0; i < n; i++) {
        Developer developer;
        cin >> companies[i] >> developer.bonus;
        company_map[companies[i]];
        developers.push_back(developer);
        int k;
        cin >> k;
        skills[i].resize(k);
        for (int j = 0; j < k; j++) {
            cin >> skills[i][j];
            skill_map[skills[i][j]];
        }
    }
    int N = 0;
    for (auto& p : skill_map) {
        p.second = N++;
    }
    cerr << "total skill kind: " << N << endl;
    for (int i = 0; i < n; i++) {
        developers[i].skills.resize(skills[i].size());
        for (int j = 0; j < skills[i].size(); j++) {
            developers[i].skills[j] = skill_map[skills[i][j]];
        }
    }
    int m;
    cin >> m;
    for (int i = 0; i < m; i++) {
        string s;
        Manager manager;
        cin >> s >> manager.bonus;
        managers.push_back(manager);
        company_map[s];
        companies.push_back(s);
    }
    N = 0;
    for (auto& p : company_map) {
        p.second = N++;
    }
    cerr << "total company kind: " << N << endl;
    for (int i = 0; i < n; i++) {
        developers[i].company = company_map[companies[i]];
    }
    for (int i = 0; i < m; i++) {
        managers[i].company = company_map[companies[i + n]];
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    // assert(freopen("test/a_solar.txt", "r", stdin));
    // assert(freopen("test/b_dream.txt", "r", stdin));
    assert(freopen("test/c_soup.txt", "r", stdin));
    read_input();
}
