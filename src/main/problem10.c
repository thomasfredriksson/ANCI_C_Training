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

 #include <string.h>

char images[26][3] = 
{
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
"z" 
};

int Pr10GetIndex(char c)
{
   int code = c - 'a';
   int digit; 

   if (code > 0)
	   return code;

   digit = c - '0';

   if (digit == 0) return 26 + 0;
   if (digit == 3) return 26 + 1;
   if (digit == 4) return 26 + 2;
   if (digit == 5) return 26 + 3;
   if (digit == 7) return 26 + 4;
   if (digit == 8) return 26 + 5;
   if (digit == 9) return 26 + 6;

   return -1;
}

void Pr10Solve()
{
   char* S = "google";

   int K = 2;
   int N = 6;

   int n;
   int a;
   int b;
   int i;
   int j;
   int add = 0;

   char* Edges[100];
   int EdgesCount = 0;
   
   int MapDegree[26 + 8];
   int result;

   for (n = 0; n < 26 + 8 - 1; n++)
   {
       MapDegree[n] = 0;
   }

   /* This code build a graph with all possible 
    * edges in the password:
	* the first two letters [go] can be:
	* go, g0, 9o, 90  the first possible edges
	* ol, 0l          the second
	* le, l3          the third
	* 
	* All these edges are added into edges array
    */
   for (n = 0; n < N - 1; n++)
   {
       a = S[n] - 'a';
	   b = S[n + 1] - 'a';
       
       for (i = 0; i < strlen(images[a]); i++) 
	   {
           for (j = 0; j < strlen(images[b]); j++) 
		   {
              char* tmp;

			  strcpy(tmp, images[a][i]);
              strcpy(tmp, images[b][j]);

              Edges[EdgesCount] = tmp;
		   }
	   }
   }
   
   /* For all vertexes we need to calculate in-depth and out-depth
    * number of vertexes
    */
   for (n = 0; n < EdgesCount; n++)
   {
	   MapDegree[Pr10GetIndex(Edges[n][0])]++;
       MapDegree[Pr10GetIndex(Edges[n][1])]--;
   }
   
   /* Go through all vertexes and find those which out-depth 
    * number of vertexes is higher than the number of 
	* in-depth vertexes.
	*
	* We need to prove that the number of (+) vertexes 
	* is equal to the number of (-) vertexes.
	* It cn be proven as the maximum number
	* of (+) vertexes is 2.
    */
   for (n = 0; n < 26 + 8 - 1; n++)
   {
       if (MapDegree[n] > 0)
	   {
           add += MapDegree[n];
	   }
   }

   if (add == 0) 
   {
       result = EdgesCount + 1;
   }
   else
   {
       result = EdgesCount + add;
   }
}   

struct DicItem
{
    char* Key;
    int Value;
};

void DicItemIncrement(
    struct DicItem Dictionary[], 
	int* DicCount, 
	char* DicKey,
	int AddValue)
{
    int i;

    for (i = 0; i < DicCount; i++)
	{
        if (strcmp(szKey,szInput) == 0)
	        break;
	}
    
    if (i == DicCount - 1)
	{
		Dictionary[i].Key = DicKey;
        Dictionary[i].Value = 0;
		(*DicCount)++;
	}

	Dictionary[i].Value += AddValue;
}

void DicItemDecrement(
    struct DicItem Dictionary[], 
	int* DicCount, 
	char* DicKey,
	int DecValue)
{
    int i;

    for (i = 0; i < DicCount; i++)
	{
        if (strcmp(szKey,szInput) == 0)
	        break;
	}
    
    if (i == DicCount - 1)
	{
		Dictionary[i].Key = DicKey;
        Dictionary[i].Value = 0;
		(*DicCount)++;
	}

	Dictionary[i].Value -= DecValue;
}

