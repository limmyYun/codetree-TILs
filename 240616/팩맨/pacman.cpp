#include <iostream>
#include <queue>
#include <vector>
#include <tuple>
#include <string.h>

using namespace std;

int mdx[8] = { 0, -1, -1, -1, 0, 1, 1, 1};
int mdy[8] = {-1, -1, 0, 1, 1, 1, 0, -1};
int pdx[4] = { 0, -1, 0, 1};
int pdy[4] = {-1, 0, 1, 0};


// 마리 수, 턴 수
int m, t;
// 팩맨 위치
int pr, pc;
// 몬스터 배열
queue<int> monster[5][5];
// 몬스터 시체 배열
int died[5][5];
// 몬스터 알 배열
queue<int> babyMonster[5][5];

void printMonsterSize(){
    cout<<"printMonsterSize"<<endl;
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            cout<<monster[i][j].size()<<" ";
        }
        cout<<endl;
    }
    cout<<endl;
}

void printdied(){
    cout<<"printdied"<<endl;
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            cout<<died[i][j]<<" ";
        }
        cout<<endl;
    }
    cout<<endl;
}

void makeBabyMoster(){
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            babyMonster[i][j] = monster[i][j];
        }
    }
}

void moveMonsters(){
    queue<int> newMonster[5][5];
    int queSize = 0;
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            queSize = monster[i][j].size();

            for(int q = 0; q<queSize; q++) {
                bool isOut = false;
                int monsterDir = monster[i][j].front();
                monster[i][j].pop();

                for(int d0=monsterDir; d0<monsterDir+8; d0++){
                    if(isOut) break;

                    int d = d0%8;
                    int sr = i + mdy[d];
                    int sc = j + mdx[d];
                    if(sr<0 || sc<0 || sr>=4 || sc>=4) continue;
                    if((sr==pr)&&(sc==pc)) continue;
                    if(died[sr][sc] != 0) continue;

                    newMonster[sr][sc].push(d);
                    isOut = true;
                }
                if(!isOut){
                    newMonster[i][j].push(monsterDir);
                }
            }
        }
    }
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            monster[i][j] = newMonster[i][j];
        }
    }
}

int tmpKill(int fm, int sm, int tm) {
    int num = 0;
    vector<int> d;
    d.push_back(fm);
    d.push_back(sm);
    d.push_back(tm);

    int gr = pr;
    int gc = pc;
    bool visited[4][4];
    memset(visited, false, sizeof(visited));
    //visited[gr][gc] = true;

    for(int dir = 0; dir<d.size(); dir++) {
        gr = gr + pdy[d[dir]];
        gc = gc + pdx[d[dir]];
        if(gr<0 || gc<0 || gr>=4 || gc>=4 || visited[gr][gc]) {
            num = 0;
            break;
        }
        visited[gr][gc] = true;
        num += monster[gr][gc].size();
    }
    return num;
}

tuple<int, int, int> findBestWay() {
    tuple<int, int, int> best;

    int numDied = 0;
    int bestDied = 0;
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            for(int k=0; k<4; k++){
                numDied = tmpKill(i,j,k);

                if(numDied>bestDied) {
                    best = make_tuple(i,j,k);
                    bestDied = numDied;
                }
            }
        }
    }
    //cout<<"bestDied : "<<bestDied<<endl;
    return best;
}

void moveMan() {
    int f = 0, s = 0, th = 0;
    vector<int> way;
    tie(f, s, th) = findBestWay();
    way.push_back(f);
    way.push_back(s);
    way.push_back(th);

    //cout<<f<<" "<<s<<" "<<th<<endl;

    for(int i=0;i<3;i++){
        int newPr = pr + pdy[way[i]];
        int newPc = pc + pdx[way[i]];

        // 몬스터 que에서 빼기
        // 몬스터 시체에 추가하기
        int size = monster[newPr][newPc].size();
        if(size>0) {
            while(!monster[newPr][newPc].empty()) monster[newPr][newPc].pop();
            died[newPr][newPc] = 3;
        }
        pr = newPr;
        pc = newPc;
    }
}

void clean() {
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            if(died[i][j] != 0) died[i][j]--;
        }
    }
}

void newPush() {
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            if(babyMonster[i][j].size() == 0) continue;

            int size = babyMonster[i][j].size();
            for(int b = 0; b<size; b++) {
                int dir = babyMonster[i][j].front();
                babyMonster[i][j].pop();

                monster[i][j].push(dir);
            }
        }
    }
}

int main() {
    cin>>m>>t;
    cin>>pr>>pc;
    pr--, pc--;

    int mr = 0, mc = 0, md = 0;
    for(int i = 0;i<m;i++){
        cin>>mr>>mc>>md;
        mr--, mc--, md--;
        monster[mr][mc].push(md);
    }

    //printMonsterSize();

    for(int ct = 0; ct<t; ct++){
        //cout<<"turn : "<<ct+1<<endl;
        // 1. 몬스터 복제
        makeBabyMoster();

        //printMonsterSize();
        
        // 2. 몬스터 이동
        moveMonsters();

        //printMonsterSize();
        //printdied();

        // 3. 팩맨의 이동
        moveMan();

        //printMonsterSize();

        // 4. 시체 소멸
        clean();
        
        // 5. 몬스터 복제
        newPush();
    }

    int result = 0;
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            result += monster[i][j].size();
        }
    }
    cout<<result<<endl;
    return 0;
}