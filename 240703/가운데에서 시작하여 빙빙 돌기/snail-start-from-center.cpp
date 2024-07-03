#include <iostream>

using namespace std;

int line = 1;
int num = 1;
int cr;
int cc;
int n;
int d = 0;
int count = 1;
int dx[4] = {1, 0, -1, 0};
int dy[4] = {0, -1, 0, 1};

int map[101][101];

int main() {
    cin>>n;

    cr = n/2;
    cc = n/2;

    int cnt = 0;

    map[cr][cc] = num;
    num++;
    cr = cr + dy[d];
    cc = cc + dx[d];

    while(true) {
        if(num > n*n) break;

        map[cr][cc] = num;
        num++;

        // 방향, 길이, update
        if(cnt+1==count){
            cnt = 0;
            line++;
            d = (d+1)%4;
        } else {
            cnt++;
        }

        if(line == 3) {
            count++;
            line = 1;
        }

        cr = cr + dy[d];
        cc = cc + dx[d];
    }

    for(int i=0;i<n;i++) {
        for(int j=0;j<n;j++){
            cout<<map[i][j]<<" ";
        }
        cout<<endl;
    }


    return 0;
}