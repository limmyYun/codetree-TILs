#include <iostream>
#include <tuple>
#include <vector>
#include <queue>

using namespace std;

int N, M, K;
// 공격력, 최근에 공격한 턴, 행+열, 열
tuple<int, int, int, int> map[11][11];
bool used[11][11];
int way[11][11];
bool visited[11][11];

int dx[4] = { 1, 0, -1, 0 };
int dy[4] = { 0, 1, 0, -1 };

int dx2[8] = { -1, -1, 0, 1, 1, 1, 0, -1 };
int dy2[8] = { 0, -1, -1, -1, 0, 1, 1, 1 };

int startD = 0, startR = 0, startC = 0;
int endD = 0, endR = 0, endC = 0;

void chooseTop() {
    tuple<int, int, int, int> top = make_tuple(-5001, 0, 0, 0);
    int tmpD, tmpRe, tmpRPC, tmpC = 0;
    tuple<int, int, int, int> current;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            tie(tmpD, tmpRe, tmpRPC, tmpC) = map[i][j];
            if (tmpD == 0) continue;
            
            current = make_tuple(-tmpD, tmpRe, tmpRPC, tmpC);
            if (top < current) {
                top = current;
                startD = tmpD;
                startR = i;
                startC = j;
            }
        }
    }
}

void chooseKillTop() {
    tuple<int, int, int, int> top = make_tuple(0, -1001, -22, -11);
    int tmpD, tmpRe, tmpRPC, tmpC = 0;
    tuple<int, int, int, int> current;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            tie(tmpD, tmpRe, tmpRPC, tmpC) = map[i][j];
            if (tmpD == 0) continue;

            current = make_tuple(tmpD, -tmpRe, -tmpRPC, -tmpC);
            if (top < current) {
                top = current;
                endD = tmpD;
                endR = i;
                endC = j;
            }
        }
    }
}

void upTop() {
    startD += N+M;
    get<0>(map[startR][startC]) = startD;
}

void init() {
    for (int i = 0; i < 11; i++) {
        for (int j = 0; j < 11; j++) {
            used[i][j] = false;
            visited[i][j] = false;
            way[i][j] = 1;
        }
    }
}

bool rAttack() {
    queue < tuple<int, int>> que;
    que.push(make_tuple(startR, startC));
    visited[startR][startC] = true;
    used[startR][startC] = true;

    bool foundWay = false;

    int r = 0, c = 0, m = 0;
    while (!que.empty()) {
        tie(r, c) = que.front();
        que.pop();

        for (int d = 0; d < 4; d++) {
            int sx = (r + dy[d]+4)%4;
            int sy = (c + dx[d]+4)%4;

            if (visited[sx][sy]) continue;
            if (get<0>(map[sx][sy]) == 0) continue;

            way[sx][sy] = (d+2)%4;
            visited[sx][sy] = true;
            que.push(make_tuple(sx, sy));

            if (sx == endR && sy == endC) {
                foundWay = true;
                break;
            }
        }
        if (foundWay) break;
    }

    if (foundWay) {
        int cr = endR, cc = endC, cd = way[endR][endC];
        get<0>(map[endR][endC]) -= startD;
        used[endR][endC] = true;

        int wayDamage = startD / 2;
        while (true) {
            //cout<<"currentR : "<<cr<<" currentC : "<<cc<<" currentD : "<<cd<<endl;
            if (cr == startR && cc == startC) break;

            int tr = (cr + dy[cd]+4)%4;
            int tc = (cc + dx[cd]+4)%4;

            used[tr][tc] = true;
            get<0>(map[tr][tc]) -= wayDamage;
            cr = tr;
            cc = tc;
            cd = way[tr][tc];
        }
    }

    return foundWay;
}

void tAttack() {
    used[startR][startC] = true;

    int wayDamage = startD / 2;

    get<0>(map[endR][endC]) -= startD;
    used[endR][endC] = true;

    for (int d = 0; d < 8; d++) {
        int sx = (endR + dy2[d] + 8)%8;
        int sy = (endC + dx2[d]+8)%8;

        if (get<0>(map[sx][sy]) == 0) continue;
        if (sx == startR && sy == startC) continue;

        used[sx][sy] = true;
        get<0>(map[sx][sy]) -= wayDamage;
    }
}

void removeTopAndPlus() {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            if (get<0>(map[i][j]) < 0) {
                get<0>(map[i][j]) = 0;
                continue;
            }
            if (used[i][j] || get<0>(map[i][j]) == 0) continue;

            get<0>(map[i][j])++;
        }
    }
}

int main() {
    cin >> N >> M >> K;

    int tmp = 0;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            cin >> tmp;
            map[i][j] = make_tuple(tmp, 0, i+j, j);
        }
    }

    int damage = 0, recent = 0, rPc = 0, c = 0;
    bool rOK = false;
    for (int k = 0; k < K; k++) {
        // 공격자 선정
        chooseTop();
        //cout<<"startR : "<<startR<<" startC : "<<startC<<" startD : "<<startD<<endl;

        // 공격 대상자 선정
        chooseKillTop();
        //cout<<"endR : "<<endR<<" endC : "<<endC<<endl;

        // 공격자 공격력 증가
        upTop();
        //cout<<"startR : "<<startR<<" startC : "<<startC<<" startD : "<<startD<<endl;

        // 탑 공격
        // 레이저 공격
        init();
        rOK = rAttack();

        // 포탄 공격
        if (!rOK) tAttack();
// for(int i=0;i<N;i++){
//     for(int j=0;j<M;j++){
//         cout<< get<0>(map[i][j])<<" ";
//     }
//     cout<<endl;
// }

        // 무너진 포탑 정리 + 포탑 정비
        removeTopAndPlus();

        // 최근에 공격한 턴 증가
        get<1>(map[startR][startC]) = k+1;
    }

    chooseKillTop();

    cout << endD;
    
    return 0;
}