#include <iostream>
#include <tuple>
#include <vector>

using namespace std;

int n, m, t;
// 무게, 번호, 방향
vector<tuple<int, int, int>> map[51][51];
vector<tuple<int, int, int>> next_map[51][51];

int dx[4] = {0,-1,0,1};
int dy[4] = {-1,0,1,0};

void Simulate() {
    // 구슬 next map에 옮기기
    int x=0, y=0, d=0;

    for(int i=0;i<n;i++) {
        for(int j=0;j<n;j++){
            next_map[i][j] = {};
        }
    }

    for(int i=0;i<n;i++) {
        for(int j=0;j<n;j++) {
            if(map[i][j].size() == 0) continue;
            tie(x,y,d) = map[i][j][0];
            
            int sx = i + dx[d];
            int sy = j + dy[d];
            if(sx<0 || sy<0 || sx>=n || sy>= n) {
                d = (d+2)%4;
                tuple<int, int, int> t = make_tuple(x, y, d);
                next_map[i][j].push_back(t);
            } else {
                next_map[sx][sy].push_back(make_tuple(x, y, d));
                //cout<<"n "<<sx<<" "<<sy<<" "<<x<< " "<<y<<" "<<d<<" size : "<<next_map[sx][sy].size()<<endl;
            }
        }
    }

    // next map에 있는거 부딫히는 거 계산
    for (int i=0;i<n;i++) {
        for(int j=0;j<n;j++) {
            map[i][j] = {};
            if(next_map[i][j].size() <= 1) {
                map[i][j] = next_map[i][j];
                //cout<<"i : "<<i<<" j :"<<j<<" size : "<<next_map[i][j].size()<<endl;
                continue;
            }

            int bestW = 0;
            int bestD = 0;
            int bestN = 0;
            //tuple<int, int, int> t = make_tuple(0,0,0);
            int WWW = 0;
            int a=0, b=0, c=0;
            for(int v = 0; v<next_map[i][j].size(); v++) {
                tie(a,b,c) = next_map[i][j][v];
                bestW += a;
                if(b > WWW) {
                    bestD = c;
                    bestN = b;
                    WWW = bestN;
                }
            }
            map[i][j].push_back(make_tuple(bestW, bestN, bestD));
        }
    }

}

int main() {
    cin>>n>>m>>t;

    int r=0, c=0, w=0, d=0;
    char dir;
    for (int i=0;i<m;i++) {
        cin>>r>>c>>dir>>w;
        if(dir == 'U') d = 1;
        else if(dir == 'L') d = 0;
        else if(dir == 'R') d = 2;
        else if(dir == 'D') d = 3;
        map[r-1][c-1].push_back(make_tuple(w,i+1, d));
    }

    for(int i=0;i<t;i++) Simulate();

    int number = 0;
    int bestW = 0;
    int tmp=0;
    int q =0 , g =0;
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            if(map[i][j].size() == 0) continue;
            number++;
            tie(tmp,q,g) = map[i][j][0];
            if(bestW < tmp) bestW = tmp;

            //cout<<"r : "<<i<<" c : "<<j<<" w : "<<tmp<<" num : "<< q<<endl;
        }
    }

    cout<<number<<" "<<bestW;


    return 0;
}