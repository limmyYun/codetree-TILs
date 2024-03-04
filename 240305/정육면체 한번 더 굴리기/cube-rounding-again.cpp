#include <iostream>
#include <queue>
#include <tuple>

using namespace std;

int n, m;
int map[21][21];
int U, F, B;
int r, c, d;

int dx[4] = { -1, 0, 1, 0 };
int dy[4] = { 0, -1, 0, 1 };

void locationChange() {
    while (true) {
        int sx = r + dy[d];
        int sy = c + dx[d];

        if (sx < 0 || sy < 0 || sx >= n || sy >= n) {
            d = (d + 2) % 4;
            continue;
        }
        r = sx;
        c = sy;
        break;
    }

}

void roll() {
    int tu = 0, tf = 0, tb = 0;
    if (d == 0) {
        tu = B;
        tf = F;
        tb = 7 - U;
    }
    else if (d == 1) {
        tu = F;
        tf = 7 - U;
        tb = B;
    }
    else if (d == 2) {
        tu = 7 - B;
        tf = F;
        tb = U;
    }
    else if (d == 3) {
        tu = 7 - F;
        tf = U;
        tb = B;
    }
    U = tu;
    F = tf;
    B = tb;
}

int calculate() {
    int visited[21][21];
    int num = 0;
    int amount = 1;
    queue<tuple<int, int>> que;
    que.push(make_pair(r, c));
    visited[r][c] = 1;
    num = map[r][c];

    int x = 0;
    int y = 0;
    while (!que.empty()) {
        tie(x, y) = que.front();
        que.pop();

        for (int dir = 0; dir < 4; dir++) {
            int sx = x + dy[dir];
            int sy = y + dx[dir];
            if (sx < 0 || sy < 0 || sx >= n || sy >= n || visited[sx][sy] == 1) continue;
            if (map[sx][sy] != num) continue;

            que.push(make_pair(sx, sy));
            visited[sx][sy] = 1;
            amount++;
        }
    }

    return amount * num;
}

void changeDir() {
    int num = map[r][c];
    int below = 7 - U;

    if (below > num) {
        d = (d + 1) % 4;
    }
    else if (num > below) {
        d = (d + 3) % 4;
    }
}

int start() {
    int s = 0;
    // 격자 확인 후 위치 변경
    locationChange();
    // 주사위 굴리기
    roll();
    // 점수계산
    s = calculate();
    // 방향 변환
    changeDir();

    return s;
}

int main() {
    cin >> n >> m;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cin >> map[i][j];
        }
    }

    U = 1;
    F = 2;
    B = 3;
    r = 0;
    c = 0;
    d = 2;

    int ans = 0;
    int score = 0;
    for (int i = 0; i < m; i++) {
        score = start();
        ans += score;
    }

    cout << ans;

    return 0;
}