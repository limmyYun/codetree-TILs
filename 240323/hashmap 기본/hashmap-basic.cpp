#include <iostream>
#include <unordered_map>

using namespace std;

unordered_map<int, int> m;

int main() {
    int t = 0;
    cin >> t;

    string s;
    for(int i=0;i<t;i++) {
        cin >> s;
        if(s == "add") {
            int n1=0, n2=0;
            cin>>n1>>n2;
            m[n1] = n2;
        } else if(s == "find") {
            int n1 = 0;
            cin>>n1;
            if(m.find(n1) != m.end()) {
                cout<<m[n1]<<endl;
            } else {
                cout<<"None"<<endl;
            }
        } else if(s=="remove") {
            int n1 = 0;
            cin>>n1;
            m.erase(n1);
        }
    }
    return 0;
}