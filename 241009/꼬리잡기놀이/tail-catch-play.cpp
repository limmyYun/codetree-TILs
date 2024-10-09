#include<iostream>
#include<tuple>
#include<vector>
#include <queue>

using namespace std;

int n, m, k;
int map[21][21];
int ans;

int dx[4] = { 1, 0, -1, 0 };
int dy[4] = { 0, -1, 0, 1 };

void move(int hr, int hc) {
	// 꼬리 사람 찾기
	bool visited[21][21];
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			visited[i][j] = false;
		}
	}
	queue<tuple<int, int>> que;
	que.push(make_tuple(hr, hc));
	visited[hr][hc] = true;
	int tr = 0, tc = 0;
	bool found = false;
	while (!que.empty()) {
		tie(tr, tc) = que.front();
		que.pop();

		for (int d = 0; d < 4; d++) {
			int sr = tr + dy[d];
			int sc = tc + dx[d];
			if (sr < 0 || sc < 0 || sr >= n || sc >= n) continue;
			if (map[sr][sc] == 4 || map[sr][sc]==0) continue;
			if (visited[sr][sc]) continue;

			visited[sr][sc] = true;
			if (map[sr][sc] == 3) {
				found = true;
				tr = sr;
				tc = sc;
			}
			que.push(make_tuple(sr, sc));
		}
		if (found) break;
	}

	// 찾은 꼬리 사람부터 앞쪽으로 1칸씩 이동
	int moveVal = map[tr][tc], tmpV = 0;;
	int cr = tr, cc = tc;
	int tarR = 0, tarC = 0, tarV = 0;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			visited[i][j] = false;
		}
	}
	// 3부터 1 전까지 이동
	while (true) {
		for (int d = 0; d < 4; d++) {
			int sr = cr + dy[d];
			int sc = cc + dx[d];
			if (sr < 0 || sc < 0 || sr >= n || sc >= n) continue;
			if (map[sr][sc] == 2) {
				tarR = sr;
				tarC = sc;
				tarV = 2;
				break;
			}
			else if (map[sr][sc] == 1) {
				tarR = sr;
				tarC = sc;
				tarV = 1;
			}
		}

		if (tarV == 2) {
			if(moveVal == 3) map[cr][cc] = 4;
			tmpV = map[tarR][tarC];
			map[tarR][tarC] = moveVal;
			visited[tarR][tarC] = true;
			moveVal = tmpV;
			cr = tarR;
			cc = tarC;
		}
		else if(tarV == 1) {
			if (moveVal == 3) map[cr][cc] = 4;
			map[tarR][tarC] = moveVal;
			visited[tarR][tarC] = true;
			cr = tarR;
			cc = tarC;
			moveVal = 1;
			break;
		}
	}
	// 1 이동
	for (int d = 0; d < 4; d++) {
		int sr = cr + dy[d];
		int sc = cc + dx[d];
		if (sr < 0 || sc < 0 || sr >= n || sc >= n) continue;
		if (map[sr][sc] == 4) {
			map[sr][sc] = 1;
			break;
		}
	}
}

tuple<int,int> throwBall(int r, int c, int d){
	int rr = -1, rc = -1;

	r = r - dy[d];
	c = c - dx[d];

	for (int i = 0; i < n; i++) {
		r = r + dy[d];
		c = c + dx[d];

		if (map[r][c] == 1 || map[r][c] == 2 || map[r][c] == 3) {
			rr = r;
			rc = c;
			return make_tuple(rr,rc);
		}
	}
	return make_tuple(rr, rc);
}

