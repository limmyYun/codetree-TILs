#include <iostream>
#include <vector>
#include <queue>
#include <tuple>

using namespace std;

int L, N, Q;
int map[41][41];
int pMap[41][41];
int power[32];
int damage[32];
bool dead[32];
vector<tuple<int, int>> cmd;
int dx[4] = { 0, 1, 0, -1 };
int dy[4] = { -1, 0, 1, 0 };


vector<int> move(int p, int dir) {
	vector<int> hp;
	int tmp[41][41];
	bool updated[32];
	for(int i=0;i<L;i++){
		for (int j = 0; j < L; j++) {
			tmp[i][j] = -1;
		}
	}
	for (int i = 0; i < N; i++) {
		updated[i] = false;
	}

	bool moveFail = false;
	updated[p] = true;
	for (int i = 0; i < L; i++) {
		for (int j = 0; j < L; j++) {
			if (pMap[i][j] == p) {
				int sr = i + dy[dir];
				int sc = j + dx[dir];
				if (sr < 0 || sc < 0 || sr >= L || sc >= L || map[sr][sc] == 2) {
					moveFail = true;
					break;
				}
				if ((pMap[sr][sc] != p) && (pMap[sr][sc] >= 0) && !updated[pMap[sr][sc]]) {
					hp.push_back(pMap[sr][sc]);
					updated[pMap[sr][sc]] = true;
				}
				//tmp[i][j] = -1;
				tmp[sr][sc] = p;
			}
		}
		if (moveFail) break;
	}

	if (moveFail) {
		hp.clear();
		return hp;
	}

	int target = 0;
	int i = 0;
	while (true) {
		if (hp.size() < i + 1) break;
		target = hp[i];
		
		for (int i = 0; i < L; i++) {
			for (int j = 0; j < L; j++) {
				if (pMap[i][j] == target) {
					int sr = i + dy[dir];
					int sc = j + dx[dir];
					if (sr < 0 || sc < 0 || sr >= L || sc >= L || map[sr][sc] == 2) {
						moveFail = true;
						break;
					}
					if ((pMap[sr][sc] != target) && (pMap[sr][sc] >= 0) && !updated[pMap[sr][sc]]) {
						hp.push_back(pMap[sr][sc]);
						updated[pMap[sr][sc]] = true;
					}
					//tmp[i][j] = -1;
					tmp[sr][sc] = target;
				}
			}
			if (moveFail) break;
		}

		if (moveFail) break;
		i++;
	}

	if (moveFail) {
		hp.clear();
		return hp;
	}

	

	// move 성공하면 tmp 배열의 것 map 에 넣어주기
	for (int i = 0; i < L; i++) {
		for (int j = 0; j < L; j++) {
			int nowM = pMap[i][j];
			if (nowM != -1) {
				if (updated[nowM]) pMap[i][j] = -1;
			}

			int nowT = tmp[i][j];
			if (nowT != -1) {
				if (updated[nowT]) pMap[i][j] = tmp[i][j];
			}
		}
	}

	return hp;
}

void kill(int tar) {
	int da = 0;
	for (int i = 0; i < L; i++) {
		for (int j = 0; j < L; j++) {
			if (pMap[i][j] == tar) {
				if (map[i][j] == 1) da++;
			}
		}
	}

	damage[tar] += da;
	if ((power[tar] - damage[tar]) <= 0) dead[tar] = true;
}

int main() {
	cin >> L >> N >> Q;

	int v = 0;
	for (int i = 0; i < L; i++) {
		for (int j = 0; j < L; j++) {
			cin >> v;
			map[i][j] = v;
			pMap[i][j] = -1;
		}
	}

	int r = 0, c = 0, w = 0, h = 0, k = 0;
	for (int i = 0; i < N; i++) {
		cin >> r >> c >> h >> w >> k;
		r--, c--;
		power[i] = k;
		// pMap 채우기
		for (int a = c; a < w + c; a++) {
			for (int b = r; b < h + r; b++) {
				pMap[b][a] = i;
			}
		}
	}

	int tp = 0, td = 0;
	for (int i = 0; i < Q; i++) {
		cin >> tp >> td;
		tp--;
		cmd.push_back(make_tuple(tp, td));
	}

	// start
	int nowP = 0, nowD = 0;
	for (int t = 0; t < Q; t++) {
		tie(nowP, nowD) = cmd[t];
		if (dead[nowP]) continue;

		// 기사 이동 가능한지 확인후 밀려난 기사가 있을때 vector return
		vector<int> hurt = move(nowP, nowD);

		if (hurt.size() == 0) continue;
		
		// 대결 데미지 처리
		for (int i = 0; i < hurt.size(); i++) {
			int target = hurt[i];
			kill(target);
		}
	}

	int ans = 0;
	for (int i = 0; i < N; i++) {
		if (dead[i]) continue;
		ans += damage[i];
	}
	cout << ans;
}