#include <iostream>

using namespace std;

bool visited[101][101];
int n, m;
int map[101][101];
int dx[4] = {0, 1, 0, -1};
int dy[4] = {-1, 0, 1, 0};

int main() {
    cin>>n>>m;

    int num = 1;

    int r = 0, c = 0, d = 2;
    map[0][0] = num;
    visited[0][0] = true;
    num++;
    while(true) {
        if(num == n*m+1) break;
        int sx = r + dy[d];
        int sy = c + dx[d];

        if(sx<0 || sy<0 || sx>=n || sy>=m || visited[sx][sy]) {
            d = (d+3) % 4;
            continue;
        }

        map[sx][sy] = num;
        num++;
        visited[sx][sy] = true;
        r = sx;
        c = sy;
    }

    for(int i = 0; i<n; i++){
        for(int j=0;j<m;j++){
            cout<<map[i][j]<<" ";
        }
         cout<<endl;
    }
    return 0;
}