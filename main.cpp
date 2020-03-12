#include <bits/stdc++.h>  // clang-format off
using namespace std;
using Int = long long;
#define REP_(i, a_, b_, a, b, ...) for (int i = (a), lim_i = (b); i < lim_i; i++)
#define REP(i, ...) REP_((i), __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
struct SetupIO { SetupIO() { cin.tie(nullptr), ios::sync_with_stdio(false), cout << fixed << setprecision(13); } } setup_io;
#ifndef _MY_DEBUG
#define dump(...)
#endif  // clang-format on

const vector<pair<int, int>> DIRECTIONS = {
    {1, 0}, {0, 1},  {-1, 0},  {0, -1},  // 4方向
    {1, 1}, {-1, 1}, {-1, -1}, {1, -1},  // 斜め
    {0, 0},                              // 自身
};

constexpr int MAX_SKILL_COUNT = 400;

struct Developer {
    int company, bonus;
    bitset<MAX_SKILL_COUNT> skills;
};

struct Manager {
    int company, bonus;
};

int num_developer, num_manager;
int W, H;
vector<string> board;
vector<Developer> developers;
vector<Manager> managers;

// 解を入れる配列
// H * Wでpos[i][j]は-1のとき、空いていてDeveloperが使える。-2のとき、空いていてManagerが使える
// -3のとき、使えない
// 0 <= i < nのとき、i番目のDeveloper
// n <= i < n + mのときi - n番目のManager
vector<vector<int>> ans_pos;

void initialize_by_valid() {
    vector<pair<int, int>> valid_developer_pos, valid_manager_pos;
    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {
            if (ans_pos[i][j] == -1) valid_developer_pos.emplace_back(i, j);
            if (ans_pos[i][j] == -2) valid_manager_pos.emplace_back(i, j);
        }
    }
    for (int i = 0; i < num_developer; i++) {
        if (i < valid_developer_pos.size()) {
            auto [a, b] = valid_developer_pos[i];
            ans_pos[a][b] = i;
        }
    }
    for (int i = 0; i < num_manager; i++) {
        if (i < valid_manager_pos.size()) {
            auto [a, b] = valid_manager_pos[i];
            ans_pos[a][b] = i + num_developer;
        }
    }
}

void print_solution() {
    vector<pair<int, int>> developer_pos(num_developer, {-1, -1}), manager_pos(num_manager, {-1, -1});
    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {
            if (0 <= ans_pos[i][j]) {
                if (ans_pos[i][j] < num_developer) {
                    developer_pos[ans_pos[i][j]] = {i, j};
                } else {
                    manager_pos[ans_pos[i][j] - num_developer] = {i, j};
                }
            }
        }
    }
    for (int i = 0; i < num_developer; i++) {
        if (developer_pos[i].first == -1) {
            cout << 'X' << '\n';
        } else {
            cout << developer_pos[i].second << ' ' << developer_pos[i].first << '\n';
        }
    }
    for (int i = 0; i < num_manager; i++) {
        if (manager_pos[i].first == -1) {
            cout << 'X' << '\n';
        } else {
            cout << manager_pos[i].second << ' ' << manager_pos[i].first << '\n';
        }
    }
}

inline int work_potential(const Developer& a, const Developer& b) {
    int cap = (a.skills & b.skills).count();
    int cup = (a.skills | b.skills).count();
    return cap * (cup - cap);
}

inline int bonus_potential(const Developer& a, const Developer& b) {
    return a.company == b.company ? a.bonus * b.bonus : 0;
}

inline int bonus_potential(const Developer& a, const Manager& b) {
    return a.company == b.company ? a.bonus * b.bonus : 0;
}

inline int bonus_potential(const Manager& a, const Manager& b) {
    return a.company == b.company ? a.bonus * b.bonus : 0;
}

int total_potential(const vector<vector<int>>& pos, int i1, int j1, int i2, int j2) {
    if (pos[i1][j1] < 0 || pos[i2][j2] < 0) return 0;
    int ret = 0;
    if (pos[i1][j1] < num_developer && pos[i2][j2] < num_developer) {
        int dev1 = pos[i1][j1], dev2 = pos[i2][j2];
        ret += work_potential(developers[dev1], developers[dev2]);
        if (developers[dev1].company == developers[dev2].company) {
            ret += bonus_potential(developers[dev1], developers[dev2]);
        }
    } else if (pos[i1][j1] < num_developer) {
        int dev1 = pos[i1][j1], man2 = pos[i2][j2] - num_developer;
        if (developers[dev1].company == managers[man2].company) {
            ret += bonus_potential(developers[dev1], managers[man2]);
        }
    } else if (pos[i2][j2] < num_developer) {
        int man1 = pos[i1][j1] - num_developer, dev2 = pos[i2][j2];
        if (managers[man1].company == developers[dev2].company) {
            ret += bonus_potential(developers[dev2], managers[man1]);
        }
    } else {
        int man1 = pos[i1][j1] - num_developer, man2 = pos[i2][j2] - num_developer;
        if (managers[man1].company == managers[man2].company) {
            ret += bonus_potential(managers[man1], managers[man2]);
        }
    }
    return ret;
}

int calc_score(vector<vector<int>>& pos) {
    int score = 0;
    REP(i1, H) {
        REP(j1, W) {
            if (pos[i1][j1] < 0) continue;
            REP(k, 2) {
                int i2 = i1 + DIRECTIONS[k].first;
                int j2 = j1 + DIRECTIONS[k].second;
                if (i2 >= H || j2 >= W || pos[i2][j2] < 0) continue;
                int tmp = total_potential(pos, i1, j1, i2, j2);
                score += total_potential(pos, i1, j1, i2, j2);
            }
        }
    }
    return score;
}

void read_input() {
    cin >> W >> H;
    board.resize(H);
    ans_pos.assign(H, vector<int>(W, -3));
    for (int i = 0; i < H; i++) cin >> board[i];
    int avail_developer_cnt = 0;
    int avail_manager_cnt = 0;
    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {
            if (board[i][j] == '_') {
                ans_pos[i][j] = -1;
                avail_developer_cnt++;
            }
            if (board[i][j] == 'M') {
                ans_pos[i][j] = -2;
                avail_manager_cnt++;
            }
        }
    }
    cerr << "available developer cnt: " << avail_developer_cnt << endl;
    cerr << "available manager cnt: " << avail_manager_cnt << endl;
    int n;
    cin >> n;
    cerr << "input developer cnt: " << n << endl;
    num_developer = n;
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
    assert(N <= MAX_SKILL_COUNT);
    for (int i = 0; i < n; i++) {
        for (const string& skill : skills[i]) {
            developers[i].skills[skill_map[skill]] = 1;
        }
    }
    int m;
    cin >> m;
    num_manager = m;
    cerr << "input manager cnt: " << m << endl;
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

const char* input_files[] = {"test/a_solar.txt",     "test/b_dream.txt",  "test/c_soup.txt",
                             "test/d_maelstrom.txt", "test/e_igloos.txt", "test/f_glitch.txt"};

const char* output_files[] = {"sol/a.sol", "sol/b.sol", "sol/c.sol", "sol/d.sol", "sol/e.sol", "sol/f.sol"};

void redirect_io(int k) {
    assert(freopen(input_files[k], "r", stdin));
    assert(freopen(output_files[k], "w", stdout));
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    int n;
    cin >> n;
    redirect_io(n);
    read_input();

    initialize_by_valid();
    cerr << "initial score: " << calc_score(ans_pos) << endl;
    print_solution();
}
