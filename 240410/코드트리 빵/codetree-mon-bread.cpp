#include <iostream>
#include <queue>
#include <vector>
#include <tuple>
#include <algorithm>

using namespace std;

int N, M;

bool blocked[16][16];
// 편의점 위치 정보 (r, c)
vector<tuple<int, int>> con;
// 사람들 현재 위치와 도착 여부
vector<tuple<int, int>> p;
// 남은 사람
int lp;
// 사람의 도착 여부
bool arrived[31];
int map[16][16];

int dx[4] = { 0, -1, 1, 0 };
int dy[4] = { -1, 0, 0, 1 };

void movePerson(int pi, int conr, int conc, int perr, int perc) {
    bool visited[16][16] = {};
    int way[16][16] = {};
    int wayTmp = 0;
    queue<tuple<int, int>> que;
    que.push(make_tuple(perr, perc));
    visited[perr][perc] = true;

    int curr = 0, curc = 0;
    bool found = false;
    while (true) {
        tie(curr, curc) = que.front();
        que.pop();

        for (int d = 0; d < 4; d++) {
            int sx = curr + dy[d];
            int sy = curc + dx[d];
            if (sx < 0 || sy < 0 || sx >= N || sy >= N) continue;
            if (visited[sx][sy]) continue;
            if (blocked[sx][sy]) continue;

            if (d == 0) wayTmp = 3;
            else if (d == 1) wayTmp = 2;
            else if (d == 2) wayTmp = 1;
            else if (d == 3) wayTmp = 0;
            way[sx][sy] = wayTmp;
            visited[sx][sy] = true;
            que.push(make_tuple(sx, sy));

            if (sx == conr && sy == conc) {
                found = true;
                break;
            }
        }
        if (found) break;
    }

    curr = conr;
    curc = conc;
    wayTmp = way[conr][conc];
    int movr = 0, movc = 0;
    while (true) {
        movr = curr + dy[way[curr][curc]];
        movc = curc + dx[way[curr][curc]];

        if (movr == perr && movc == perc) {
            //cout << "here pi : " << pi << endl;
            get<0>(p[pi]) = curr;
            get<1>(p[pi]) = curc;
            //cout << "here 2 pi : " << pi << endl;
            break;
        }
        curr = movr;
        curc = movc;
    }
}

void move(int inNum) {
    int pr = 0, pc = 0;
    int cr = 0, cc = 0;
    //cout <<"inNum : "<< inNum << endl;
    if (inNum + 1 >= M) inNum = M-1;
    //cout << "inNum 2 : " << inNum << endl;
    for (int i = 0; i < inNum+1; i++) {
        //cout << "why i : " << i << endl;
        if (arrived[i]) continue;
        tie(pr, pc) = p[i];
        tie(cr, cc) = con[i];
        //cout << "i : " << i << " pr : " << pr << " pc : " << pc << " cr : " << cr << " cc : " << cc<<endl;
        movePerson(i, cr,cc, pr, pc);
    }
    //cout << "here";
}

void check() {
    int pr = 0, pc = 0;
    int cr = 0, cc = 0;
    for (int i = 0; i < M; i++) {
        if (arrived[i]) continue;
        tie(pr, pc) = p[i];
        tie(cr, cc) = con[i];
        if (pr == cr && pc == cc) {
            arrived[i] = true;
            lp--;
            blocked[cr][cc] = true;
        }
    }
}

int getDistance(int startp, int mr, int mc) {
    queue<tuple<int, int, int>> que;
    bool visited[16][16] = {};
    que.push(make_tuple(0, mr, mc));

    int curd=0, curr = 0, curc = 0;
    int conr = get<0>(con[startp]);
    int conc = get<1>(con[startp]);
    while (true) {
        tie(curd, curr, curc) = que.front();
        que.pop();

        for (int d = 0; d < 4; d++) {
            int sx = curr + dy[d];
            int sy = curc + dx[d];
            if (sx < 0 || sy < 0 || sx >= N || sy >= N) continue;
            if (visited[sx][sy]) continue;
            if (blocked[sx][sy]) continue;

            que.push(make_tuple(curd + 1, sx, sy));
            visited[sx][sy] = true;

            if (sx == conr && sy == conc) {
                return curd + 1;
            }
        }
    }
}

bool comp(tuple<int, int, int> a, tuple<int, int, int> b) {
    return a > b;
}

void startPerson(int sp) {
    vector<tuple<int, int, int>> av;

    int tmpd = 0;
    int mind = 100;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (map[i][j] == 0) continue;
            if (blocked[i][j]) continue;
            tmpd = getDistance(sp, i, j);
            //cout << "distance" <<tmpd<< endl;
            if (tmpd == mind) {
                av.push_back(make_tuple(tmpd, -i, -j));
                //cout << "push" << endl;
            }
            else if (tmpd < mind) {
                av.clear();
                av.push_back(make_tuple(tmpd, -i, -j));
                mind = tmpd;
                //cout << "pop and push" << endl;
            }
        }
    }

    sort(av.begin(), av.end(), comp);

    get<0>(p[sp]) = -get<1>(av[0]);
    get<1>(p[sp]) = -get<2>(av[0]);
    blocked[-get<1>(av[0])][-get<2>(av[0])] = true;
}

int main() {
    cin >> N >> M;
    lp = M;

    int tmp = 0;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            cin >> tmp;
            if (tmp == 1) map[i][j] = 1;
        }
    }
    int tmpR = 0, tmpC = 0;
    for (int i = 0; i < M; i++) {
        cin >> tmpR >> tmpC;
        con.push_back(make_tuple(tmpR - 1, tmpC - 1));
        p.push_back(make_tuple(-1, -1));
    }

    int t = 0;
    while (1) {
        //cout << "t : " << t << endl;
        // 현재 map에 존재하는 사람 위치 옮기기
        move(t-1);
        //cout << "t : " << t << endl;

        // 옮겨진 사람들이 편의점 도착했는지 확인
        check();
        //cout << "t : " << t << endl;

        // t번째 사람이 있다면 베이스 캠프로 옮겨주기
        if (t + 1 <= p.size()) {
            startPerson(t);
        }
        //cout << "t : " << t << endl;
        if (lp <= 0) break;
        t++;
    }
    cout << t+1;
    return 0;
}