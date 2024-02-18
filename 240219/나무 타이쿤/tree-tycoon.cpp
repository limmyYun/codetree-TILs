#include <iostream>
#include <vector>
#include <tuple>
#include <queue>
#include <memory.h>

using namespace std;

typedef tuple<int, int> cell;
int map[16][16];
bool medicine[16][16];
// 행, 열
queue<cell> libro;
int n;
int m;
int dx[8] = {0,-1,-1,-1,0,1,1,1};
int dy[8] = {1,1,0,-1,-1,-1,0,1};

void newInsert(){
    for (int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            if(medicine[i][j]) continue;
            if(map[i][j] >= 2) {
                map[i][j] -= 2;
                libro.push(make_pair(i,j));
            }
        }
    }
}

void insert(){
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            int plus = 0;
            if(medicine[i][j]){
                for(int d=0;d<4;d++){
                    int sx = i + dx[2*d+1];
                    int sy = j + dy[2*d+1];
                    if(sx<0 || sx>=n || sy<0 || sy>=n) continue;
                    if(map[sx][sy]<1) continue;

                    plus++;
                }
                map[i][j] += plus;
            }
        }
    }
}

void move(int d, int p){
    int newr=0;
    int newc=0;
    while(!libro.empty()){
        cell c = libro.front();
        libro.pop();

        newr = dx[d] * p + get<0>(c);
        newc = dy[d] * p + get<1>(c);
        if (newr >= n) newr -= n;
        if (newc >= n) newc -= n;
        if (newr < 0) newr += n;
        if (newc < 0) newc += n;

        medicine[newr][newc] = true;
        map[newr][newc] += 1;
    }
}

int main() {
    cin>>n>>m;

    for (int i=0;i<n;i++){
        for (int j=0;j<n;j++){
            cin>>map[i][j];
        }
    }
    
    int dir, p=0;
    libro.push(make_pair(n-1, 0));
    libro.push(make_pair(n-2, 0));
    libro.push(make_pair(n-1, 1));
    libro.push(make_pair(n-2, 1));
    for(int i=0;i<m;i++){
        memset(medicine, false, sizeof(medicine));
        cin>>dir>>p;

        // 규칙에 따라 영양제 이동 후 medicine 맵에 표시
        move(dir-1, p);
// for(int x=0;x<n;x++){
//     for(int y=0;y<n;y++){
//         cout<<map[x][y]<<" ";
//     }
//     cout<<endl;
// }
        // 영양제 투입
        insert();

        // 새로운 특수 영양제 투입
        newInsert();
    }

    // 리브로 수의 총합 구하기
    int ans=0;
    for (int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            ans += map[i][j];
        }
    }

    cout<<ans;
    return 0;
}