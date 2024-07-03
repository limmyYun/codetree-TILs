#include <iostream>

using namespace std;

int n;
int t;
int map[100][100];
string str;
int cr = 0, cc = 0;
int dir = 0;

int dx[4] = {0, 1, 0, -1};
int dy[4] = {-1, 0, 1, 0};

bool setDirection(char c) {
    if(c == 'L') {
        dir = (dir+3) % 4;
        return true;
    } else if(c == 'R'){
        dir = (dir+1) % 4;
        return true;
    }
    return false;
}

int main() {
    cin>>n>>t;
    cin>>str;
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            cin>>map[i][j];
        }
    }

    int ans = 0;
    cr = n/2;
    cc = n/2;
    ans += map[cr][cc];

    for(int i = 0;i<str.size();i++){
        bool changed = setDirection(str[i]);
        if(changed) continue;
        int sr = cr + dy[dir];
        int sc = cc + dx[dir];
        if(sr<0 || sc<0 || sr>=n || sc>=n) continue;
        cr = sr;
        cc = sc;
        ans += map[cr][cc];
    }

    cout<<ans;
    return 0;
}