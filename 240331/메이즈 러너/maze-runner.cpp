#include <iostream>
#include <tuple>
#include <cstdlib>
#include <vector>
#include <algorithm>

using namespace std;

int N, M, K;

int map[11][11];
int outr = 0, outc = 0;
int moveLen = 0;
int leftP = 0;
// 현재 r, 현재 c
vector<tuple<int, int>> p;

int dx[4] = {0, 0, -1, 1};
int dy[4] = {-1, 1, 0, 0};

void move() {
    int r=0, c=0;

    for(int i=0; i<p.size(); i++) {
        tie(r,c) = p[i];
        
        for (int d=0; d<4; d++) {
            int sx = r + dy[d];
            int sy = c + dx[d];
            if(sx<0 || sy<0 || sx>=N || sy>=N) continue;
            if((map[sx][sy] > 0) && (map[sx][sy]<=9)) continue;

            if (map[sx][sy] == -1) {
                // 탈출
                moveLen++;
                leftP--;
                if(map[r][c] >= 11) map[r][c]--;
                else if(map[r][c] == 10) map[r][c] = 0;

                p.erase(p.begin()+i);
                i--;
                break;
            }
            
            int currentL = abs(r-outr) + abs(c-outc);
            int newL = abs(sx-outr) + abs(sy-outc);
            if (currentL<=newL) continue;

            if(map[r][c] >= 11) map[r][c]--;
            else if(map[r][c] == 10) map[r][c] = 0;

            if(map[sx][sy]>=10) map[sx][sy]++;
            else if(map[sx][sy]==0) map[sx][sy] = 10;
            moveLen++;
            get<0>(p[i]) = sx;
            get<1>(p[i]) = sy;
            break;
        }
    }
}

bool check(int r, int c, int len) {
    for(int i=r; i<r+len; i++) {
        for(int j=c; j<c+len; j++) {
            if(i<0 || j<0 || i>=N || j>=N) continue;
            
            if(map[i][j] >= 10) return true;
        }
    }

    return false;
}

tuple<int, int> getSquare(int x) {
    int startR=outr-x+1, startC=outc-x+1;
    bool pass = false;
    for(int i=0; i<x; i++) {
        for(int j=0; j<x; j++) {
            if((i+startR)<0 || (j+startC)<0 || (i+startR)>=N || (j+startC)>=N) continue;
            pass = check(i+startR, j+startC, x);
            if(pass) return make_tuple(i+startR,j+startC);
        }
    }
}

void rotate(int r, int c, int len) {
    int tmp[11][11];
    vector<tuple<int, int, int, int>> changeP;

    for(int i=0; i<len; i++){
        for(int j=0; j<len; j++) {
            tmp[r+j][c+len-1-i] = map[r+i][c+j];
            if(map[r+i][c+j] >= 10) {
                changeP.push_back(make_tuple(r+i, c+j, r+j, c+len-1-i));
            }
        }
    }

    for(int i=0; i<len; i++){
        for(int j=0; j<len; j++) {
            map[r+i][c+j] = tmp[r+i][c+j];
            if(map[r+i][c+j] == -1) {
                outr = r+i;
                outc = c+j;
            } else if((map[r+i][c+j]>0) && (map[r+i][c+j]<=9)) map[r+i][c+j]--;
        }
    }
    for(int person = 0; person<p.size(); person++) {
        int pr=0, pc=0;
        tie(pr, pc) = p[person];
        for(int newPerson = 0; newPerson<changeP.size(); newPerson++) {
            int br=0, bc=0, ar=0, ac=0;
            tie(br, bc, ar, ac) = changeP[newPerson];
            if((br==pr) && (bc==pc)) {
                get<0>(p[person]) = ar;
                get<1>(p[person]) = ac;
            }
        }
    }
}

int main() {
    cin >> N >> M >> K;

    for (int i = 0; i<N; i++) {
        for (int j=0; j<N;j++) {
            cin >> map[i][j];
        }
    }

    for (int i=0;i<M;i++) {
        int r = 0, c = 0;
        cin>>r>>c;
        map[r-1][c-1] = 10;
        p.push_back(make_tuple(r-1, c-1));
    }

    cin>>outr>>outc;
    outr--, outc--;
    map[outr][outc] = -1;

    leftP = M;

    for (int t=0; t<K; t++) {
        // 참가자 이동 - 이동 횟수 계산
        //cout<<" t : "<<t<<" moveLen : "<<moveLen<<endl;
// for(int i=0;i<p.size();i++){
//     int a=0, b=0;
//     tie(a,b) = p[i];
//     cout<<"a : "<<a<<" b : "<<b<<endl;
// }
// for(int i=0;i<N;i++) {
//     for(int j=0;j<N;j++) {
//         cout<<map[i][j]<<" ";
//     }
//     cout<<endl;
// }
// cout<<endl;
        move();

        // 모든 참가자 탈출시 종료
        if(leftP <= 0) break;

        // 출구와 참가자 포함한 가장 작은 정사각형 구하기
        // 출구와 exit 사이의 거리 중 가장 짧은 거리 구해서 변의 길이 구하기
        int shortest = 20;
        int nowx = 0, nowy = 0, nowF=0;
        for(int i=0;i<N;i++) {
            for(int j=0;j<N;j++) {
                if(map[i][j] <10) continue;
                nowx = abs(outr-i);
                nowy = abs(outc-j);
                nowF = max(nowx, nowy);
                shortest = min(shortest, nowF);
            }
        }
        int r=0, c=0;
        shortest++;
        tie(r,c) = getSquare(shortest);
        // 정사각형 회전
        rotate(r, c, shortest);
    }

    cout<<moveLen<<endl;
    cout<<outr+1<<" "<<outc+1;

    return 0;
}