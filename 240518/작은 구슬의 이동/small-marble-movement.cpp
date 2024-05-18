#include <iostream>

using namespace std;

int n, t, r, c, d;
int dx[4]={-1,0,1,0};
int dy[4]={0,-1,0,1};

int main() {
    string ds;
    cin>>n>>t>>r>>c>>ds;
    r--;
    c--;
    if(ds=="U") d = 1;
    else if(ds=="D") d = 3;
    else if(ds=="R") d = 2;
    else d = 0;

    for(int i=0;i<t;i++){
        int sx = r + dy[d];
        int sy = c + dx[d];
        if(sx<0 || sy<0 || sx>=n || sy>= n) {
            d = (d+2)%4;
            continue;
        }
        r = sx;
        c = sy;
    }
    cout<<r+1<<" "<<c+1;
    return 0;
}