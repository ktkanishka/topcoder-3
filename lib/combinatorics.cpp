#include <iostream>
#include <cassert>
#include <algorithm>
#include <vector>
using namespace std;

typedef long long LL;
constexpr int MOD = 1e9+7;

/*
 
 Power mod by repeated squaring, O(lg b)
 
 powmod(a,b) = (a^b) % MOD
 
 Reference:
  - CLRS 31.6 powers of an element
  - Ant boo 2-6 how to solve math problem
 
 */
LL powmod(LL a, LL b) {
  assert(0<=a && a<MOD);
  assert(0<=b);

  LL res=1;
  for(LL mask=1; mask<=b; mask<<=1) {
    if(b&mask) res*=a, res%=MOD;
    a*=a; a%=MOD;
  }
  return res;
}

/*
 
 Compute modular multiplicative inverse, O(lg a)
 
 - compute a^-1 (mod p) p is prime AND p ⧷ a
 - proved by Fermat's little theorem
 
 Fermat's little theorem:
 
 a^p     ≡ a (mod p)       if p is prime
 a^(p-1) ≡ 1 (mod p)       if p is prime AND p ⧷ a
 a^-1    ≡ a^(p-2) (mod p) if p is prime AND p ⧷ a

 References:
  - https://en.wikipedia.org/wiki/Modular_multiplicative_inverse#Computation
  - https://en.wikipedia.org/wiki/Fermat%27s_little_theorem
 
 */
LL modinv(LL a, LL p=MOD) {
  return powmod(a, p-2);
}

/*
 
 factorial(n) % MOD, O(n)
 
 - lazily computed and cached
 - O(1) time if cached
 
 */
LL factmod(LL n) {
  static vector<LL> memo(1,1);
  if(memo.size()<=n) {
    LL l=memo.size();
    memo.resize(n+1);
    for(LL i=l; i<=n; ++i) memo[i]=memo[i-1]*i, memo[i]%=MOD;
  }
  return memo[n];
}

/*
 
 n chooses k (% MOD) (binomial coefficient), O(n)
 
 C(n,k) = n!/((n-k)!*k!)
 
  - compute on-demand by mod inverse and factorial mod
  - deprecated because it's not efficient for large n
   - use O(k) version
 
 */
LL choose_deprecated(LL n, LL k) {
  if (n<k) return 0;
  k = min(n-k,k);
  LL res=factmod(n);
  res *= modinv(factmod(n-k)), res%=MOD;
  res *= modinv(factmod(  k)), res%=MOD;
  
  return res;
}

/*
 
 n chooses k (% MOD) (binomial coefficient), O(k)
 
   C(n,k)
 = ∏ {(n-i): i=0..k } * 1/k!
 = ∏ {(n-i): i=0..k } * modinv(k!)
 
 - dependency to modinv() and factmod()
 - compute k! first and get its mod inverse
 
 */
LL choose_factmod(LL n, LL k) {
  if(n<k) return 0;
  k = min(n-k,k);
  LL res=modinv(factmod(k));
  for(int i=0; i<k; ++i) res*=(n-i)%MOD,res%=MOD;
  return res;
}

/*
 
 n chooses k (% MOD) (binomial coefficient), O(k) in cache warmup
 
   C(n,k)
 = ∏ {(n-i): i=0..k } * 1/k!
 = ∏ {(n-i): i=0..k } * ∏ { modinv(i): i=1..k }
 
 - Pre-compute factorial of mod inverse of k (<=MAXK) first in O(k)
 
 Mod inverse is pre-computed based on below formula:

     i*(MOD/i)+MOD%i = MOD
 <=> inv(i) ≡ (inv(MOD%a) * -(MOD/i)) (mod MOD)
 
 References:
  - https://www37.atwiki.jp/uwicoder/pages/2118.html
  - https://discuss.codechef.com/questions/3869/best-known-algos-for-calculating-ncr-m
  - https://comeoncodeon.wordpress.com/2011/10/09/modular-multiplicative-inverse/
   - compute mod multiplicative inverse in linear time used in this function
 
 */
