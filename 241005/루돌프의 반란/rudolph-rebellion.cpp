#include <iostream>
#include <tuple>
#include <vector>

using namespace std;

int N, M, P, C, D;

int map[51][51];

vector < tuple<int, int>> s;
int score[31];
int sleep[31];
bool out[31];
int lp;
int ruR;
int ruC;

int dx8[8] = { -1, -1, 0, 1, 1, 1, 0, -1 };
int dy8[8] = { 0,-1,-1,-1,0,1,1,1 };
int dx4[4] = { 0, 1, 0, -1 };
int dy4[4] = { -1, 0, 1, 0 };

void dfs(int r, int c, int mr, int mc, int direction, int flag) {
	//cout << "r : " << r << " c : " << c << " mr : " << mr << " mc : " << mc << " direction : " << direction << endl;
	if (mr < 0 || mc < 0 || mr >= N || mc >= N) {
		out[map[r][c]-1] = true;
		lp--;
		return;
	}

	if ((map[mr][mc] != 0) && !((r==mr) && (c==mc))) {
		int movr = 0;
		int movc = 0;
		if (flag == 0) {
			movr = mr + dy8[direction];
			movc = mc + dx8[direction];
			dfs(mr, mc, movr, movc, direction, flag);
		}
		else {
			movr = mr + dy4[direction];
			movc = mc + dx4[direction];
			dfs(mr, mc, movr, movc, direction, flag);
		}
	}

	int idx = map[r][c]-1;
	//cout << "before update : idx : " << idx << endl;
	map[r][c] = 0;
	map[mr][mc] = idx + 1;
	get<0>(s[idx]) = mr;
	get<1>(s[idx]) = mc;
	//cout << "update : map[mr][mc] : " << map[mr][mc] << " get<0>(s[idx]) : " << get<0>(s[idx]) << " get<1>(s[idx]) : " << get<1>(s[idx]) << endl;
}

void crash(int sr, int sc, int sani, int di, int amount, int flag) {
	score[sani] += amount;
	
	int movesr = sr + amount * dy4[di];
	int movesc = sc + amount * dx4[di];
	if (flag == 0) {
		movesr = sr + amount * dy8[di];
		movesc = sc + amount * dx8[di];
	}

	//cout << " crash amount : "<<amount<<" d : "<<di << " sr : " << sr << " sc : " << sc << " movesr : " << movesr << " movesc : " << movesc << endl;

	if (movesr < 0 || movesc < 0 || movesr >= N || movesc >= N) {
		out[sani] = true;
		lp--;
		return;
	}
	//get<0>(s[sani]) = movesr;
	//get<1>(s[sani]) = movesc;
	//map[sr][sc] = 0;
	//map[movesr][movesc] = sani;

	// 상호 작용 확인
	//cout << "sangho sr : " << sr << " sc : " << sc << " movesr : " << movesr << " movesc : " << movesc << endl;
	dfs(get<0>(s[sani]), get<1>(s[sani]), movesr, movesc, di, flag);
}

void moveRu() {
	int nearsr = 0, nearsc = 0, nearsd = 10000, nearsi = 0;
	// 가장 가까운 산타 찾기
	int d = 0;
	for (int i = 0; i < P; i++) {
		if (out[i]) continue;
		d = (get<0>(s[i]) - ruR) * (get<0>(s[i]) - ruR) + (get<1>(s[i]) - ruC) * (get<1>(s[i]) - ruC);
		if (d < nearsd) {
			nearsr = get<0>(s[i]);
			nearsc = get<1>(s[i]);
			nearsd = d;
			nearsi = i;
		}
		else if (d == nearsd) {
			if (nearsr < get<0>(s[i])) {
				nearsr = get<0>(s[i]);
				nearsc = get<1>(s[i]);
				nearsd = d;
				nearsi = i;
			}
			else if (nearsr == get<0>(s[i])) {
				if (nearsc < get<1>(s[i])) {
					nearsr = get<0>(s[i]);
					nearsc = get<1>(s[i]);
					nearsd = d;
					nearsi = i;
				}
			}
		}
	}

	// 가까운 산타에게 이동
	int nextRuR = 0, nextRuC = 0, nextRuD = 5000, ruDir = 0;
	int tmpD = 0;
	for (int dir = 0; dir < 8; dir++) {
		int sx = ruR + dy8[dir];
		int sy = ruC + dx8[dir];
		if (sx < 0 || sy < 0 || sx >= N || sy >= N) continue;
		tmpD = (nearsr - sx) * (nearsr - sx) + (nearsc - sy) * (nearsc - sy);
		if (nextRuD > tmpD) {
			nextRuR = sx;
			nextRuC = sy;
			nextRuD = tmpD;
			ruDir = dir;
		}
	}

	// 산타와 충돌한다면 산타 날리고 점수 얻고
	if ((nextRuR == nearsr) && (nextRuC == nearsc)) {
		sleep[nearsi] = 2;
		crash(nearsr, nearsc, nearsi, ruDir, C, 0);
		map[nextRuR][nextRuC] = -1;
		map[ruR][ruC] = 0;
	}
	else {
		map[ruR][ruC] = 0;
		map[nextRuR][nextRuC] = -1;
	}
	ruR = nextRuR;
	ruC = nextRuC;
}