void Pr10SolveLarge()   
{
    int K = 10;
	char* S = "tbilcwhafiparmdmotcaaloth";
	int N = 25;

    char* Edges[100];
	int EdgesCount = 0;

	struct DicItem Dictionary[100];
	int DicCount = 0;

	int celkovo_hran;

    int n;
	int k;

	char* startSubStr = (char*)malloc((K - 1) * sizeof(char));
    char* endSubStr = (char*)malloc((K - 1) * sizeof(char)); 
	char* prefixSubStr = (char*)malloc((K - 1) * sizeof(char)); 
	char* suffixSubStr = (char*)malloc((K - 1) * sizeof(char));

	/* Copy all possible substrings into Edges array 
	 * The length of all substrings is K
	 */
    for (n = 0; n <= N - K; n++)
	{
        Edges[n] = (char*)malloc(K * sizeof(char));
        strncpy(Edges[n], S + n, K);
	}

	EdgesCount = N - K;
    
	startSubStr = (char*)malloc((K - 1) * sizeof(char));
    endSubStr = (char*)malloc((K - 1) * sizeof(char));

    strncpy(Edges[n], S, K - 1);
    strncpy(Edges[n], S + (N - 1) - (K - 1), K - 1);
    
	/* Here we need some explanation. Lets choose a simple example:
	 * google. g->9, o->0, e->3, K = 3
	 * Edges: goo, oog, ogl, gle
	 * Graph vertexes will be all possible substrings by length: K - 1 = 2.
	 * 
	 * Lets choose the first edge: goo
	 * vrcholov = len('g9') * len('o0') = 2 * 2 = 4 - is the number of all possible 
	 * vertexes created from 'go' by applying all substitutions: g->9, o->0
	 * All possible vertexes: go, g0, 9o, 90.
	 * vrcholov is the number of all possible source vertexes for 'goo'
	 *  
	 * hran = vrcholov * strlen(images[ E[K-1]-'a' ]) - is the number of all possible 
	 * edges needed to represent 'goo' with all its variations. If the last letter
     * has 2 possible variants, we will have 4 * 2 possible edges.
	 * 
	 * DEG[ string(E.begin(),E.end()-1) ] -= strlen(images[ E[K-1]-'a' ]);
	 *
     * string(E.begin(),E.end()-1) = 'go' is one of the vertexes
	 * strlen(images[ E[K-1]-'a' ]) = 2 is a number of destination symbols
	 * That means that we have 2 edges which go out from the vertex 'go'
	 * All outgoing edges we mark with '-'
	 * 
	 * DEG[ string(E.begin()+1,E.end()) ] += strlen(images[ E[0]-'a' ]);
	 * string(E.begin()+1,E.end())
	 *
	 */

    for (n = 0; n <= EdgesCount - 1; n++)
	{
        char* edge = Edges[n];
		char symbol;

        int vrcholov = 1;
		int hran;

		for (k = 0; k < K - 1; k++)
		{
			symbol = edge[k] - 'a';
            vrcholov *= strlen(images[symbol]);
		}

		symbol = edge[K - 1] - 'a';
		hran = vrcholov * strlen(symbol);
        celkovo_hran += hran;

        strncpy(prefixSubStr, edge, K - 1);
        strncpy(suffixSubStr, edge + 1, K - 1);

        symbol = edge[0] - 'a';
        DicItemIncrement(Dictionary, &DicCount, suffixSubStr, strlen(images[symbol]);

    	symbol = edge[K - 1] - 'a';
        DicItemIncrement(Dictionary, &DicCount, prefixSubStr, strlen(images[symbol]);
	}

//        map< string, long long > DEG;
//
//        long long celkovo_hran = 0;
//        FOREACH(it,edges) {
//            string E = *it;
//            long long vrcholov = 1;
//            for (int k=0; k<K-1; ++k) vrcholov *= strlen(images[ E[k]-'a' ]);
//            long long hran = vrcholov * strlen(images[ E[K-1]-'a' ]);
//            celkovo_hran += hran;
//
//            DEG[ string(E.begin(),E.end()-1) ] -= strlen(images[ E[K-1]-'a' ]);
//            DEG[ string(E.begin()+1,E.end()) ] += strlen(images[ E[0]-'a' ]);
//        }
}



//// another fine solution by misof
//// #includes {{{
//#include <algorithm>
//#include <numeric>
//
//#include <iostream>
//#include <sstream>
//#include <string>
//#include <vector>
//#include <queue>
//#include <set>
//#include <map>
//#include <stack>
//
//#include <cstdio>
//#include <cstdlib>
//#include <cstring>
//#include <cctype>
//#include <cassert>
//
//#include <cmath>
//#include <complex>
//using namespace std;
//// }}}
//
///////////////////// PRE-WRITTEN CODE FOLLOWS, LOOK DOWN FOR THE SOLUTION ////////////////////////////////
//
//// pre-written code {{{
//// BEGIN CUT HERE
//#define DEBUG(var) { cout << #var << ": " << (var) << endl; }
//template <class T> ostream& operator << (ostream &os, const vector<T> &temp) { os << "[ "; for (unsigned int i=0; i<temp.size(); ++i) os << (i?", ":"") << temp[i]; os << " ]"; return os; } // DEBUG
//template <class T> ostream& operator << (ostream &os, const set<T> &temp) { os << "{ "; for(__typeof((temp).begin()) it=(temp).begin();it!=(temp).end();++it) os << ((it==(temp).begin())?"":", ") << (*it); os << " }"; return os; } // DEBUG
//template <class T> ostream& operator << (ostream &os, const multiset<T> &temp) { os << "{ "; for(__typeof((temp).begin()) it=(temp).begin();it!=(temp).end();++it) os << ((it==(temp).begin())?"":", ") << (*it); os << " }"; return os; } // DEBUG
//template <class S, class T> ostream& operator << (ostream &os, const pair<S,T> &a) { os << "(" << a.first << "," << a.second << ")"; return os; } // DEBUG
//template <class S, class T> ostream& operator << (ostream &os, const map<S,T> &temp) { os << "{ "; for(__typeof((temp).begin()) it=(temp).begin();it!=(temp).end();++it) os << ((it==(temp).begin())?"":", ") << (it->first) << "->" << it->second; os << " }"; return os; } // DEBUG
//// END CUT HERE
//#define FOR(i,a,b) for(int i=(int)(a);i<=(int)(b);++i)
//#define FOREACH(it,c) for(__typeof((c).begin()) it=(c).begin();it!=(c).end();++it)
//// }}}
//
///////////////////// CODE WRITTEN DURING THE COMPETITION FOLLOWS ////////////////////////////////
//
//char images[26][3] = {
//"a4",
//"b8",
//"c",
//"d",
//"e3",
//"f",
//"g9",
//"h",
//"i1",
//"j",
//"k",
//"l",
//"m",
//"n",
//"o0",
//"p",
//"q",
//"r",
//"s5",
//"t7",
//"u",
//"v",
//"w",
//"x",
//"y",
//"z" };
//
//long long transition_cost(const string &A, const string &B) {
//    for (int n=1; n<=int(A.size()); ++n) if (string(A.begin()+n,A.end()) == string(B.begin(),B.end()-n)) return n;
//}
//
//template<class T, class U> 
//class MincostMaxflow {
//  public:
//  class edge { 
//    public:
//      int source, destination;
//      T capacity, residue;
//      U cost;
//      edge(int s, int d, T cap, T res, U cost) 
//        : source(s), destination(d), capacity(cap), residue(res), cost(cost) { }
//  };
//
//  vector< vector<int> > V;
//  vector<edge> E;
//  void clear();
//  void addArc(int source, int destination, T capacity, U cost);
//  pair<T,U> getFlow(int source, int sink);
//};
//
//template<class T, class U> void MincostMaxflow<T,U>::clear() { V.clear(); E.clear(); }
//
//template<class T, class U> 
//void MincostMaxflow<T,U>::addArc(int source, int destination, T capacity, U cost) {
//    // cout << "addArc " << source << " " << destination << " " << capacity << " " << cost << endl;
//  int e = E.size();
//  if (source >= int(V.size())) V.resize(source+1);
//  if (destination >= int(V.size())) V.resize(destination+1);
//  V[source].push_back( e );
//  V[destination].push_back( e+1 );
//  E.push_back(edge(source,destination,capacity,capacity,cost));
//  E.push_back(edge(destination,source,capacity,0,-cost));
//}
//
//template<class T, class U> 
//pair<T,U> MincostMaxflow<T,U>::getFlow(int source, int sink) {
//  if (source >= int(V.size())) V.resize(source+1);
//  if (sink >= int(V.size())) V.resize(sink+1);
//
//  int N = V.size(), M = E.size();
//  T flowSize = 0;
//  U flowCost = 0;
//  T Tinfinity = 1; while (2*Tinfinity > Tinfinity) Tinfinity *= 2; 
//  U Uinfinity = 1; while (2*Uinfinity > Uinfinity) Uinfinity *= 2;
//  U Uepsilon = 1; for (int i=0; i<30; i++) Uepsilon /= 2;
//  vector<T> flow(M,0);
//  vector<U> potential(N,0);
//  while (1) {
//    // use dijkstra to find an augmenting path
//    vector<int> from(N,-1);
//    vector<U> dist(N,Uinfinity);
//    
//    priority_queue< pair<U,int>, vector<pair<U,int> >, greater<pair<U,int> > > Q;
//    Q.push(make_pair(0,source));
//    from[source]=-2; 
//    dist[source] = 0;
//
//    while (!Q.empty()) {
//      U howFar = Q.top().first; 
//      int where = Q.top().second;
//      Q.pop();
//      if (dist[where] < howFar) continue;
//
//      for (int i=0; i<int(V[where].size()); i++) {
//        if (E[ V[where][i] ].residue == 0) continue;
//        int dest = E[ V[where][i] ].destination;
//        U cost = E[ V[where][i] ].cost;
//        if (dist[dest] > dist[where] + potential[where] - potential[dest] + cost + Uepsilon) {
//          dist[dest] = dist[where] + potential[where] - potential[dest] + cost;
//          from[dest] = V[where][i];
//          Q.push( make_pair( dist[dest],dest ));
//        }
//      }
//    }
//
//    // update vertex potentials
//    for (int i=0; i<N; i++) {
//      if (dist[i]==Uinfinity) potential[i] = Uinfinity;
//      else if (potential[i]<Uinfinity) potential[i] += dist[i];
//    }
//
//    // if there is no path, we are done
//    if (from[sink] == -1) return make_pair(flowSize,flowCost);
//
//    // construct an augmenting path
//    T canPush = Tinfinity;
//    int where = sink;
//
//    while (1) { 
//      if (from[where]==-2) break;
//      canPush = min(canPush, E[ from[where] ].residue );
//      where = E[ from[where] ].source;
//    }
//
//    // update along the path
//    where = sink;
//    while (1) { 
//      if (from[where]==-2) break;
//      E[ from[where] ].residue -= canPush;
//      E[ from[where]^1 ].residue += canPush;
//      flowCost += canPush * E[ from[where] ].cost;
//      where = E[ from[where] ].source;
//    }
//    flowSize += canPush;
//  }
//  return make_pair(-1,47);
//}
//
//int main() {
//    int T; cin >> T;
//    FOR(t,1,T) {
//        int K; cin >> K;
//        string S; cin >> S;
//        int N = S.size();
//
//        set<string> edges;
//        for (int n=0; n+K<=N; ++n) edges.insert( string( S.begin()+n, S.begin()+n+K ) );
//        //DEBUG(edges);
//
//        string startv(S.begin(), S.begin()+K-1);
//        string endv(S.end()-(K-1), S.end());
//
//        map< string, long long > DEG;
//
//        long long celkovo_hran = 0;
//        FOREACH(it,edges) {
//            string E = *it;
//            long long vrcholov = 1;
//            for (int k=0; k<K-1; ++k) vrcholov *= strlen(images[ E[k]-'a' ]);
//            long long hran = vrcholov * strlen(images[ E[K-1]-'a' ]);
//            celkovo_hran += hran;
//
//            DEG[ string(E.begin(),E.end()-1) ] -= strlen(images[ E[K-1]-'a' ]);
//            DEG[ string(E.begin()+1,E.end()) ] += strlen(images[ E[0]-'a' ]);
//        }
//  
//        vector< pair<string,long long> > VON,DNU;
//        map<string,int> ID;
//        int next_id = 3;
//
//        long long add = 0;
//        FOREACH(it,DEG) {
//            long long x = it->second;
//            if (x==0) continue;
//
//            string V = it->first;
//            if (!ID.count(V)) { ID[V]=next_id; ++next_id; }
//            long long mul = 1;
//            for (int k=0; k<K-1; ++k) mul *= strlen(images[ V[k]-'a' ]);
//            if (x > 0) {
//                VON.push_back(make_pair(V,x*mul));
//                add += x*mul; 
//            } else DNU.push_back(make_pair(V,-x*mul));
//        }
//        
//        if (add == 0) {
//            cout << "Case #" << t << ": " << (celkovo_hran+K-1) << endl;
//            continue;
//        }
//        /*
//        DEBUG(celkovo_hran);
//        DEBUG(add);
//        DEBUG(VON); DEBUG(DNU);
//        DEBUG(transition_cost("abcde","cdefg"));
//        */
//
//        MincostMaxflow<long long,long long> MF;
//        MF.addArc(0,1,add-1,0);
//        FOREACH(it,VON) MF.addArc(1,ID[it->first],it->second,0);
//        FOREACH(jt,DNU) MF.addArc(ID[jt->first],2,jt->second,0);
//        FOREACH(it,VON) FOREACH(jt,DNU) MF.addArc(ID[it->first],ID[jt->first],min(it->second,jt->second),transition_cost(it->first,jt->first));
//
//        pair<long long, long long> result = MF.getFlow(0,2);
//        // DEBUG(result);
//        assert( result.first == add-1 );
//
//        cout << "Case #" << t << ": " << (celkovo_hran+result.second+K-1) << endl;
//    }
//  return 0;
//}
//// vim: fdm=marker:commentstring=\ \"\ %s:nowrap:autoread
