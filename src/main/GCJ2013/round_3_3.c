
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



#include <stdio.h>

const int inf = 1001001;
const int N = 4010;

int g[N][N];
int ss[N], ff[N], d1[N], d2[N], pred[N], last[N], edge[N];
int b[N];
int mx[N], r[N];

int main() {
  //freopen("in", "r", stdin);
  //freopen("out", "w", stdout);
  int tt,qq;
  scanf("%I64d", &tt);
  for (qq=1;qq<=tt;qq++) {
    int n; // N represents the total number of cities
	       // (cities are numbered from 1 to N), 
	int m; // M represents the total number of shuttle routes, 
	int p; // P represents the number of shuttle routes 
           // on my path from Mountain View (city #1) to London (city #2).
	int i;

    printf("Case #%I64d: ", qq);
    scanf("%I64d %I64d %I64d", &n, &m, &p);
    for (i=1;i<=n;i++) last[i] = 0;
    for (i=1;i<=m;i++) {

	   /*
	     This is followed by M lines, each consisting of four integers, ui, vi, ai, bi. 
         Each line represents the fact that there is 
         a one-way shuttle route from city ui to city vi, 
         and you know that its length can be any integer value from ai to bi, inclusive. 
         The routes are given IDs from 1 to M in the same order of the input.
	    */

      scanf("%d %d %d %d", ss+i, ff+i, d1+i, d2+i);
	  /* 1. (3, 4) ss[1] = 3
	   * 2. (3, 6) ss[2] = 3
	   * 3. (2, 1) ss[3] = 2
	   * 4. (4, 6) ss[4] = 4
	   * pred = [0,1,0,0]  last = []
       * 
	   * pred[1] = last[3] = 0
	   * last[3] = 1
	   * pred[2] = 1
	   * last[3] = 2
	   * pred[3] = 0
	   * last[2] = 3
	   * pred[4] = 0
	   * last[4] = 4
	   */
      pred[i] = last[ss[i]];
      last[ss[i]] = i;
    }
    for (i=1;i<=p;i++) scanf("%d", edge+i);

    for (i=1;i<=n;i++)
      for (j=1;j<=n;j++) 
		  g[i][j] = (i == j ? 0 : inf);

    for (i=1;i<=m;i++)
      if (d2[i] < g[ss[i]][ff[i]])
        g[ss[i]][ff[i]] = d2[i];
      


    for (long long k=1;k<=n;k++)
      for (long long i=1;i<=n;i++)
        for (long long j=1;j<=n;j++)
          if (g[i][k]+g[k][j] < g[i][j]) g[i][j] = g[i][k]+g[k][j];
    long long ll = 0, rr = p;
    while (ll < rr) {
      long long mid = (ll+rr+1) >> 1;
      for (long long i=1;i<=n;i++) mx[i] = inf;
      long long time = 0;
      for (long long i=mid;i>=1;i--) {
        time += d1[edge[i]];
        long long ver = ss[edge[i]];
        for (long long j=1;j<=n;j++)
          if (g[ver][j]-time < mx[j]) mx[j] = g[ver][j]-time;
      }
      for (long long i=1;i<=n;i++) r[i] = inf+10, b[i] = true;
      r[ff[edge[mid]]] = 0;
      for (long long it=1;it<=n;it++) {
        long long mn = inf+100, km = -1;
        for (long long i=1;i<=n;i++)
          if (b[i] && r[i] < mn) {
            mn = r[i];
            km = i;
          }
        b[km] = false;
        if (mn > mx[km]) continue;
        long long j = last[km];
        while (j > 0) {
          if (mn+d1[j] < r[ff[j]]) {
            r[ff[j]] = mn+d1[j];
          }
          j = pred[j];
        }
      }
      if (r[2] <= mx[2]) ll = mid;
      else rr = mid-1;
    }
    if (ll == p) printf("Looks Good To Me\n");
    else printf("%I64d\n", edge[ll+1]);
    fflush(stdout);
  }
  return 0;
}
