#include <iostream>

using namespace std;

int n, m, d;
int cr = 0, cc = 0;

int dx[4] = {1, 0, -1, 0};
int dy[4] = {0, 1, 0, -1};
bool visited[101][101];
int ans[101][101];

int main() {
    cin>>n>>m;

    int cnt = 1;
    visited[cr][cc] = true;
    ans[cr][cc] = cnt;
    cnt++;

    while(true) {
        if(cnt>=n*m+1) break;
        int sx = cr + dy[d];
        int sy = cc + dx[d];
        if(sx<0 || sy<0 || sx>=n || sy>=m || visited[sx][sy]) {
            d = (d+1)%4;
            continue;
        }

        ans[sx][sy] = cnt;
        visited[sx][sy] = true;
        cr = sx;
        cc = sy;
        cnt++;
    }

    for(int i=0;i<n;i++){
        for(int j=0;j<m;j++){
            cout<<ans[i][j]<<" ";
        }
        cout<<endl;
    }
    
    return 0;
}