#include <iostream>
#include <queue>
#include <vector>
#include <tuple>
#include <string.h>

using namespace std;

int N, M, P, C, D;
int rr, rc;

int map[51][51];
int score[31];
bool out[31];
int sleep[31];
int san;
tuple<int, int> info[31];

int ruDx[8] = { 0, 1, 1, 1, 0, -1, -1, -1 };
int ruDy[8] = { -1, -1, 0, 1, 1, 1, 0, -1 };
int sanDx[4] = { 0, 1, 0, -1 };
int sanDy[4] = { -1, 0, 1, 0 };

int getDistance(int r1, int c1, int r2, int c2) {
	return (r1 - r2) * (r1 - r2) + (c1 - c2) * (c1 - c2);
}

void crash(int nowR, int nowC, int d, int targetSan) {
	// 산타 위치 업데이트(info, map), sleep 업데이트, out 업데이트
	int sanR = nowR;
	int sanC = nowC;
	int newL = 0;
	sleep[targetSan] = 1;
	bool first = true;
	while (true) {
		if (first) {
			sanR = sanR + (ruDy[d])*C;
			sanC = sanC + (ruDx[d])*C;
			first = false;
		}
		else {
			sanR = sanR + ruDy[d];
			sanC = sanC + ruDx[d];
		}

		if (sanR < 0 || sanC<0 || sanR >= N || sanC >= N) {
			out[targetSan] = true;
			break;
		}
		newL = map[sanR][sanC];
		
		if (newL == -2) {
			//map[get<0>(info[targetSan])][get<1>(info[targetSan])] = -2;
			map[sanR][sanC] = targetSan;
			get<0>(info[targetSan]) = sanR;
			get<1>(info[targetSan]) = sanC;
			break;
		}
		else {
			map[sanR][sanC] = targetSan;
			get<0>(info[targetSan]) = sanR;
			get<1>(info[targetSan]) = sanC;
			targetSan = newL;
		}
	}
}

void moveRu() {
	int tSan = 0, bestD = 10000;
	// 가장 가까운 산타 찾기
	for (int i = 0; i < P; i++) {
		if (out[i]) continue;
		int cd = getDistance(rr, rc, get<0>(info[i]), get<1>(info[i]));
		if (cd < bestD) {
			bestD = cd;
			tSan = i;
		}
		else if (cd == bestD) {
			if (get<0>(info[tSan]) < get<0>(info[i])) {
				bestD = cd;
				tSan = i;
			}
			else if (get<0>(info[tSan]) == get<0>(info[i])) {
				if (get<1>(info[tSan]) < get<1>(info[i])) {
					bestD = cd;
					tSan = i;
				}
			}
		}
	}

	// 1칸 돌진
	// 가장 가깝게 가는 위치 찾기
	int bestDir = 0, tmpD = 0;
	bestD = 10000;
	for (int d = 0; d < 8; d++) {
		int sr = rr + ruDy[d];
		int sc = rc + ruDx[d];
		if (sr < 0 || sc < 0 || sr >= N || sc >= N) continue;

		tmpD = getDistance(sr, sc, get<0>(info[tSan]), get<1>(info[tSan]));
		if (bestD > tmpD) {
			bestDir = d;
			bestD = tmpD;
		}
	}

	// 찾았으니까 그 위치에 산타가 있는지 없는지 확인
	int newR = rr + ruDy[bestDir];
	int newC = rc + ruDx[bestDir];
	if (map[newR][newC] >= 0) {
		// 산타 존재 => 충돌
		score[tSan] += C;
		crash(newR, newC, bestDir, tSan);
		map[newR][newC] = -1;
		map[rr][rc] = -2;
		rr = newR;
		rc = newC;
	}
	else {
		// 산타 존재 x => 루돌프 이동
		map[newR][newC] = -1;
		map[rr][rc] = -2;
		rr = newR;
		rc = newC;
	}
}

void moveSan(int tar) {
	int nowLen = getDistance(rr, rc, get<0>(info[tar]), get<1>(info[tar]));
	int bestD = -1, bestLen = 10000;
	for (int d = 0; d < 4; d++) {
		int sr = get<0>(info[tar]) + sanDy[d];
		int sc = get<1>(info[tar]) + sanDx[d];
		if (sr < 0 || sc < 0 || sr >= N || sc >= N) continue;
		if (map[sr][sc] >= 0) continue;

		int tmpLen = getDistance(rr, rc, sr, sc);
		if (bestLen > tmpLen) {
			bestD = d;
			bestLen = tmpLen;
		}
	}

	if ((bestD == -1) || (nowLen<=bestLen)) {
		return;
	}

	// 움직이자
	// 이동한 위치에 루돌프 있는지 확인
	int r = get<0>(info[tar]) + sanDy[bestD];
	int c = get<1>(info[tar]) + sanDx[bestD];
	
	if (map[r][c] == -1) {
		score[tar] += D;
		// crash
		int nd = (bestD + 2) % 4;
		int nr = r;
		int nc = c;
		bool f = true;
		sleep[tar] = 1;
		map[get<0>(info[tar])][get<1>(info[tar])] = -2;
		while (true) {
			if (f) {
				nr = nr + (sanDy[nd])*D;
				nc = nc + (sanDx[nd])*D;
				f = false;
			}
			else {
				nr = nr + sanDy[nd];
				nc = nc + sanDx[nd];
			}
			if (nr < 0 || nc<0 || nr>= N || nc >= N) {
				out[tar] = true;
				break;
			}
			int newL = map[nr][nc];

			if ((newL == -2) || newL == tar) {
				//map[get<0>(info[targetSan])][get<1>(info[targetSan])] = -2;
				map[nr][nc] = tar;
				get<0>(info[tar]) = nr;
				get<1>(info[tar]) = nc;
				break;
			}
			else {
				map[nr][nc] = tar;
				get<0>(info[tar]) = nr;
				get<1>(info[tar]) = nc;
				tar = newL;
			}
		}
	}
	else {
		map[r][c] = tar;
		map[get<0>(info[tar])][get<1>(info[tar])] = -2;
		get<0>(info[tar]) = r;
		get<1>(info[tar]) = c;
	}
}

int main() {
	cin >> N >> M >> P >> C >> D;
	cin >> rr >> rc;
	rr--, rc--;
	int n=0, r=0, c=0;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			map[i][j] = -2;
		}
	}
	map[rr][rc] = -1;
	san = P;
	for (int i = 0; i < P; i++) {
		cin >> n >> r >> c;
		map[r - 1][c - 1] = n-1;
		info[n - 1] = make_tuple(r - 1, c - 1);
	}

	for (int t = 0; t < M; t++) {
		if (P <= 0) break;
		for (int i = 0; i < P; i++) {
			if (sleep[i] == 1) sleep[i]++;
			else if (sleep[i] == 2) sleep[i] = 0;
		}

		// 루돌프
		moveRu();

		// 산타
		for (int i = 0; i < P; i++) {
			if (out[i]) continue;
			if (sleep[i]>=1) {
				continue;
			}

			moveSan(i);
		}
		
		// 탈락하지 않은 산타 점수 +1
		for (int i = 0; i < P; i++) {
			if (out[i]) continue;
			score[i]++;
		}
	}

	for (int i = 0; i < P; i++) {
		cout << score[i] << " ";
	}
}