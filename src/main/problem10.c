/*
 
 Problem

 Ashish has forgotten his password. 
 He remembers that he used the following algorithm to create his password: 
 Ashish took up to k consecutive words from a passage of text, 
 and took the first letter from each word. 
 Then, he might have changed some of the letters to their "l33tspeak" equivalents. 
 Specifically, he might have changed 
 "o" to "0", 
 "i" to "1", 
 "e" to "3", 
 "a" to "4", 
 "s" to "5", 
 "t" to "7",
 "b" to "8" 
 and/or 
 "g" to "9".

 For example, if Ashish took his password 
 from the first sentence of 
 The Fellowship of the Ring -- 

 "This book is largely concerned with Hobbits, 
  and from its pages a reader may discover much
  of their character and a little of their history" 
  
  -- 
  
  Ashish would have reduced that to "tbilcwhafiparmdmotcaaloth". 
  Then the password might be "tbilcwh", "7b1lcwh4f", "a", "4", or "4al07h", etc.

  Ashish has a special extension installed in his browser 
  that will prevent his computer from uploading any string that contains his password. 
  In order to figure out which passage of text he took his password from,
  Ashish has created a webpage to take advantage of this extension. 
  Every second, the webpage will tell the browser to post a "password string" 
  for a new passage of text: 
  a string that contains all of the possible passwords 
  that Ashish could have chosen from that passage of text. 
  As soon as his browser fails to post such a string, 
  Ashish will know where he took his password from.

  For example, if k = 2 and the passage of text contains words 
  starting with the letters "google", 
  then one password string for that passage is "goo0og00gle9o909l3". 
  All substrings of length ≤ 2 from the original string, 
  and all of their l33tspeak equivalents, are contained in the new string.

   Given the first letters of the words in a passage of text, 
   what is the minimum number of characters in the "password string" of that passage?

   Input

   The first line of the input gives the number of test cases, T. 
   T test cases follow. Each test case consists of two lines. 
   The first line contains the integer k. 
   The second line contains a string S, 
   representing the first letters of the words in a passage of text. 
   S contains only the characters 'a' - 'z', with no spaces.

  Output

  For each test case, output one line containing "Case #x: y", 
  where x is the case number (starting from 1) and y is 
  the minimum number of characters in the password string for S.

  Limits

  1 ≤ T ≤ 20.
 
  S will contain at least 2 * k characters.
  There will exist a password string with at most 1018 characters.
 
  Small dataset

  S will contain at most 1000 characters.
  k = 2.
  Large dataset

  S will contain at most 5000 characters.
  2 ≤ k ≤ 500.
 
  Sample
 
  Input 
 
  Output 
 
  4
  2
  poppop
  2
  google
  2
  tbilcwhafiparmdmotcaaloth
  10
  tbilcwhafiparmdmotcaaloth
  Case #1: 6
  Case #2: 18
  Case #3: 53
  Case #4: 1136

  In the first sample input, one possible password string is "0ppop0".
  In the second sample input, one possible password string is "goo0og00gle9o909l3".






  This is certainly an intimidating problem. 
  Even small strings, like the example from The Fellowship of the Ring, 
  are difficult to work through. 
  
  When you are trying to combine passwords of length at most 500 
  into a password string, 
  there are so many possibilities to consider that optimization quickly becomes overwhelming.


  The Small Input

  Let's begin with the small input. 
  In this case, we need to connect up pairs of letters. 
  The key insight here is to imagine everything as a graph. 
  
  If you have heard of De Bruijn sequences before, 
  that is the perfect thing to build from. 
  
  We will make a vertex for each letter 
  (the 26 normal letters as well as all the 133t variations), 
  and add an edge between every pair of letters. 
  
  Each 2-letter word that we need in our password string 
  corresponds to an edge on this graph. 
  
  For example, "t0" corresponds to the edge 't' -> '0'. 
  Let's call these candidate edges.

  Next let's consider a password string. 
  We can think of this as a path on the graph. 
  We start at the first letter, 
  and then continually move to the next letter in the string. 
  
  For example, "abc0" corresponds 
  to the path 'a' -> 'b' -> 'c' -> '0'. 
  
  Therefore, the problem can be re-stated as follows: 
  what is the length of the shortest path on this graph 
  that includes all the candidate edges?

  Here is it is helpful to think back 
  to another classic algorithm problem: Eulerian paths. 
  
  The problem could also be re-stated as: 

  if we start with just the candidate edges, 
  what is the minimum number of edges we need to add 
  so that the resulting graph has an Eulerian path? 
  
  Fortunately for us, the Eulerian path problem has been completely solved!

  Fact: 

  A directed graph has an Eulerian path if and only if 
  
  (1) every vertex has in-degree equal 
      to its out-degree except possibly 
	  for two vertices that are off by one, and 
	  
  (2) every vertex with positive degree is connected 
      in the underlying undirected graph.

  If you play with some examples, 
  you will see that you will always be connected here, 
  but let's come back to a formal proof 
  when we talk about the large input. 
  
  The fact that connectivity comes for free 
  is really what makes this problem solvable, 
  and so it is a good thing to think about!

  The remaining condition says that all we need to do 
  is add edges to balance in-degrees and out-degrees. 
  
  We can add any edge we want at any time, so the method is simple: choose a vertex u with higher in-degree than out-degree and a vertex v with higher out-degree than in-degree, and then add an edge from u to v. Repeat until there are only two vertices left that are only slightly imbalanced, and we're done! After all that talk, we end up with a very simple greedy algorithm.


  http://samplecodebank.blogspot.com/2011/04/eulerian-path-in-c.html


  http://chessprogramming.wikispaces.com/De+Bruijn+Sequence+Generator

*/

