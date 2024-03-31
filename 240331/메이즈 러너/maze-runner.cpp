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
        //cout<<"p.size() = "<< p.size()<<endl;
        tie(r,c) = p[i];
//         cout<<"r : "<<r<<" c : "<<c<<endl;
// for(int i=0;i<p.size();i++){
//     int a=0, b=0;
//     tie(a,b) = p[i];
//     cout<<"a : "<<a<<" b : "<<b<<endl;
// }
        
        for (int d=0; d<4; d++) {
            int sx = r + dy[d];
            int sy = c + dx[d];
            if(sx<0 || sy<0 || sx>=N || sy>=N) continue;
            if((map[sx][sy] > 0) && (map[sx][sy]<=9)) continue;

            if (map[sx][sy] == -1) {
                // 탈출
                //cout<<"here"<<endl;
                moveLen++;
                leftP--;
                if(map[r][c] >= 11) map[r][c]--;
                else if(map[r][c] == 10) map[r][c] = 0;

                p.erase(p.begin()+i);
                i--;
// for(int j=0;j<p.size();j++){
//     int a=0, b=0;
//     tie(a,b) = p[j];
//     cout<<"a : "<<a<<" b : "<<b<<endl;
// }
// cout<<endl;
                break;
            }
            
            int currentL = abs(r-outr) + abs(c-outc);
            int newL = abs(sx-outr) + abs(sy-outc);
            if (currentL<=newL) continue;

            if(map[r][c] >= 11) map[r][c]--;
            else if(map[r][c] == 10) map[r][c] = 0;

            if(map[sx][sy]>=11) map[sx][sy]++;
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

    for(int i=0; i<len; i++){
        for(int j=0; j<len; j++) {
            tmp[r+j][c+len-1-i] = map[r+i][c+j];
            //cout<<"array r: "<<r+i<<" c: "<<c+j<<" re r : "<<r+j<<" c : "<<c+len-1-i<<endl;
            if(map[r+i][c+j] >= 10) {
                for(int person = 0; person<p.size(); person++) {
                    int m=0, n=0;
                    tie(m,n) = p[person];
                    if((m==r+i) && (n==c+j)) {
                        get<0>(p[person]) = r+j;
                        get<1>(p[person]) = c+len-1-i;
                    }
                }
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
//         cout<<" t : "<<t<<endl;
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
// for(int i=0;i<N;i++) {
//     for(int j=0;j<N;j++) {
//         cout<<map[i][j]<<" ";
//     }
//     cout<<endl;
// }
// cout<<endl;

        // 모든 참가자 탈출시 종료
        if(leftP <= 0) break;

        // 출구와 참가자 포함한 가장 작은 정사각형 구하기
        // 출구와 exit 사이의 거리 중 가장 짧은 거리 구해서 변의 길이 구하기
// for(int i=0;i<N;i++) {
//     for(int j=0;j<N;j++) {
//         cout<<map[i][j]<<" ";
//     }
//     cout<<endl;
// }
// cout<<endl;
        int shortest = 20;
        int nowx = 0, nowy = 0, nowF;
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
        //cout<<"r : "<<r<<" c : "<<c<<" shortest : "<<shortest<<" outr : "<<outr<<" outcc : "<<outc<<endl;
        tie(r,c) = getSquare(shortest);

        //cout<<"before rotate r : "<<r<<" c : "<<c<<" shortest : "<<shortest<<endl;

        // 정사각형 회전
        rotate(r, c, shortest);
// for(int i=0;i<N;i++) {
//     for(int j=0;j<N;j++) {
//         cout<<map[i][j]<<" ";
//     }
//     cout<<endl;
// }
// cout<<endl;
    }

    cout<<moveLen<<endl;
    cout<<outr+1<<" "<<outc+1;

    return 0;
}