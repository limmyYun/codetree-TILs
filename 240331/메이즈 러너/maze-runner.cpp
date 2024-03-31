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
        cout<<"p.size() = "<< p.size()<<endl;
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
                continue;
            }
            
            int currentL = abs(r-outr) + abs(c-outc);
            int newL = abs(sx-outr) + abs(sy-outc);
            if (currentL<=newL) continue;

            if(map[r][c] >= 11) map[r][c]--;
            else if(map[r][c] == 10) map[r][c] = 0;

            map[sx][sy]++;
            moveLen++;
            p[i] = make_tuple(sx, sy);
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
    int startR=outr-x, startC=outc-x;
    bool pass = false;
    for(int i=startR; i<startR+x; i++) {
        for(int j=startC; j<startC+x; j++) {
            if(i<0 || j<0 || i>=N || j>=N) continue;
            pass = check(i, j, x);
            //cout<<"check i : "<<i<<" j : "<<j<<" x : "<<x<< "pass : "<<pass<<endl;
            if(pass) return make_tuple(i,j);
        }
    }
}

void rotate(int r, int c, int len) {
    int tmp[11][11];

    for(int i=r; i<r+len; i++){
        for(int j=c; j<c+len; j++) {
            tmp[j][len-1-i] = map[i][j];
            if(map[i][j] >= 10) {
                for(auto person : p) {
                    int p=0, q=0;
                    tie(p,q) = person;
                    if((p==i) && (q==j)) {
                        person = make_tuple(j,len-1-i);
                    }
                }
            }
        }
    }

    for(int i=r; i<r+len; i++){
        for(int j=c; j<c+len; j++) {
            map[i][j] = tmp[i][j];
            if(map[i][j] == -1) {
                outr = i;
                outc = j;
            } else if((map[i][j]>0) && (map[i][j]<=9)) map[i][j]--;
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

    for (int p=0; p<K; p++) {
        // 참가자 이동 - 이동 횟수 계산
        cout<<" p : "<<p<<endl;
        move();

        // 모든 참가자 탈출시 종료
        if(leftP <= 0) break;

        // 출구와 참가자 포함한 가장 작은 정사각형 구하기
        // 출구와 exit 사이의 거리 중 가장 짧은 거리 구해서 변의 길이 구하기
        int shortest = 0;
        int nowx = 0, nowy = 0;
        for(int i=0;i<N;i++) {
            for(int j=0;j<N;j++) {
                if(map[i][j] <10) continue;
                nowx = abs(outr-i);
                nowy = abs(outc-j);
                shortest = max({shortest, nowx, nowy});
            }
        }
        int r=0, c=0;
        shortest++;
        tie(r,c) = getSquare(shortest);

        cout<<"r : "<<r<<" c : "<<c<<" shortest : "<<shortest<<endl;

        // 정사각형 회전
        rotate(r, c, shortest);
    }

    cout<<leftP;
    cout<<outr<<" "<<outc;

    return 0;
}