/*  Here it is an array which contains 
    a letter and a digit 
    
    "o" to "0", 
    "i" to "1", 
    "e" to "3", 
    "a" to "4", 
    "s" to "5", 
    "t" to "7",
    "b" to "8" 
    and/or 
    "g" to "9".
 */

char images[26][3] = 
{
"a4", "b8", "c" , "d" ,
"e3", "f" , "g9", "h" ,
"i1", "j" , "k" , "l" ,
"m" , "n" , "o0", "p" ,
"q" , "r" , "s5", "t7",
"u" , "v" , "w" , "x" ,
"y" , "z" 
};


/*

  #include <algorithm>
#include <numeric>

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <queue>
#include <set>
#include <map>
#include <stack>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <cassert>

#include <cmath>
#include <complex>
using namespace std;



#define FOR(i,a,b) for(int i=(int)(a);i<=(int)(b);++i)
#define FOREACH(it,c) for(__typeof((c).begin()) it=(c).begin();it!=(c).end();++it)


char images[26][3] = {
"a4",
"b8",
"c",
"d",
"e3",
"f",
"g9",
"h",
"i1",
"j",
"k",
"l",
"m",
"n",
"o0",
"p",
"q",
"r",
"s5",
"t7",
"u",
"v",
"w",
"x",
"y",
"z" };

int main() {
    int T; cin >> T;
    FOR(t,1,T) {
        int K; cin >> K;
        string S; cin >> S;
        int N = S.size();
        set<string> edges;
        for (int n=0; n+1<N; ++n) {
            int a = S[n]-'a', b = S[n+1]-'a';
            for (int i=0; i<strlen(images[a]); ++i) {
                for (int j=0; j<strlen(images[b]); ++j) {
                    string tmp = "";
                    tmp += images[a][i];
                    tmp += images[b][j];
                    edges.insert(tmp);
                }
            }
        }
        long long edge_count = 0;
        map<char,int> deg;
        FOREACH(it,edges) {
            ++deg[(*it)[0]]; 
            --deg[(*it)[1]]; 
            ++edge_count;
        }
        int add = 0;
        FOREACH(it,deg) if (it->second > 0) add += it->second;
        if (add == 0) {
            cout << "Case #" << t << ": " << (edge_count+1) << endl;
        } else {
            cout << "Case #" << t << ": " << (edge_count+add) << endl;
        }
    }
  return 0;
}









#include <string>
#include <vector>
#include <map>
#include <set>
#include <queue>
#include <stack>
#include <cstdlib>
#include <cstring>
#include <cassert>
#include <iostream>
#include <sstream>
#include <cstddef>
#include <algorithm>
#include <utility>
#include <iterator>
#include <numeric>
#include <list>
#include <complex>
#include <cstdio>
#include <climits>
#include <fcntl.h>
#include <unistd.h>

using namespace std;

typedef vector<int> vi;
typedef vector<string> vs;
typedef long long ll;
typedef complex<double> pnt;
typedef pair<int, int> pii;

#define RA(x) (x).begin(), (x).end()
#define FE(i, x) for (typeof((x).begin()) i = (x).begin(); i != (x).end(); i++)
#define SZ(x) ((int) (x).size())

template<class T>
void splitstr(const string &s, vector<T> &out)
{
    istringstream in(s);
    out.clear();
    copy(istream_iterator<T>(in), istream_iterator<T>(), back_inserter(out));
}

template<class T> T gcd(T a, T b) { return b ? gcd(b, a % b) : a; }

static void redirect(int argc, const char **argv)
{
    if (argc > 1)
    {
        int fd = open(argv[1], O_RDONLY);
        if (fd == -1) { perror(argv[1]); exit(1); }
        if (-1 == dup2(fd, 0)) { perror(argv[1]); exit(1); }
        if (-1 == close(fd)) { perror(argv[1]); exit(1); }
    }

    if (argc > 2)
    {
        int fd = open(argv[2], O_WRONLY | O_CREAT, 0666);
        if (fd == -1) { perror(argv[2]); exit(1); }
        if (-1 == dup2(fd, 1)) { perror(argv[2]); exit(1); }
        if (-1 == close(fd)) { perror(argv[2]); exit(1); }
    }
}

static int dfs(bool *seen, const int *odeg, const vector<int> *edges, int cur)
{
    int ans = 0;
    if (seen[cur])
        return 0;
    seen[cur] = true;
    if (odeg[cur] > 0)
        ans += odeg[cur];

    for (int i = 0; i < SZ(edges[cur]); i++)
        ans += dfs(seen, odeg, edges, edges[cur][i]);
    return ans;
}

int main(int argc, const char **argv)
{
    redirect(argc, argv);

    int cases;
    cin >> cases;

    const string extra = "oieastbg";

    for (int cas = 0; cas < cases; cas++)
    {
        int K;
        string S;
        cin >> K >> S;

        const int V = 34;
        bool seen[V] = {};
        int odeg[V] = {};
        vector<int> edges[V];
        ll ans = 0;
        set<pair<int, int> > pairs;
        for (int i = 0; i < SZ(S) - 1; i++)
        {
            int codes[2][2];
            for (int j = 0; j < 2; j++)
            {
                char c = S[i + j];
                codes[j][0] = c - 'a';
                if (extra.find(c) != string::npos)
                    codes[j][1] = extra.find(c) + 26;
                else
                    codes[j][1] = -1;
            }

            for (int j = 0; j < 2; j++)
                if (codes[0][j] != -1)
                    for (int k = 0; k < 2; k++)
                        if (codes[1][k] != -1)
                        {
                            int a = codes[0][j];
                            int b = codes[1][k];
                            pair<int, int> key(a, b);
                            if (!pairs.count(key))
                            {
                                pairs.insert(key);
                                edges[a].push_back(b);
                                edges[b].push_back(a);
                                odeg[a]++;
                                odeg[b]--;
                                ans++;
                            }
                        }
        }

        for (int i = 0; i < V; i++)
        {
            if (!seen[i] && !edges[i].empty())
            {
                int E = dfs(seen, odeg, edges, i);
                if (E == 0)
                    E = 1;
                ans += E;
            }
        }

        printf("Case #%d: %lld\n", cas + 1, ans);
    }
    return 0;
}







 */