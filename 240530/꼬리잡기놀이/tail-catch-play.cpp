#include <iostream>
#include <tuple>
#include <vector>
#include <queue>

using namespace std;

int ans, n, m, k;
int map[21][21];
vector<tuple<int, int>> head;

int dx[4] = { 1, 0, -1, 0 };
int dy[4] = { 0, -1, 0, 1 };

void printDebug() {
    cout << endl;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << map[i][j] << " ";
        }
        cout << endl;
    }
}

void moveTeam(int r, int c, int he) {
    // 1을 먼저 옮겨줘
    // 직전 위치 가지고 있고
    // 새로운 위치가 1이거나 4일때까지 업데이트 해주면 되지

    int tmap[21][21];
    bool visited[21][21];
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            tmap[i][j] = 0;
            visited[i][j] = false;
        }
    }

    // move 1
    for (int d = 0; d < 4; d++) {
        int sr = r + dy[d];
        int sc = c + dx[d];
        if (sr < 0 || sc < 0 || sr >= n || sc >= n) continue;

        if (map[sr][sc] == 3 || map[sr][sc] == 4) {
            tmap[sr][sc] = 1;
            get<0>(head[he]) = sr;
            get<1>(head[he]) = sc;
        }
    }

    int cr = r, cc = c;
    visited[r][c] = true;
    queue<tuple<int, int>> que;
    que.push(make_tuple(r, c));

    while (!que.empty()) {
        tie(cr, cc) = que.front();
        que.pop();

        for (int d = 0; d < 4; d++) {
            int sr = cr + dy[d];
            int sc = cc + dx[d];
            if (sr < 0 || sc < 0 || sr >= n || sc >= n || visited[sr][sc]) continue;
            if (map[sr][sc] == 0 || map[sr][sc] == 4) continue;

            visited[sr][sc] = true;
            que.push(make_tuple(sr, sc));
            tmap[cr][cc] = map[sr][sc];
            cr = sr;
            cc = sc;
        }
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (tmap[i][j] != 0) {
                map[i][j] = tmap[i][j];
            }
            else if (tmap[i][j] == 0 && map[i][j] == 3 && visited[i][j]) {
                map[i][j] = 4;
            }
        }
    }
}

void move() {
    int tmpr = 0, tmpc = 0;

    for (int i = 0; i < head.size(); i++) {
        // move this team
        //cout << "head : " << get<0>(head[i]) << " , " << get<1>(head[i]) << endl;
        moveTeam(get<0>(head[i]), get<1>(head[i]), i);
    }
}

tuple<int, int> throwBall(int round, int d) {
    int returnR = -1, returnC = -1;
    if (d == 0) {
        for (int i = 0; i < n; i++) {
            if (map[round][i] == 0 || map[round][i] == 4) continue;
            returnR = round;
            returnC = i;
            break;
        }
    }
    else if (d == 1) {
        for (int i = n - 1; i >= 0; i--) {
            if (map[i][round] == 0 || map[i][round] == 4) continue;
            returnR = i;
            returnC = round;
            break;
        }
    }
    else if (d == 2) {
        round = (n - 1 - round);
        for (int i = n - 1; i >= 0; i--) {
            if (map[round][i] == 0 || map[round][i] == 4) continue;
            returnR = round;
            returnC = i;
            break;
        }
    }
    else if (d == 3) {
        round = (n - 1 - round);
        for (int i = 0; i < n; i++) {
            if (map[i][round] == 0 || map[i][round] == 4) continue;
            returnR = i;
            returnC = round;
            break;
        }
    }

    return make_tuple(returnR, returnC);
}

int getScoreAndChange(int r, int c) {
    int fr = 0, fc = 0, er = 0, ec = 0, pos = 0;
    int score = 0;
    bool visited[21][21];
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            visited[i][j] = false;
        }
    }

    queue<tuple<int, int, int>> que;
    que.push(make_tuple(r, c, 1));
    if (map[r][c] == 1) {
        pos = 1;
        fr = r;
        fc = c;
    }
    else if (map[r][c] == 3) {
        er = r;
        ec = c;
    }
    visited[r][c] = true;

    int nr = 0, nc = 0, np = 0;
    while (!que.empty()) {
        tie(nr, nc, np) = que.front();
        que.pop();

        for (int d = 0; d < 4; d++) {
            int sr = nr + dy[d];
            int sc = nc + dx[d];
            if (sr < 0 || sc < 0 || sr >= n || sc >= n) continue;
            if (map[sr][sc] == 0 || map[sr][sc] == 4 || visited[sr][sc]) continue;

            que.push(make_tuple(sr, sc, np + 1));
            visited[sr][sc] = true;
            if (map[sr][sc] == 1) {
                pos = np + 1;
                fr = sr;
                fc = sc;
            }
            else if (map[sr][sc] == 3) {
                er = r;
                ec = c;
            }
        }
    }

    // change
    int tmpF = map[fr][fc];
    int tmpE = map[er][ec];
    map[fr][fc] = tmpE;
    map[er][ec] = tmpF;

    for (int i = 0; i < head.size(); i++) {
        if (get<0>(head[i]) == fr && get<1>(head[i]) == fc) {
            get<0>(head[i]) = er;
            get<1>(head[i]) = ec;
        }
    }

    return pos * pos;
}

int main() {
    cin >> n >> m >> k;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cin >> map[i][j];
            if (map[i][j] == 1) head.push_back(make_tuple(i, j));
        }
    }

    int dir = 0;
    int round = -1;
    int tr = 0, tc = 0;

    for (int i = 0; i < k; i++) {
        // Round 시작

        // 머리 사람 따라서 이동
        move();
        //cout << "after move" << endl;
        //printDebug();

        // 공 던지기
        round++;
        if (round >= n) {
            dir = (dir + 1) % 4;
            round = 0;
        }
        tie(tr, tc) = throwBall(round, dir);
        if (tr == -1 && tc == -1) continue;

        // 점수 계산
        //cout << "tr : " << tr << " tc : " << tc << endl;
        ans += getScoreAndChange(tr, tc);
        //printDebug();
        //cout << "round : " << i + 1 << " : " << ans << endl;
    }

    cout << ans;
    return 0;
}