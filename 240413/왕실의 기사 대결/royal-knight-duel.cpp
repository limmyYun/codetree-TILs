#include <iostream>
#include <tuple>
#include <vector>
#include <stack>

using namespace std;

int L, N, Q;
int map[41][41];
int damage[31];
bool dead[31];
int ans[31];

vector<tuple<int, int>> cmd;
vector<tuple<int, int>> kh[31];
int khmap[41][41];
//int khmap[41][41];

int dx[4] = { 0, 1, 0, -1 };
int dy[4] = { -1, 0, 1, 0 };

vector<int> check(int who, int dir) {
	vector<int> re;
	bool alreadyPushed[31] = {};
	alreadyPushed[who] = true;

	for (int i = 0; i < kh[who].size(); i++) {
		//for (int d = 0; d < 4; d++) {
		//cout << "here who : " <<who<<" dir : "<<dir<<" kh[who].size() : "<< kh[who].size() << endl;
			int sx = get<0>(kh[who][i]) + dy[dir];
			int sy = get<1>(kh[who][i]) + dx[dir];
			if (sx < 0 || sy < 0 || sx >= L || sy >= L || map[sx][sy] == 2) {
				re.clear();
				re.push_back(-1);
				return re;
			}
			if (khmap[sx][sy] >= 1 && !alreadyPushed[khmap[sx][sy]-1]) {
				re.push_back(khmap[sx][sy]-1);
				//cout << "get<0>(kh[who][i]) : " << get<0>(kh[who][i]) << " get<1>(kh[who][i]) : " << get<1>(kh[who][i]) << endl;
				//cout << "sx : " << sx << " sy : " << sy << " khmap[sx][sy]-1 : " << khmap[sx][sy] - 1 << endl;
				alreadyPushed[khmap[sx][sy]-1] = true;
			}
			//cout << "pass get<0>(kh[who][i]) : " << get<0>(kh[who][i]) << " get<1>(kh[who][i]) : " << get<1>(kh[who][i]) << endl;
			//cout << "pass sx : " << sx << " sy : " << sy << " khmap[sx][sy]-1 : " << khmap[sx][sy] - 1 << endl;
		//}	
	}
	return re;
}

void move(int who, int dir) {
	int tmpmap[41][41] = {};
	
	for (int i = 0; i < kh[who].size(); i++) {
		int sx = get<0>(kh[who][i]) + dy[dir];
		int sy = get<1>(kh[who][i]) + dx[dir];
		
		tmpmap[sx][sy] = who+1;
		get<0>(kh[who][i]) = sx;
		get<1>(kh[who][i]) = sy;
	}

	for (int i = 0; i < L; i++) {
		for (int j = 0; j < L; j++) {
			if (khmap[i][j] == who + 1) {
				//cout << " i :"<<i << " j : " << j << endl;
				khmap[i][j] = 0;
			}
		}
	}
	for (int i = 0; i < L; i++) {
		for (int j = 0; j < L; j++) {
			if (tmpmap[i][j]==who+1) khmap[i][j] = tmpmap[i][j];
		}
	}
}

int main() {

	cin >> L >> N >> Q;

	for (int i = 0; i < L; i++) {
		for (int j = 0; j < L; j++) {
			cin >> map[i][j];
		}
	}

	int r = 0, c = 0, h = 0, w = 0, k = 0;
	for (int i = 0; i < N; i++) {
		cin >> r >> c >> h >> w >> k;
		
		khmap[r - 1][c - 1] = i+1;
		//kh[i].push_back(make_tuple(r - 1, c - 1));
		//cout << "here" << endl;

		for (int j = 0; j < h; j++) {
			for (int t = 0; t < w; t++) {
				khmap[r - 1 + j][c - 1 + t] = i+1;
				kh[i].push_back(make_tuple(r - 1 + j, c - 1 + t));
			}
		}
		damage[i] = k;
	}

	int who = 0, m = 0;
	for (int i = 0; i < Q; i++) {
		cin >> who >> m;
		cmd.push_back(make_tuple(who - 1, m));
	}


	int cmdw = 0, cmdd = 0;
	for (int i = 0; i < Q; i++) {
		// 현재 명령 저장
		cmdw = get<0>(cmd[i]);
		cmdd = get<1>(cmd[i]);
		//cout << endl;
		if (dead[cmdw]) continue;

		// 움직여야할 기사들 번호 넣기
		stack<int> stk;
		vector<int> v;
		vector<int> nv;
		bool ignoreCmd = false;
		bool alreadyInStack[31] = {};

		stk.push(cmdw);
		alreadyInStack[cmdw] = true;
		v.push_back(cmdw);
		while (true) {
			vector<int> tmpv;
			for (int j = 0; j < v.size(); j++) {
				tmpv = check(v[j], cmdd);
				
				if ((tmpv.size()!=0) && (tmpv[0] == -1)) {
					ignoreCmd = true;
					break;
				}
				for (int p = 0; p < tmpv.size(); p++) {
					if (alreadyInStack[tmpv[p]]) continue;
					nv.push_back(tmpv[p]);
					stk.push(tmpv[p]);
					alreadyInStack[tmpv[p]] = true;
				}
			}

			if (nv.size() == 0) break;
			v.clear();
			v = nv;
			nv.clear();
		}

		if (ignoreCmd) continue;

		// stack 에서 한개씩 빼서 기사들 움직이기
		int moving = 0;
		while (!stk.empty()) {
			moving = stk.top();
			stk.pop();

			move(moving, cmdd);
		}

		// 점수 계산
		for (int f = 0; f < L; f++) {
			for (int g = 0; g < L; g++) {
				if (map[f][g] != 1) continue;
				if ((khmap[f][g] == cmdw+1) || khmap[f][g] == 0) continue;
				if (!alreadyInStack[khmap[f][g] - 1]) continue;
				damage[khmap[f][g] - 1]--;
				ans[khmap[f][g] - 1]++;
				//cout << "damage hert khmap[f][g] - 1 : " << khmap[f][g] - 1 <<"ans[khmap[f][g] - 1] : "<< ans[khmap[f][g] - 1]<< endl;
				if (damage[khmap[f][g] - 1] <= 0) {
					dead[khmap[f][g] - 1] = true;

					int size = kh[khmap[f][g] - 1].size();
					for (int b = 0; b < size; b++) {
						//cout << "get<0>(kh[khmap[f][g] - 1][b]) : " << get<0>(kh[khmap[f][g] - 1][b]) << " get<1>(kh[khmap[f][g] - 1][b]) : " << get<1>(kh[khmap[f][g] - 1][b]) << endl;
						khmap[get<0>(kh[khmap[f][g] - 1][b])][get<1>(kh[khmap[f][g] - 1][b])] = 0;
					}
				}
			}
		}
	}

	int answer = 0;
	for (int i = 0; i < Q; i++) {
		if (dead[i]) continue;
		//cout << "i : " << i << " ans[i] : " << ans[i] << endl;
		answer += ans[i];
	}

	cout << answer;
	return 0;
}