#include <fstream> // for greed plugin
#include <iostream>
#include <algorithm> // max,min
#include <vector>
#include <string>
#include <sstream>
#include <set>
#include <map>
#include <iostream>
#include <utility>
#include <cctype>
#include <queue>
#include <stack>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <unordered_set>
#include <unordered_map>
#include <limits.h>
#include <cstring>
#include <tuple>
#include <cassert>
#include <numeric>
using namespace std;

// type alias
typedef long long LL;

typedef vector < int > VI;
typedef unordered_map < int, int > MAPII;
typedef unordered_set < int > SETI;

// repetition
#define FOR(i,a,b) for(int i=(a);i<(b);++i)
#define REP(i,n)  for(int i=0;i<(n);++i)
#define FORR(x,arr) for(auto& x:arr)
#define SZ(a) int((a).size())

// debug cerr
#define dump(x)  cerr << #x << " = " << (x) << endl;
#define dump2(x,y)  cerr << #x << " = " << (x) << ", " << #y << " = " << (y) << endl;
#define dump3(x,y,z)  cerr << #x << " = " << (x) << ", " << #y << " = " << (y) << ", " << #z << " = " << (z) << endl;
#define dump4(x,y,z,a)  cerr << #x << " = " << (x) << ", " << #y << " = " << (y) << ", " << #z << " = " << (z) << ", " << #a << " = " << (a) << endl;
#define dumpAR(ar) FORR(x,(ar)) { cout << x << ','; } cerr << endl;

/**
 
 7/21/2017
 
 8:38-9:05 passed system tests (147.91 pt)

 - spent much time to find a mistake about indexing and vertex
 - should take care of problem size more. then simple brute force is easy to start and finish shortly.
 
 My original solution is a bit tricky. Brute force directly seems to be more straightforward
 - https://community.topcoder.com/stat?c=problem_solution&rm=328948&rd=16748&pm=14324&cr=22744380
 - https://community.topcoder.com/stat?c=problem_solution&rm=328950&rd=16748&pm=14324&cr=23331631
 - https://community.topcoder.com/stat?c=problem_solution&rm=328956&rd=16748&pm=14324&cr=40443867
 
 19:35-19:41 add three nested loop solution
 
 */

class TriangleEasy {
public:
  int E[51][51]={};
  int find(int N, vector<int> x, vector<int> y) {
    REP(i,SZ(x)) E[x[i]][y[i]]=E[y[i]][x[i]]=1;
    
    int maxe=0;
    REP(i,N) FOR(j,i+1,N) FOR(k,j+1,N) {
      int sum = E[i][j]+E[j][k]+E[k][i];
      maxe = max(maxe,sum);
    }
    
    return 3-maxe;
  }
};

// O(V^3) time
class TriangleEasy_org {
  public:
  VI E[51];
  int find(int N, vector<int> x, vector<int> y) {
    REP(i,51) E[i].clear();
    REP(i,SZ(x)) E[x[i]].push_back(y[i]), E[y[i]].push_back(x[i]);
    
    int maxe=0;
    REP(u,N) {
      VI es=E[u];
      maxe = max(maxe,min(2,SZ(es)));
      REP(j,SZ(es)) REP(i,j) {
        int v=es[i],w=es[j];
        if(std::find(E[v].begin(),E[v].end(),w)!=E[v].end())  maxe=3;
      }
    }
    return 3-maxe;
  }
};

// CUT begin
ifstream data("TriangleEasy.sample");

string next_line() {
    string s;
    getline(data, s);
    return s;
}

template <typename T> void from_stream(T &t) {
    stringstream ss(next_line());
    ss >> t;
}

void from_stream(string &s) {
    s = next_line();
}

template <typename T> void from_stream(vector<T> &ts) {
    int len;
    from_stream(len);
    ts.clear();
    for (int i = 0; i < len; ++i) {
        T t;
        from_stream(t);
        ts.push_back(t);
    }
}

template <typename T>
string to_string(T t) {
    stringstream s;
    s << t;
    return s.str();
}

string to_string(string t) {
    return "\"" + t + "\"";
}

bool do_test(int n, vector<int> x, vector<int> y, int __expected) {
    time_t startClock = clock();
    TriangleEasy *instance = new TriangleEasy();
    int __result = instance->find(n, x, y);
    double elapsed = (double)(clock() - startClock) / CLOCKS_PER_SEC;
    delete instance;

    if (__result == __expected) {
        cout << "PASSED!" << " (" << elapsed << " seconds)" << endl;
        return true;
    }
    else {
        cout << "FAILED!" << " (" << elapsed << " seconds)" << endl;
        cout << "           Expected: " << to_string(__expected) << endl;
        cout << "           Received: " << to_string(__result) << endl;
        return false;
    }
}

int run_test(bool mainProcess, const set<int> &case_set, const string command) {
    int cases = 0, passed = 0;
    while (true) {
        if (next_line().find("--") != 0)
            break;
        int n;
        from_stream(n);
        vector<int> x;
        from_stream(x);
        vector<int> y;
        from_stream(y);
        next_line();
        int __answer;
        from_stream(__answer);

        cases++;
        if (case_set.size() > 0 && case_set.find(cases - 1) == case_set.end())
            continue;

        cout << "  Testcase #" << cases - 1 << " ... ";
        if ( do_test(n, x, y, __answer)) {
            passed++;
        }
    }
    if (mainProcess) {
        cout << endl << "Passed : " << passed << "/" << cases << " cases" << endl;
        int T = time(NULL) - 1500651472;
        double PT = T / 60.0, TT = 75.0;
        cout << "Time   : " << T / 60 << " minutes " << T % 60 << " secs" << endl;
        cout << "Score  : " << 250 * (0.3 + (0.7 * TT * TT) / (10.0 * PT * PT + TT * TT)) << " points" << endl;
    }
    return 0;
}

int main(int argc, char *argv[]) {
    cout.setf(ios::fixed, ios::floatfield);
    cout.precision(2);
    set<int> cases;
    bool mainProcess = true;
    for (int i = 1; i < argc; ++i) {
        if ( string(argv[i]) == "-") {
            mainProcess = false;
        } else {
            cases.insert(atoi(argv[i]));
        }
    }
    if (mainProcess) {
        cout << "TriangleEasy (250 Points)" << endl << endl;
    }
    return run_test(mainProcess, cases, argv[0]);
}
// CUT end
