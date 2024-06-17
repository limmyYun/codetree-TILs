#include <iostream>

using namespace std;

int n, m;

int dx[4] = {0, 1, 0, -1};
int dy[4] = {-1, 0, 1, 0};

bool map[101][101];

bool check(int r, int c) {
    int num = 0;
    for(int d=0; d<4; d++){
        int sr = r+dy[d];
        int sc = c+dx[d];
        if(sr<0 || sc<0 || sr>=n || sc>=n) continue;
        if(!map[sr][sc]) continue;
        num++;
    }

    if(num==3) return true;
    else return false;
}

int main() {
    cin>>n>>m;

    int r = 0, c = 0;
    bool isGood = false;
    for(int i = 0; i<m; i++) {
        cin>>r>>c;
        r--, c--;
        isGood = check(r,c);
        map[r][c] = true;
        cout<<isGood<<endl;
    }
    return 0;
}