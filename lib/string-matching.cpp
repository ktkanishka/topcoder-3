#include <iostream>
#include <vector>
#include <cassert>

using namespace std;

// build string match transition table `SMT[][]` for character set ∑={'a'-'z'}
// in CLRS 32.3
//
// input: P (|P|<=1024)
// output: SMT
//
// SMT[i][j] = length (or next matching index) of longest prefix of P for suffix of P[0..i]+('a'+j)
// SMT[i][j] = max { k : P[0..<k] ⊐ P[0..i]+('a'+j) }
//
// O(|P|^3*|∑|) time
int SMT[1024][26];
void buildSMT(string P) {
  memset(SMT, 0, sizeof SMT);
  int L=P.size();
  for(int i=0; i<=L; ++i) {
    for(int j=0; j<26; ++j) {
      char c = 'a'+j;
      if (i<L&&c==P[i]) {
        // matched
        SMT[i][j]=i+1;
      } else {
        // unmatched
        string suf = P.substr(0,i)+c;
        int SL=(int)suf.size();
        // find longest prefix P[0..<k]
        for(int k=1; k<=i; ++k) if(P.substr(0,k)==suf.substr(SL-k)) SMT[i][j]=k;
      }
    }
  }
}

// Build prefix function 𝞹 `pi` of pattern string P for KMP string matching
//  - COMPUTE-PREFIX-FUNCTION(P) in CLRS 32.4
//  - Note that this 𝞹 is slightly different from `computeLongestPrefixSuffix()` due to 0-based array index
//
// input: P
// output: 𝞹[|P|]
//
//  𝝅[q] = next matching index of P when P[q] unmatched
//  𝝅[q] = max { k : k<=q AND P[0..<k] ⊐ P[0..<q] }
//
// Θ(|P|) time
vector<int> buildPrefixFunction(const string P) {
  int L = P.size();
  vector<int> pi(L+1, 0);
  
  if (L==0) return pi;
  
  int j=0;
  for(int i=2; i<L; ++i) {
    j = pi[i-1];
    while(true) {
      if (P[j]==P[i-1]) {
        // matched
        pi[i] = j+1; break;
      }
      // unmatced below
      
      // reached to head
      if (j==0) {
        pi[i] = 0; break;
      }
      j = pi[j];
    }
  }
  
  return pi;
}

// Compute length of longest prefix of P for suffix of P[0..q]
//
// input: P
// output: 𝝅[|P|]
//
//  𝝅[q] = max { k : k<q AND P[0..<k] ⊐ P[0..q] }
//
// Θ(|P|) time
vector<int> computeLongestPrefixSuffix(const string P) {
  int L = P.size();
  vector<int> pi(L, 0);
  
  if (L==0) return pi;
  int j=0;
  for(int i=1; i<L; ++i) {
    if (P[i]==P[j]) {
      // matched
      pi[i] = pi[i-1]+1;
      j++;
    } else {
      // unmatched
      j = pi[i-1];
      while(j>0 && P[i]!=P[j]) {
        j = pi[j-1];
      }
      if (P[i]==P[j]) j++;
      pi[i] = j;
    }
  }
  
  return pi;
}

// KMP algorithm - returns index of T where P begins
int doKMP(string s, string p) {
  int i = 0, j = 0, L = s.size(), M = p.size();
  
  vector<int> pi = buildPrefixFunction(p);
  while (i<L) {
    if (s[i]==p[j]) {
      i++; j++;
      if (j==M) return i-M;
    } else if (j > 0) {
      j = pi[j];
    } else {
      i++;
    }
  }
  
  return -1;
}

int main(int argc, char const *argv[]) {
  // CLRS Ex 32.4-1
  string P1="ababbabbabbababbabb";
  auto pi1_a = computeLongestPrefixSuffix(P1);
  vector<int> pi1_e = { 0,0,1,2,0,1,2,0,1,2,0,1,2,3,4,5,6,7,8 };
  for(int i=0; i<pi1_e.size(); ++i) {
    assert(pi1_a[i]==pi1_e[i]);
  }
  auto pi2_a = buildPrefixFunction(P1);
  vector<int> pi2_e = { 0,0,0,1,2,0,1,2,0,1,2,0,1,2,3,4,5,6,7,0 };
  for(int i=0; i<pi2_e.size(); ++i) {
    assert(pi2_a[i]==pi2_e[i]);
  }

  // CLRS Figure 32.7
  string P = "ababaca";
  int idx = doKMP("abababacaba", P);
  assert(idx==2);
  
  // CLRS Figure 32.7
  buildSMT(P);
  vector<vector<int>> expected = {
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {1,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {1,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {1,4,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {1,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  };
  for(int i=0; i<expected.size(); ++i) {
    for(int j=0; j<26; ++j) {
      assert(SMT[i][j]==expected[i][j]);
    }
  }
}
