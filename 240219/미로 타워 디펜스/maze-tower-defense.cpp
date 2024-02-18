#include <iostream>
#include <vector>
#include <tuple>

using namespace std;

// r, c, monster
typedef tuple<int,int,int> m;
int n, s;
int d, p;
int map[26][26];

vector<m> ms;

int dx[4] = {0, 1, 0, -1};
int dy[4] = {-1, 0, 1, 0};

int ans = 0;

void makev(){
    vector<m> newv;
    int cr = n/2;
    int cc = n/2;
    int cline = 1;
    int clineNum = 0;

    int cmove = 0;
    int cdir = 0;

    while(1){
        // 움직이기
        int sr = cr + dx[cdir%4];
        int sc = cc + dy[cdir%4];
        if (sr<0 || sr>=n || sc<0 || sr>=n) break;
        if(map[sr][sc] == 0) break;

        if(map[sr][sc] != -1) newv.push_back(make_tuple(sr, sc, map[sr][sc]));
        cr = sr;
        cc = sc;
        cmove++;

        if (cmove == cline) {
            cmove = 0;
            clineNum ++;
            cdir++;
        }
        if (clineNum == 2) {
            cline ++;
            clineNum = 0;
        }
    }
    ms = newv;
}

void dirKill(){
    int cr = n/2;
    int cc = n/2;
    if(d == 0) d = 2;
    else if(d==2) d=0;

    for (int i=0;i<p;i++){
        int sr = cr + dx[d];
        int sc = cc + dy[d];
        if (sr<0 || sr>=n || sc<0 || sr>=n) break;
        if(map[sr][sc]==0) break;

        ans += map[sr][sc];
        map[sr][sc] = -1;

        // for(auto v : ms) {
        //     if(get<0>(v) == sr && get<1>(v) == sc) {
        //         get<2>(v) = -1;
        //         break;
        //     }
        // }
        cr = sr;
        cc = sc;
    }
}

bool killOverFour() {
    bool result = false;

    int cn = -2;
    int killNum = 0;
    vector<m> kill;

    for (auto v : ms) {
        if(map[get<0>(v)][get<1>(v)] == -1) continue;
        if(cn == -2) cn = get<2>(v);
        if(get<2>(v) == cn) {
            killNum++;
            kill.push_back(v);
        } else {
            if (killNum >= 4) {
                // kill
                for (auto killv : kill) {
                    map[get<0>(killv)][get<1>(killv)] = -1;
                }
                ans += killNum*cn;
                killNum = 0;
                result = true;
            }
            cn = get<2>(v);
            killNum = 1;
            kill.clear();
            kill.push_back(v);
        }
    }
    if (killNum >= 4) {
        // kill
        for (auto killv : kill) {
            map[get<0>(killv)][get<1>(killv)] = -1;
        }
        ans += killNum*cn;
        killNum = 0;
        result = true;
    }

    return result;
}

vector<m> makeNewV() {
    vector<m> newv;
    int cn = 0;
    int ca = 0;
    for(auto v : ms) {
        if (cn == 0) {
            cn = get<2>(v);
            ca = 1;
            continue;
        }

        if (cn == get<2>(v)) {
            ca++;
            continue;
        }
        // 다르면
        newv.push_back(make_tuple(0,0,ca));
        newv.push_back(make_tuple(0,0,cn));
        ca = 1;
        cn = get<2>(v);
    }
    newv.push_back(make_tuple(0,0,ca));
    newv.push_back(make_tuple(0,0,cn));

    return newv;
}

void saveNewV() {
    vector<m> newVec;
    int vsize = ms.size();
    int vIdx = 0;

    int cr = n/2;
    int cc = n/2;
    int cline = 1;
    int clineNum = 0;

    int cmove = 0;
    int cdir = 0;

    while(1){
        // 움직이기
        int sr = cr + dx[cdir%4];
        int sc = cc + dy[cdir%4];
        if (sr<0 || sr>=n || sc<0 || sr>=n) break;
        cr = sr;
        cc = sc;
        
        // if (vsize == vIdx -1) {
        //     map[sr][sc] = 0;
        //     continue;
        // }

        if (vsize-1 >= vIdx) {
            map[sr][sc] = get<2>(ms[vIdx]);
            newVec.push_back(make_tuple(sr,sc,get<2>(ms[vIdx])));
            vIdx++;
        } else {
            map[sr][sc] = 0;
        }
        cmove++;

        if (cmove == cline) {
            cmove = 0;
            clineNum ++;
            cdir++;
        }
        if (clineNum == 2) {
            cline ++;
            clineNum = 0;
        }
    }
    ms = newVec;
}

int main() {
    cin>>n>>s;

    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            cin >> map[i][j];
        }
    }

    // vector에 순서대로 채우기
    makev();
// for(auto v : ms){
//     cout<<"r : "<<get<0>(v)<<" c : "<<get<1>(v)<<" a : "<<get<2>(v);
//     cout<<endl;
// }

    for (int i=0;i<s;i++){
        cin>>d>>p;

// for(int i=0;i<n;i++){
//     for(int j=0;j<n;j++){
//         cout<<map[i][j]<<" ";
//     }
//     cout<<endl;
// }
// cout<<endl;
        // 방향대로 공격
        dirKill();
// for(int i=0;i<n;i++){
//     for(int j=0;j<n;j++){
//         cout<<map[i][j]<<" ";
//     }
//     cout<<endl;
// }
// cout<<endl;

        bool isKilled = true;
        while(isKilled) {
            isKilled = killOverFour();
// for(auto v : ms){
//     cout<<"r : "<<get<0>(v)<<" c : "<<get<1>(v)<<" a : "<<get<2>(v);
//     cout<<endl;
// }
// cout<<endl;
            makev();
        }

        // 새로운 v 창출
        ms = makeNewV();

        // 새로운 v를 map에 저장
        saveNewV();
// for(int i=0;i<n;i++){
//     for(int j=0;j<n;j++){
//         cout<<map[i][j]<<" ";
//     }
//     cout<<endl;
// }
// cout<<endl;
    }
    cout<<ans;
    return 0;
}