LL choose(LL n, LL k) {
  if(n<k) return 0;
  k = min(n-k,k);
  
  const int MAXK = 200;
  assert(k<=MAXK);
  static LL inv[MAXK+1],invfact[MAXK+1];
  
  if(inv[1]==0) {
    inv[1]=1;
    for(int i=2;i<=MAXK;i++) inv[i]=inv[MOD%i]*(MOD-MOD/i)%MOD;
    invfact[0]=1;
    for(int i=1;i<=MAXK;i++) invfact[i]=invfact[i-1]*inv[i]%MOD;
  }
  LL res=invfact[k];
  for(int i=0;i<k;++i) res*=(n-i)%MOD,res%=MOD;
  return res;
}

/*
 
 Create n chooses k (% MOD) table (k=0..n), O(N^2) time
 
 C(n,k) = C(n-1,k)+C(n-1,k-1)
 
 - Precompute n chooses k for n={1..N},k={1..N}
 - DP following Pascal's triangle
  - https://en.wikipedia.org/wiki/Pascal%27s_triangle
 
 */
void choose(LL N, vector<vector<int>> &C) {
  for(int i=0; i<=N; ++i) {
    C[i][0]=1;
    for(int j=1; j<=i; ++j) {
      // i chooses j
      C[i][j] = C[i-1][j]+C[i-1][j-1], C[i][j]%=MOD;
    }
  }
}

/*
 
 Multichoose k of n elements (% MOD), O(n+k) time

 H(n,k) = C(n+k-1,k) % MOD
 
 - Compute on-demand by binomial coefficient
 - Populate k slots by infinite n elements with repetion
 
 */
LL multichoose(LL n, LL k) {
  if(n==0&&k==0) return 1;
  return choose(n+k-1,k);
}

/*
 
 IEP (inclusion-exclusion principle) for union set when family of sets is homogeneous
 
 - O(N*T(f)) time to compute
 - O(N^2) to initialize combinatorics table
 
 Compute cardinality of union set A[0]∪A[1]∪..∪A[N] when cardinality of intersection of `k` sets can be computed by f(k) (family of sets is homogeneous)
 
   |∪ { A[i] : i=0..N-1 }|
 = ∑ { (-1)^|X| * |∩ { A[i] : i∈X }| : X ⊆ {0..N-1} } // IEP
 = ∑ { (-1)^|k+1| * C(N,k) * f(k) : k = 1..N }
     where f(k) = |∩ { A[i] : i∈X, X⊆{0..N-1}, |X|=k }|
 
 With homogeneous property, cardinality of intersection of C(N,k) families of sets is computed in the last step.
 
 References:
  - https://en.wikipedia.org/wiki/Inclusion%E2%80%93exclusion_principle#A_special_case
  - Ant book 4-1 complex math (basic form)
  - https://qiita.com/urutom/items/f192350c74394ae56e19
 
 Usage:
  // implement f(k)
  IEP iep(N);
  LL res=iep.solve();
 
 */
#define MAX_N 300
struct IEP {
public:
  IEP(int N, long long MOD=1000000007): N(N), MOD(MOD) {
    for(int i=0; i<=N; ++i) {
      C[i][0]=1;
      for(int j=1; j<=i; ++j) {
        C[i][j] = C[i-1][j]+C[i-1][j-1], C[i][j]%=MOD;
      }
    }
  }
  long long solve() {
    long long res=0;
    for(int k=1; k<=N; ++k) {
      long long sign=k%2==0?-1:1;
      long long x=sign;
      x*=C[N][k],x%=MOD;
      x*=f(k),x%=MOD;
      x+=MOD,x%=MOD;
      res+=x,res%=MOD;
    }
    return res;
  }
  
  // ⚠️ Implement this function
  // Compute f(k) = |∩ { A[i] : i∈X, X⊆{0..N-1}, |X|=k }|
  // Return size of intersection of `k` sets out of A[i], i=0..N-1
  long long f(int k) {
    // https://qiita.com/urutom/items/f192350c74394ae56e19
    // example to compute number of derangement
    // https://en.wikipedia.org/wiki/Derangement
    // https://oeis.org/A000166/list
    // f(k) = (N-k)!
    long long res=1;
    for(int n=1; n<=N-k; ++n) res*=n,res%=MOD;
    return res;
  }
private:
  int N;
  long long MOD;
  long long C[MAX_N][MAX_N];
};