void moveSan() {
	int curd = 0;
	int tmpd = 0;
	int newr = 0, newc = 0, newd = 10000, newdirection = 0;
	bool isUpdate = false;
	for (int i = 0; i < P; i++) {
		if (out[i]) continue;
		if (sleep[i] >= 1) {
			sleep[i]--;
			continue;
		}

		curd = (get<0>(s[i]) - ruR) * (get<0>(s[i]) - ruR) + (get<1>(s[i]) - ruC) * (get<1>(s[i]) - ruC);

		newr = 0, newc = 0, newd = 10000, newdirection=0, isUpdate = false;
		
		for (int d = 0; d < 4; d++) {
			int sx = get<0>(s[i]) + dy4[d];
			int sy = get<1>(s[i]) + dx4[d];
			if (sx < 0 || sy < 0 || sx >= N || sy >= N) continue;
			if (map[sx][sy] > 0) continue;
			tmpd = (sx - ruR) * (sx - ruR) + (sy - ruC) * (sy - ruC);
			if (tmpd >= curd) continue;

			if (newd > tmpd) {
				newr = sx;
				newc = sy;
				newd = tmpd;
				newdirection = d;
				isUpdate = true;
				//cout << "sx : " << sx << " sy : " << sy << endl;
			}
		}

		if (!isUpdate) continue;
		
		// 산타가 옮겨져야할 위치 찾았음
		// 루돌프랑 충돌하는지 확인 후 map 업데이트
		if ((newr == ruR) && (newc == ruC)) {
			//cout << "i : " << i << " newr : " << newr << " newc : " << newc << endl;
			sleep[i] = 1;
			crash(newr, newc, i, (newdirection+2)%4, D, 1);
			//map[get<0>(s[i])][get<1>(s[i])] = 0;
		}
		else {
			map[newr][newc] = i+1;
			map[get<0>(s[i])][get<1>(s[i])] = 0;
			get<0>(s[i]) = newr;
			get<1>(s[i]) = newc;
			//cout << "i : " << i << " newr : " << newr << " newc : " << newc << endl;
		}
//cout << endl;
//for (int e = 0; e < N; e++) {
//	for (int c = 0; c < N; c++) {
//		cout << map[e][c] << " ";
//	}
//	cout << endl;
//}
	}
}

int main() {
	cin >> N >> M >> P >> C >> D;
	cin >> ruR >> ruC;
	ruR--;
	ruC--;
	map[ruR][ruC] = -1;

	int tmpn=0, tmpr = 0, tmpc = 0;
	for (int i = 0; i < P; i++) {
		s.push_back(make_tuple(0, 0));
	}
	for (int i = 0; i < P; i++) {
		cin >>tmpn>> tmpr >> tmpc;
		s[tmpn - 1] = make_tuple(tmpr - 1, tmpc - 1);
		//s.push_back(make_tuple(tmpr - 1, tmpc - 1));
		map[tmpr - 1][tmpc - 1] = tmpn;
	}
	lp = P;

	for (int i = 0; i < M; i++) {
		//cout << "i : " << i << endl;
		if (lp <= 0) break;
		// 루돌프 움직임
		moveRu();

		// 산타 움직임
		moveSan();

		for (int sa = 0; sa < P; sa++) {
			if (out[sa]) continue;
			score[sa]++;
		}
	}

	for(int i=0;i<P;i++){
		cout << score[i] << " ";
	}
	return 0;
}