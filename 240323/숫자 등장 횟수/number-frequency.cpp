#include <iostream>
#include <unordered_map>

using namespace std;

unordered_map<int, int> map;

int main() {
    int m =0, n=0;
    cin>>n>>m;

    int tmp = 0;
    for(int i=0;i<n;i++) {
        cin>>tmp;
        if(map.find(tmp) != map.end()) {
            int ori = map[tmp];
            map[tmp] = ori + 1;
        } else {
            map[tmp] = 1;
        }
    }

    for(int i=0;i<m;i++) {
        cin>>tmp;
        cout<<map[tmp]<<" ";
    }
    return 0;
}