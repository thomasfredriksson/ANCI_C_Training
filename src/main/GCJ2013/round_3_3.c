
/*


Problem


It is time for the Google Code Jam Finals, 
and we all want to be there! 
Unfortunately, a few of us accidentally ended up going to Mountain View 
instead of the correct location: London, England. 

But don't worry - we can take the free Google 
shuttle service from Mountain View to London!

The shuttle service consists of M one-way routes connecting pairs of cities. 
For every route, you know from which city and to which city it's going, 
but unfortunately you do not know exactly how long these routes are. 

Instead, for every route, you only know 
that its length can be any integer value from ai to bi, 
inclusive.

I have taken Google shuttles many times before, 
so I have suggested a path of routes from Mountain View to London. 

But you worry that my path-finding skills are not as good as yours, 
and you want to check my work.

Given the path I am suggesting, 
could it possibly be a shortest path from Mountain View to London? 

If not, what is the ID of the first shuttle route on my path 
that is definitely not part of a shortest path 
(assuming that all previous shuttle routes 
 have been taken according to the path I suggested)?

For example, suppose we have the following list of shuttle routes:

ID | Start City     |  Destination City  |  Shuttle Length
---+----------------+--------------------+----------------
1  | Mountain View  |  London            |  [100, 1000]
2  | Mountain View  |  Paris             |  [500, 5000]
3  | Paris          |  London            |  [400, 600]
4  | Paris          |  Moscow            |  [500, 5000]
5  | Moscow         |  London            |  [1, 10000]

I suggest the path Mountain View -> Paris -> Moscow -> London. 

The true shortest path might either be 
the direct route from Mountain View to London, 
or the path Mountain View -> Paris -> London. 

This means that the second route on my path (Paris -> Moscow) 
was the first one that is definitely not part of a shortest path.

Input

The first line of the input gives the number of test cases, T. 
T test cases follow. 
Each test begins with a line containing 
three positive integers N, M, and P. 

N represents the total number of cities 
(cities are numbered from 1 to N), 

M represents the total number of shuttle routes, 
and P represents the number of shuttle routes 
on my path from Mountain View (city #1) to London (city #2).

This is followed by M lines, each consisting of four integers, ui, vi, ai, bi. 
Each line represents the fact that there is 
a one-way shuttle route from city ui to city vi, 
and you know that its length can be any integer value from ai to bi, inclusive. 
The routes are given IDs from 1 to M in the same order of the input.

This is followed by a line consisting 
of P unique integers in the range from 1 to M. 

These represent, in order, the shuttle routes I am taking you on. 
Each one is an ID of a route from the previous list.


Output

For each test case, output one line containing "Case #x: n", 
where x is the case number (starting from 1) 
and n is the ID of the first shuttle route in my path 
that could not possibly be part of the shortest path from Mountain View to London. 
If there is no such route, print "Looks Good To Me" instead.

Limits

1 ≤ T ≤ 10.
1 ≤ ui, vi ≤ N.
1 ≤ ai ≤ bi ≤ 1000000.

My path is guaranteed to be a valid path 
from Mountain View (city #1) to London (city #2).

There might be more than one shuttle route between the same two cities, 
and there might be a shuttle route going from a city to itself. Also the suggested path might visit the same city more than once, but it will not use the same shuttle route more than once.
Small dataset

2 ≤ N ≤ 20.
1 ≤ M ≤ 20.
1 ≤ P ≤ 10
Large dataset

2 ≤ N ≤ 1000.
1 ≤ M ≤ 2000.
1 ≤ P ≤ 500.
Sample


Input 
 	
Output 
 
3
4 5 3
1 2 100 1000
1 3 500 5000
3 2 400 600
3 4 500 5000
4 2 1 10000
2 4 5
3 3 2
1 3 1 1
3 2 1 1
1 2 1 2
1 2
5 6 3
1 3 1 1
4 2 1 9
1 4 1 1
3 5 2 2
5 2 2 2
3 4 1 2
1 6 2



Introduction

The problem asks you to determine which part of a path E1, E2, ..., Ek 
can be a prefix of some shortest path from 1 to 2. 

This would be easy if not for the fact 
we don't know the exact edge costs 
- we only know ranges into which they fall.


Small dataset


Let's concentrate on any fixed prefix 
E1, E2, ..., Ep of the proposed path, 
and try to make it a prefix of some shortest path. 

If we decide on a shortest path candidate 
(that includes our prefix), 
it's obivously advantageous to assume the edges 
on our path are as short as possible, 
and the other edges are as long as possible.

Notice that a consequence of this is that 
we can just restrict ourselves to looking at graphs 
in which each of the edges is either as long 
or as short as possible. 

Thus, since in the small dataset 
the number of edges is only 20, 
we can simply check all possibilities 
- make some of the edges short, the rest long, 
find the shortest path in the resulting graph 

(preferring the proposed path by, 
for instance, decreasing the cost of the edges 
on this path by a small epsilon), 

and then out of all the possibilities 
pick the one that takes 
the most steps along the proposed path.


Large dataset

This is obviously not going to fly for the large dataset. 

Again, we fix a prefix E1, E2, ..., Ep of the proposed path 
and try to make it a prefix of the shortest path. 
Assume Ep ends in Tokyo. 

Thus, we will try to get from Tokyo to London as fast as possible, 
while still not allowing for a shorter path 
from Mountain View to London 
that doesn't begin with our prefix. 

If we want to optimize for speed, 
we can do a binary search for the longest prefix 
that can be a start of some shortest path; 

if we optimize for simplicity, 
we can just iterate over all prefixes.

Imagine two robots that try to reach London as fast as possible. 
One starts from Tokyo, and has a handicap of the cost to travel 
from the Mountain View to Tokyo along the proposed path (we call this the "good" robot). 
When this robot goes across an edge, it will always take the minimal cost - 
this robot represents the shortest path we hope to construct. 
         
The other robot starts from Mountain View, 
and tries to reach London via some other path. 
We call this the "bad" robot and will try to force it to take as long as possible. 
 
The question is whether we can make the good robot be at least 
as fast as the bad robot 
(the bad robot can obviously be equally fast simply by following the good robot).

We already set the costs for the edges E1 to Ep to the low values. 
Since our aim is to make the good robot move fast and the bad robot move slow, 
a naive approach is to simply have the good robot pay the low cost for all other edges, 
and the bad robot pay the high cost. 
 
However, we will still encounter a problem in this model. 
If two robots go across a same edge, 
they are actually taking different costs, which is not possible in a fixed configuration.



Notice, however, that the two robots are walking the same graph. 
Thus, if they reach a same node at a different time, 
the one that arrived earlier will always 
beat the other one if the shortest path goes through that node, 
because it can always follow the other one’s route. 

This means the later robot does not have any purpose 
in visiting this node at all. 
Since ties are resolved in favor of the good robot, 
we can simply decrease the good robot's handicap by 0.5 to avoid any ties.

Thus, we can solve the problem with a single run of Dijkstra's algorithm. 

We begin with two starting points - 
one in Mountain View at time 0, 
and the other in Tokyo with time equal to the cost of travel 
along the proposed path to Tokyo minus 0.5. 

When processing a node, we calculate the costs of outgoing edges as follows:

If the edge is one of E1, ..., Ep, we take the low cost.
If the current node is processed because the good robot reached it 
(which we know because the cost of the current node is not an integer, it ends with 0.5), 
the cost is the low cost.

Otherwise, we are processing the node because the bad robot reached it, 
and the cost is the high cost.

As Dijkstra's algorithm visits a node only 
once to process outgoing edges, 
we will never have a robot visit a node that the other robot reached earlier.



*/