/*
 
 IEP (inclusion-exclusion principle) in COMPLEMENTARY form when family of sets is homogeneous
 
 - O(N*T(f)) time to compute
 - O(N^2) to initialize combinatorics table
 
 https://en.wikipedia.org/wiki/Inclusion%E2%80%93exclusion_principle#Statement
 > In applications it is common to see the principle expressed in its complementary form.
 
 Compute cardinality of intersection set Ā[0]∩Ā[1]∩..∩Ā[N] when family of sets is homogeneous.
 
   |∩ { Ā[i] : i=1..N }|
 = |Ū { A[i] : i=0..N-1 }|                            // De Morgan's laws
 = |U| - |∪ { A[i] : i=0..N-1 }|                      // U is universal set
 = ∑ { (-1)^|X| * |∩ { A[i] : i∈X }| : X ⊆ {0..N-1} } // IEP
 = ∑ { (-1)^|k| * C(N,k) * f(k) : k = 0..N }
     where f(k) = |∩ { A[i] : i∈X, X⊆{0..N-1}, |X|=k }|
 
 Note that universal set corresponds to k=0 case.
 Also parity of exponent of `-1` is different.
 
 References:
 - https://en.wikipedia.org/wiki/Inclusion%E2%80%93exclusion_principle#A_special_case
 - https://yukicoder.me/wiki/algorithm_summary
 - https://qiita.com/urutom/items/f192350c74394ae56e19
 
 Used problems:
  - https://github.com/k-ori/topcoder/blob/master/NoRepeatPlaylist/NoRepeatPlaylist.cpp#L152
 
 Usage:
  // implement f(k)
  IEP_comp iep(N);
  LL res=iep.solve();
 
 */
#define MAX_N 300
struct IEP_comp {
public:
  IEP_comp(int N, long long MOD=1000000007): N(N), MOD(MOD) {
    for(int i=0; i<=N; ++i) {
      C[i][0]=1;
      for(int j=1; j<=i; ++j) {
        C[i][j] = C[i-1][j]+C[i-1][j-1], C[i][j]%=MOD;
      }
    }
  }
  long long solve() {
    long long res=0;
    for(int k=0; k<=N; ++k) {
      long long sign=k%2==0?1:-1;
      long long x=sign;
      x*=C[N][k],x%=MOD;
      x*=f(k),x%=MOD;
      x+=MOD,x%=MOD;
      res+=x,res%=MOD;
    }
    return res;
  }
  
  // ⚠️ Implement this function
  // Compute f(k) = |∩ { A[i] : i∈X, X⊆{0..N-1}, |X|=k }|
  // Return size of intersection of `k` sets out of A[i], i=0..N-1
  long long f(int k) {
    // https://qiita.com/urutom/items/f192350c74394ae56e19
    // example to compute number of derangement
    // https://en.wikipedia.org/wiki/Derangement
    // https://oeis.org/A000166/list
    // f(k) = (N-k)!
    long long res=1;
    for(int n=1; n<=N-k; ++n) res*=n,res%=MOD;
    return res;
  }
private:
  int N;
  long long MOD;
  long long C[MAX_N][MAX_N];
};

// main

int main(int argc, char const *argv[]) {
  int m2 = powmod(7,560);
  assert(m2==108725231);
  
  LL m = 560;
  LL mi = modinv(m);
  assert((m*mi)%MOD==1);
  
  assert(factmod(560)==597965522);
  assert(choose(771,50)==275127687);
  assert(multichoose(771,50)==954694855);
  
  vector<vector<int>> dp(201,vector<int>(201,0));
  choose(200,dp);
  assert(dp[200][23]==choose(200,23));
  
  int N=20;
  long long dear20 = 895014631192902121LL%MOD;
  long long factN=1;
  for(int n=1; n<=N; ++n) factN*=n,factN%=MOD;
  IEP ie(N);
  long long dearrangement1=(factN-ie.solve()+MOD)%MOD;
  assert(dearrangement1==dear20);
  IEP_comp ie2(N);
  assert(ie2.solve()==dear20);
}
