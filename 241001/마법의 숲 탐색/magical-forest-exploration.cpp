#include <iostream>
#include <queue>
#include <vector>
#include <tuple>
#include <string.h>

using namespace std;

int R, C, K;
int map[71][71];
bool center[71][71];
bool out[71][71];

int dx[4] = { 0, 1, 0, -1 };
int dy[4] = { -1, 0, 1, 0 };
queue<tuple<int, int>> info;

int cr, cc, cd;
int cnt = 3;

bool down() {
	bool isOk = true;

	int mr = cr + 1;
	int mc = cc;

	for (int d = 1; d < 4; d++) {
		int sr = mr + dy[d];
		int sc = mc + dx[d];
		if ((mr < 0) && (sr < 0 || sr >= R)) continue;
		if ((sr < 0 || sc < 0 || sr >= R || sc >= C) || (map[sr][sc] != 0)) {
			isOk = false;
			break;
		}
		isOk = true;
	}
	if (isOk) {
		cr = mr;
		cc = mc;
	}
	return isOk;
}

bool leftDown() {
	bool isOk = true;

	int mr = cr;
	int mc = cc - 1;

	// left check
	for (int d = 2; d < 5; d++) {
		int sr = mr + dy[d % 4];
		int sc = mc + dx[d % 4];
		if (sc < 0 || sc >= C) {
			isOk = false;
			break;
		}
		if ((mr < 1) && (sr < 0 || sr >= R)) continue;
		if ((sr < 0 || sc < 0 || sr >= R || sc >= C) || (map[sr][sc] != 0)) {
			isOk = false;
			break;
		}
		isOk = true;
	}
	if (!isOk) return isOk;

	// down check
	isOk = true;
	mr = mr + 1;
	for (int d = 1; d < 4; d++) {
		int sr = mr + dy[d];
		int sc = mc + dx[d];
		if ((mr < 0) && (sr < 0 || sr >= R)) continue;
		if ((sr < 0 || sc < 0 || sr >= R || sc >= C) || (map[sr][sc] != 0)) {
			isOk = false;
			break;
		}
		isOk = true;
	}
	if (isOk) {
		cr = mr;
		cc = mc;
		cd = (cd - 1 + 4) % 4;
	}
	return isOk;
}

bool rightDown() {
	bool isOk = true;

	int mr = cr;
	int mc = cc + 1;

	// right check
	for (int d = 0; d < 3; d++) {
		int sr = mr + dy[d % 4];
		int sc = mc + dx[d % 4];
		if (sc < 0 || sc >= C) {
			isOk = false;
			break;
		}
		if ((mr < 1) && (sr < 0 || sr >= R)) continue;
		if ((sr < 0 || sc < 0 || sr >= R || sc >= C) || (map[sr][sc] != 0)) {
			isOk = false;
			break;
		}
		isOk = true;
	}
	if (!isOk) return isOk;

	// down check
	isOk = true;
	mr = mr + 1;
	for (int d = 1; d < 4; d++) {
		int sr = mr + dy[d];
		int sc = mc + dx[d];
		if ((mr < 0) && (sr < 0 || sr >= R)) continue;
		if ((sr < 0 || sc < 0 || sr >= R || sc >= C) || (map[sr][sc] != 0)) {
			isOk = false;
			break;
		}
		isOk = true;
	}
	if (isOk) {
		cr = mr;
		cc = mc;
		cd = (cd + 1) % 4;
	}
	return isOk;
}

int moveMan(int r, int c, int v) {
	bool visited[71][71];
	for (int i = 0; i < R; i++) {
		for (int j = 0; j < C; j++) {
			visited[i][j] = false;
		}
	}

	queue<tuple<int, int, int>> que;
	que.push(make_tuple(r, c, v));
	/*cout << "r : " << r << " c : " << c << endl;*/
	visited[r][c] = true;
	int maxR = r;

	int nr = 0, nc = 0, nv = 0;
	while (!que.empty()) {
		tie(nr, nc, nv) = que.front();
		que.pop();

		for (int d = 0; d < 4; d++) {
			int sr = nr + dy[d];
			int sc = nc + dx[d];
			if (sr < 0 || sc < 0 || sr >= R || sc >= C) continue;
			if (map[sr][sc] == 0) continue;
			if (visited[sr][sc]) continue;


			if (map[sr][sc] == nv) {
				if (maxR < sr) maxR = sr;
				visited[sr][sc] = true;
				que.push(make_tuple(sr, sc, map[sr][sc]));
			}
			else {
				if (out[nr][nc]) {
					if (maxR < sr) maxR = sr;
					visited[sr][sc] = true;
					que.push(make_tuple(sr, sc, map[sr][sc]));
				}
			}
		}
	}

	return maxR + 1;
}

int main() {
	cin >> R >> C >> K;

	for (int i = 0; i < K; i++) {
		int c = 0, d = 0;
		cin >> c >> d;
		info.push(make_tuple(c, d));
	}

	for (int i = 0; i < R; i++) {
		for (int j = 0; j < C; j++) {
			map[i][j] = 0;
			center[i][j] = false;
			out[i][j] = false;
		}
	}

	int nowC = 0, nowD = 0;
	int total = 0;
	for (int i = 0; i < K; i++) {
		tie(nowC, nowD) = info.front();
		info.pop();
		cr = -2, cc = nowC - 1, cd = nowD;

		bool noCal = false;

		// 최대한 남쪽으로 이동
		while (true) {
			bool isExit = false;

			isExit = down();
			if (isExit) continue;
			isExit = leftDown();
			if (isExit) continue;
			isExit = rightDown();

			if (isExit == false) {
				// 더이상 갈 수 없고 map 초과했을 때
				if (cr < 1 || cc == 0 || cc == C - 1 || cr == R - 1) {
					// map clear
					for (int i = 0; i < R; i++) {
						for (int j = 0; j < C; j++) {
							map[i][j] = 0;
							center[i][j] = false;
							out[i][j] = false;
						}
					}
					noCal = true;
				}
				else {
					// map 채우기
					map[cr][cc] = cnt;
					for (int d = 0; d < 4; d++) {
						int sr = cr + dy[d];
						int sc = cc + dx[d];
						map[sr][sc] = cnt;
						if (d == cd) out[sr][sc] = true;
					}
					center[cr][cc] = true;
					cnt++;
				}
				break;
			}
		}

		if (noCal) continue;

		// 정령 이동
		int n = moveMan(cr, cc, map[cr][cc]);
		total += n;
		cout << "i : " << i << " maxR : " << n << endl;
	}

	cout << total;
}