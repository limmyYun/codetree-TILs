#include <iostream>
#include <queue>
#include <vector>
#include <tuple>
#include <string.h>

using namespace std;

int k, m;
int map[5][5];
//int bestMap[5][5];
int num[301];
int counter = 0;

int row[9] = { 1, 2, 3, 1, 2, 3, 1, 2, 3 };
int col[9] = { 1, 1, 1, 2, 2, 2, 3, 3, 3 };
int dx[4] = { 0, 1, 0, -1 };
int dy[4] = { 1, 0, -1, 0 };

void rotate(int cen, int angle, int tmpMap[5][5]) {
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			tmpMap[i][j] = map[i][j];
		}
	}

	int r = row[cen];
	int c = col[cen];
	if (angle == 0) {
		// 90 회전
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				tmpMap[j+ (r-1)][2-i + (c-1)] = map[i + (r-1)][j + (c-1)];
			}
		}
	}
	else if (angle == 1) {
		// 180 회전
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				tmpMap[2-i + (r-1)][2-j + (c-1)] = map[i + (r-1)][j + (c-1)];
			}
		}
	}
	else if (angle == 2) {
		// 270 회전
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				tmpMap[2-j + (r-1)][i + (c-1)] = map[i + (r-1)][j + (c-1)];
			}
		}
	}
}

int check(int nowMap[5][5], int option) {
	bool visited[5][5];
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			visited[i][j] = false;
		}
	}

	vector<tuple<int, int>> vec;
	int totalNum = 0;
	
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			if (visited[i][j]) continue;
			queue < tuple<int, int, int>> que;
			int v = 1;
			que.push(make_tuple(i, j, nowMap[i][j]));
			visited[i][j] = true;

			while (!que.empty()) {
				int cr = 0, cc = 0, cn = 0;
				tie(cr, cc, cn) = que.front();
				que.pop();

				for (int d = 0; d < 4; d++) {
					int nr = cr + dy[d];
					int nc = cc + dx[d];
					if (nr < 0 || nc < 0 || nr >= 5 || nc >= 5) continue;
					if (visited[nr][nc]) continue;
					if (nowMap[nr][nc] != cn) continue;

					visited[nr][nc] = true;
					que.push(make_tuple(nr, nc, cn));
					v++;
				}
			}

			if (v >= 3) {
				vec.push_back(make_tuple(i, j));
				totalNum += v;
			}
		}
	}

	if (option == 0) return totalNum;

	// 0 으로 채우기
	for (auto v : vec) {
		int r = 0, c = 0;
		tie(r, c) = v;

		queue < tuple<int, int, int>> que;
		que.push(make_tuple(r, c, nowMap[r][c]));
		nowMap[r][c] = 0;

		while (!que.empty()) {
			int cr = 0, cc = 0, cn = 0;
			tie(cr, cc, cn) = que.front();
			que.pop();

			for (int d = 0; d < 4; d++) {
				int nr = cr + dy[d];
				int nc = cc + dx[d];
				if (nr < 0 || nc < 0 || nr >= 5 || nc >= 5) continue;
				if (nowMap[nr][nc] != cn) continue;

				que.push(make_tuple(nr, nc, cn));
				nowMap[nr][nc] = 0;
			}
		}
	}
	return totalNum;
}

void fill(int tmpMap[5][5]) {
	for (int j = 0; j < 5; j++) {
		for (int i = 4; i >= 0; i--) {
			if (tmpMap[i][j] != 0) continue;
			tmpMap[i][j] = num[counter++];
		}
	}
}

int main() {
	cin >> k >> m;
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			cin >> map[i][j];
		}
	}

	for (int i = 0; i < m; i++) {
		cin >> num[i];
	}

	// start
	
	for (int t = 0; t < k; t++) {
		int bestRC = -1;
		int bestA = -1;
		int bestV = 0;

		for (int i = 0; i < 9; i++) {
			for (int ang = 0; ang < 3; ang++) {
				int nowMap[5][5] = {0, };
				rotate(i, ang, nowMap);

				// 유물 획득 가치 확인
				int value = check(nowMap, 0);
				if (value > bestV) {
					bestV = value;
					bestRC = i;
					bestA = ang;
				}
				else if ((value == bestV) && (bestA > ang)) {
					bestV = value;
					bestRC = i;
					bestA = ang;
				}
			}
		}

		if (bestV == 0) break;
		
		bool found = true;
		int bestMap[5][5] = {};
		int totalValue = 0;
		rotate(bestRC, bestA, bestMap);
		while (found) {
			// 유물 획득 확인
			int nowValue = check(bestMap, 1);
			if (nowValue > 0) {
				found = true;
				totalValue += nowValue;
			}
			else {
				found = false;
				continue;
			}
			// 유물 획득하면 빈자리 채우기
			fill(bestMap);
		}

		for (int i = 0; i < 5; i++) {
			for (int j = 0; j < 5; j++) {
				map[i][j] = bestMap[i][j];
			}
		}
		cout << totalValue<<" ";
	}
}