/*
#include <cstdio>
#include <numeric>
#include <iostream>
#include <vector>
#include <set>
#include <cstring>
#include <string>
#include <map>
#include <cmath>
#include <ctime>
#include <algorithm>
#include <bitset>
#include <queue>
#include <sstream>
#include <deque>

using namespace std;

#define mp make_pair
#define pb push_back
#define rep(i,n) for(int i = 0; i < (n); i++)
#define re return
#define fi first
#define se second
#define sz(x) ((int) (x).size())
#define all(x) (x).begin(), (x).end()
#define sqr(x) ((x) * (x))
#define sqrt(x) sqrt(abs(x))
#define y0 y3487465
#define y1 y8687969
#define fill(x,y) memset(x,y,sizeof(x))
                         
typedef vector<int> vi;
typedef long long ll;
typedef long double ld;
typedef double D;
typedef pair<int, int> ii;
typedef vector<ii> vii;
typedef vector<string> vs;
typedef vector<vi> vvi;

template<class T> T abs(T x) { re x > 0 ? x : -x; }

int n;
int m;
int p;
int d[2][10000];
int light[10000];
int g[2][10000];
int A[10000];
int B[10000];
int C[10000];
vii v[10000];
set<ii> all;
int mark[10000];
int x[10000];

int go (int t, int s, int sd) {
	all.clear ();
	for (int i = 0; i < n; i++) {
		d[t][i] = 1e9;
		mark[i] = 0;
	}
	d[t][s] = sd;
	mark[s] = 1;
	all.insert (mp (sd, s));
	while (!all.empty ()) {
		int x = all.begin ()->se;
		all.erase (all.begin ());
//		printf ("%d %d = %d\n", t, x, d[t][x]);
		if (t == 1 && d[1][x] > d[0][x]) continue;
		for (int i = 0; i < sz (v[x]); i++) {
			int y = v[x][i].fi;
			int z = g[t][v[x][i].se];
			if (d[t][y] > d[t][x] + z) {
				if (mark[y]) all.erase (mp (d[t][y], y));
				d[t][y] = d[t][x] + z;
				mark[y] = 1;
				all.insert (mp (d[t][y], y));
			}
		}
	}
	if (t == 1 && d[1][1] <= d[0][1]) re 1;
	re 0;
}

int main () {
	int tt, it;
	cin >> tt;
	for (it = 1; it <= tt; it++) 
	{
		int ans, cur;
        scanf ("%d%d%d", &n, &m, &p);

        for (i = 0; i < n; i++) 
			v[i].clear();

        for (i = 0; i < m; i++) 
		{
        	int a, b, c, d;
        	scanf ("%d%d%d%d", &a, &b, &c, &d); a--; b--;
        	v[a].pb (mp (b, i));
        	A[i] = c;
        	B[i] = d;
        	C[i] = b;
        }

        for (i = 0; i < p; i++) 
		{
        	scanf("%d", &x[i]);
        	x[i]--;
        }

	    ans = -1;
	    memset (light, 0, sizeof (light));

		cur = 0;
	    for (i = 0; i < p; i++) 
		{
	       cur += A[x[i]];
	       light[x[i]] = 1;

	       for (j = 0; j < m; j++)
		   {
	           if (light[j]) 
			   {
	        	   g[0][j] = A[j];
	        	   g[1][j] = A[j];
	           } 
			   else 
			   {
	        	   g[0][j] = B[j];
	        	   g[1][j] = A[j];
	           }
		   }

	       go (0, 0, 0);
	       if (!go (1, C[x[i]], cur)) 
		   {
	           ans = x[i] + 1;
	           break;
	       }
	    }
				
		cout << "Case #" << it << ": ";
		if (ans == -1) cout << "Looks Good To Me"; else cout << ans;

		cout << endl;

		cerr << it << " / " << tt << " " << clock () << endl;
	}
	return 0;
}

*/