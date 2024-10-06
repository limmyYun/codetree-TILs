// vector 배열 초기화 방법
// 전역변수이면 초기화 알아서 되나? 배열?
// vector 해당 값 get?

#include <iostream>
#include <vector>
#include <tuple>

using namespace std;

int n, m, k;
vector<int> gun[20][20];
int player[20][20];

int pPoint[30];
int pGun[30];
int ans[30];
int pDir[30];

int dx[4] = { 0, 1, 0, -1 };
int dy[4] = { -1, 0, 1, 0 };


int main() {
	cin >> n >> m >> k;
	int g = 0;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			cin >> g;
			player[i][j] = -1;
			gun[i][j].push_back(g);
		}
	}

	int x = 0, y = 0, d = 0, s = 0;
	for (int i = 0; i < m; i++) {
		cin >> x >> y >> d >> s;
		pPoint[i] = s;
		pDir[i] = d;
		player[x - 1][y - 1] = i;
	}

	for (int t = 0; t < k; t++) {
		//cout<<" t : "<<t<<endl;
		for (int per = 0; per < m; per++) {
			int curR = 0, curC = 0;
			// player 0~m-1 번 찾기
			bool found = false;
			for (int i = 0; i < n; i++) {
				for (int j = 0; j < n; j++) {
					if (player[i][j] == per) {
						found = true;
						curR = i;
						curC = j;
						break;
					}
					if (found) break;
				}
			}

			// 1칸 이동
			int sr = curR + dy[pDir[per]];
			int sc = curC + dx[pDir[per]];

			if (sr < 0 || sc < 0 || sr >= n || sc >= n) {
				pDir[per] = (pDir[per] + 2) % 4;
				sr = curR + dy[pDir[per]];
				sc = curC + dx[pDir[per]];
			}

			if (player[sr][sc] < 0) {
				// player 없는 경우
				if (gun[sr][sc].size() != 0) {
					// 총 있는 경우
					int bestTg = -1, bestV = pGun[per];
					for (int tg = 0; tg < gun[sr][sc].size(); tg++) {
						if (bestV < gun[sr][sc][tg]) {
							bestTg = tg;
							bestV = gun[sr][sc][tg];
						}
					}
					if (bestTg != -1) {
						if (pGun[per] != 0) {
							gun[sr][sc].push_back(pGun[per]);
						}
						pGun[per] = gun[sr][sc][bestTg];
						gun[sr][sc].erase(gun[sr][sc].begin() + bestTg);
					}
				}
				player[sr][sc] = per;
				player[curR][curC] = -1;
			}
			else {
				// player 가 있는 경우
				// 이긴 p, 진 p 찾기
				int winP = 0, loseP = 0;
				int fightP = player[sr][sc];
				if ((pPoint[per] + pGun[per]) > (pPoint[fightP] + pGun[fightP])) {
					winP = per;
					loseP = fightP;
				}
				else if ((pPoint[per] + pGun[per]) == (pPoint[fightP] + pGun[fightP])) {
					if (pPoint[per] > pPoint[fightP]) {
						winP = per;
						loseP = fightP;
					}
					else {
						winP = fightP;
						loseP = per;
					}
				}
				else {
					winP = fightP;
					loseP = per;
				}

				player[curR][curC] = -1;
				player[sr][sc] = winP;

				// 이긴 p 합의 차이 만큼 point 획득
				ans[winP] += (pPoint[winP] + pGun[winP]) - (pPoint[loseP] + pGun[loseP]);

				// 진 p 는 총 있으면 내려놓기
				if (pGun[loseP] != 0) {
					gun[sr][sc].push_back(pGun[loseP]);
					pGun[loseP] = 0;
				}
				// 진 p는 내가 가진 방향대로 1칸 이동 + 다른 p 있으면 90씩 회전
				int newR = -1, newC = -1;
				for (int nd = pDir[loseP]; nd < pDir[loseP] + 4; nd++) {
					newR = sr + dy[nd%4];
					newC = sc + dx[nd%4];
					if (newR < 0 || newC < 0 || newR >= n || newC >= n) continue;
					if (player[newR][newC] != -1) continue;
					
					pDir[loseP] = nd % 4;
					break;
				}

				// 갈 방향 찾았으면 그 자리에 총 있는지 확인해서 총 줍기
				if ((newR != -1) && (newC != -1)) {
					player[newR][newC] = loseP;
					int bestTg = -1, bestV = pGun[loseP];
					for (int tg = 0; tg < gun[newR][newC].size(); tg++) {
						if (bestV < gun[newR][newC][tg]) {
							bestTg = tg;
							bestV = gun[newR][newC][tg];
						}
					}
					if (bestTg != -1) {
						pGun[loseP] = gun[newR][newC][bestTg];
						gun[newR][newC].erase(gun[newR][newC].begin() + bestTg);
					}
				}

				// 이긴 플레이어는 현재 자리에서 가장 공격력 높은 총 줍기
				int bestTg = -1, bestV = pGun[winP];
				for (int tg = 0; tg < gun[sr][sc].size(); tg++) {
					if (bestV < gun[sr][sc][tg]) {
						bestTg = tg;
						bestV = gun[sr][sc][tg];
					}
				}
				if (bestTg != -1) {
					if (pGun[winP] != 0) {
						gun[sr][sc].push_back(pGun[winP]);
					}
					pGun[winP] = gun[sr][sc][bestTg];
					gun[sr][sc].erase(gun[sr][sc].begin() + bestTg);
				}
			}
		}
	}

	for (int i = 0; i < m; i++) {
		cout << ans[i] << " ";
	}
}