void check(int r, int c) {
	int fr = -1, fc = -1, er = -1, ec = -1;
	bool visited[21][21];
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			visited[i][j] = false;
		}
	}

	queue<tuple<int, int>> que;
	que.push(make_tuple(r, c));
	visited[r][c] = true;

	// 지금 좌표가 머리 or 꼬리인지 확인
	if (map[r][c] == 1) {
		fr = r;
		fc = c;
	}
	else if (map[r][c] == 3) {
		er = r;
		ec = c;
	}

	int cr = 0, cc = 0;
	while (fr==-1 || fc==-1 || er == -1 || ec == -1) {
		tie(cr, cc) = que.front();
		que.pop();

		for (int d = 0; d < 4; d++) {
			int sr = cr + dy[d];
			int sc = cc + dx[d];
			if (sr < 0 || sc < 0 || sr >= n || sc >= n) continue;
			if (visited[sr][sc]) continue;
			if (map[sr][sc] == 0 || map[sr][sc]==4) continue;

			if (map[sr][sc] == 1) {
				fr = sr;
				fc = sc;
			}
			else if (map[sr][sc] == 3) {
				er = sr;
				ec = sc;
			}
			visited[sr][sc] = true;
			que.push(make_tuple(sr, sc));
		}
	}

	// 머리 사람, 꼬리 사람 찾았으면 내 위치가 몇번째인지 확인
	int score = 0;
	// 현재 내가 머리 인지 확인
	if (r == fr && c == fc) {
		score = 1;
	}
	else {
		// 머리부터 시작해서 내 위치까지 찾기
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				visited[i][j] = false;
			}
		}

		queue<tuple<int, int, int>> que2;
		que2.push(make_tuple(fr, fc, 1));
		visited[fr][fc] = true;

		int curR = 0, curC = 0, curV = 0;
		int tmpR = 0, tmpC = 0;
		while (true) {
			tie(curR, curC, curV) = que2.front();
			que2.pop();

			for (int d = 0; d < 4; d++) {
				int sr = curR + dy[d];
				int sc = curC + dx[d];
				if (sr < 0 || sc < 0 || sr >= n || sc >= n) continue;
				if (visited[sr][sc]) continue;
				if (map[sr][sc] == 2) {
					tmpR = sr;
					tmpC = sc;
					break;
				}
				else if (map[sr][sc] == 3) {
					tmpR = sr;
					tmpC = sc;
				}
			}

			if (tmpR == r && tmpC == c) {
				score = (curV + 1) * (curV + 1);
				break;
			}
			visited[tmpR][tmpC] = true;
			que2.push(make_tuple(tmpR, tmpC, curV+1));
		}
	}
	ans += score;

	// 방향 전환
	map[fr][fc] = 3;
	map[er][ec] = 1;
}

int main() {
	cin >> n >> m >> k;
	int tmp = 0;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			cin >> tmp;
			map[i][j] = tmp;
		}
	}

	// 겉 면 회전해서 시작 위치, 방향 저장하기
	vector<tuple<int, int, int>> vec;
	for (int i = 0; i < n; i++) {
		vec.push_back(make_tuple(i, 0, 0));
	}
	for (int i = 0; i < n; i++) {
		vec.push_back(make_tuple(n - 1, i, 1));
	}
	for (int i = n-1; i >= 0; i--) {
		vec.push_back(make_tuple(i, n-1, 2));
	}
	for (int i = n - 1; i >= 0; i--) {
		vec.push_back(make_tuple(0, i, 3));
	}
	

	for (int t = 0; t < k; t++) {
		// 머리 사람 따라 1칸 이동
		vector<tuple<int, int>> v;

		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				if (map[i][j] == 1) {
					// 머리 사람 찾았으니 이동
					v.push_back(make_tuple(i, j));
				}
			}
		}

		for (auto head : v) {
			move(get<0>(head), get<1>(head));
		}

		// 공 던지기
		int idx = t % ((n-1)*(n-1));
		int gr = -1, gc = -1;
		tie(gr, gc) = throwBall(get<0>(vec[idx]), get<1>(vec[idx]), get<2>(vec[idx]));
		if ((gr == -1) && (gc == -1)) continue;

		// 공 받았으면 머리 사람, 꼬리 사람 찾아서 변경 + 점수 계산
		check(gr, gc);
	}

	cout << ans;
}