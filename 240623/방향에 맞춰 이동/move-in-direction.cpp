#include <iostream>

using namespace std;

// 0= 오른쪽, 1= 아래, 2=왼쪽, 3=위
int dx[4] = {1, 0, -1, 0};
int dy[4] = {0, 1, 0, -1};

int main() {
    int N = 0;
    cin>>N;

    string str = "";
    int num = 0;
    int dir = 0;
    int cx = 0, cy = 0;


    for(int i = 0; i<N;i++){
        cin>>str>>num;

        if(str == "N") {
            dir = 3;
        } else if(str=="E") {
            dir = 0;
        } else if(str == "S") {
            dir = 1;
        } else if(str=="W") {
            dir = 2;
        }

        int sx = cx + dy[dir]*num;
        int sy = cy + dx[dir]*num;

        cx = sx;
        cy = sy;
    }

    cout<<cy<<" "<<-cx;
    return 0;
}