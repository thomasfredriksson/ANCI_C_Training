
/*
 
 Problem

 This problem was inspired by a board game called Can't Stop, 
 designed by Sid Sackson. 
 This problem has a similar idea, 
 but does not assume you have played Can't Stop.

 You're playing a (very large) board game. 
 In this game, you're given a sequence of N roll sets. 
 Each roll set consists of D die rolls. 
 Each die roll is an integer.

 To win the game, 
 you have to find the largest totally awesome interval of the sequence. 
 An interval is any consecutive sequence of roll sets. 

 An interval is called totally awesome 
 if there exist k numbers such that 
 every roll set in the interval 
 contains at least one of those k numbers.

 For example, suppose D=2 and k=3, 
 and the roll sets are as follows:

 Set 0: 10 20
 Set 1: 50 60
 Set 2: 70 30
 Set 3: 40 40
 Set 4: 30 30
 Set 5: 20 40

 The interval from Set 0 to Set 2 
 is totally awesome because roll sets 0-2 
 all contain 10, 50 or 70. 
 
 The interval from Set 1 to Set 5 is totally awesome 
 because roll sets 1-5 all contain 50, 30 or 40. 
 
 That interval contains 5 roll sets, 
 and it is the largest totally awesome interval.

 Your job is to output the indices of the first 
 and last roll set in the longest totally awesome interval. 
 
 If there are multiple totally awesome intervals of that length, 
 output the indices for the one with the lowest first index. 
 Note that the first roll set has index 0.

 Input

 The first line of the input gives the number of test cases, T. 
 T test cases follow. 
 Each test case starts with three space-separated integers: 
 N, D and k, as described above. 
 On the next line, there will be N*D integers. 
 The first D integers will be the rolls from the first roll set; 
 the second D integers will be the rolls from the second roll set; 
 and so on.

 Output

 For each test case, output one line containing "Case #x: y z", 
 where x is the case number (starting from 1), 
 and y and z are the first and last indices 
 of the longest totally awesome interval 
 (with ties broken using the lowest index), 
 as described above.

 Limits

 1 ≤ T ≤ 100.
 1 ≤ D ≤ 4.
 1 ≤ every die roll ≤ 105.
 For 6 test cases, 1 ≤ N ≤ 105.
 For all the other test cases, 1 ≤ N ≤ 103.
 Small dataset

 k = 2.
 Large dataset

 2 ≤ k ≤ 3.
 Sample


 Input 
 	
 Output 
 
 4
 8 1 2
 1 2 3 2 4 5 4 6
 4 3 2
 1 2 3 4 5 6 7 8 9 10 11 12
 6 2 3
 10 20 50 60 70 30 40 40 30 30 20 40
 10 1 3
 2 4 3 1 4 5 3 1 1 2

 Case #1: 1 3
 Case #2: 0 1
 Case #3: 1 5
 Case #4: 1 4

 The board game Can't Stop was designed by Sid Sackson, 
 and has been published by many publishers. 
 Neither Mr. Sackson nor any of the publishers endorses, 
 or has any involvement with, Google Code Jam.





 Divide and Conquer


 There is a nice divide-and-conquer solution. 
 Split the input down the middle; 
 the best interval is either entirely in the left half, 
 entirely in the right half, 
 or crosses the middle. 
 We handle the right and left cases recursively; 

 if we can handle the "crosses" case in linear time, 
 we will have an O(N lg N) algorithm, 
 which is good enough.

 If the best interval crosses the middle, 
 we know it must use the middle element, 
 so we must pick one of the D numbers in that roll to use. 
 
 Now go out as far as we can to the left and right with that number. 
 Once we stop, 
 we can extend either to the left or the right, 
 so there are 2D different numbers to try. 
 
 Expand again, 
 and get 2D more numbers to try for a final expansion. 
 
 In total, we only tried D*2D*2D different choices, 
 so the whole thing runs in 256N time, and we get our O(N lg N) time algorithm.

 Note: 
 surprisingly for a divide and conquer algorithm, 
 we don't use any information from the left and right halves 
 to solve the middle case; 
 the fact that it crosses is enough.
 

 Slow and Simple


 There's another set of approaches to this problem.
 Try every starting position, 
 expanding out to the right 
 and only making choices when necessary 
 (just as in the divide-and-conquer). 

 This is O(D^K * N) for each starting position, 
 or O(D^K * N^2) overall, which is too slow. 
 However, there are several different improvements 
 (some small, some large) 
 one could make in order to make this linear in N.



 Linear Time

 One improvement works in this way: 
 Every time you pick a number, 
 check if the roll before your starting position contains that number. 
 
 If so, we can safely ignore that choice 
 (because we would have done better starting one roll earlier 
  and choosing the same set of numbers).

 It turns out that this runs in linear time, 
 but the reason why isn't obvious. 
 
 We want to prove that a particular index i 
 is only visited from O(1) starting positions. 
 
 Imagine we have reached i from start. 
 Now imagine starting at i and going leftward 
 (as usual, we expand left as far as possible before picking each new number). 
 
 If we only choose numbers from the set that got us from start to i, 
 we must get back to start 
 (and no further, since the optimization 
  above guaranteed that start - 1 doesn't contain any of our numbers). 
  
  But there are only 1 + D + D2 + D3 different choices of numbers starting from i, so there are only that many possible positions for start, so i will only be visited O(D3) times.




 */





/***********************************  Proble 1   ************************************/





#include <iostream>
#include <utility>
#include <vector>
using namespace std;

template<typename T, typename U> inline void makemax(T &res, const U &x) {
  if (x > res) {
    res = x;
  }
}

int N, D, k;
struct RollSet {
  int val[4];
  int next_without[4];

  int where(int x) const {
    for (int i=0; i<D; ++i) {
      if (val[i] == x) {
        return i;
      }
    }
    return -1;
  }
};
vector<RollSet> rolls;

vector<int> xs;
vector<pair<int, int> > num_pos;

void rek(int pos, int &farthest) {
  // debug(pos);
  // debug(xs);

  if ((int)xs.size() == k || pos == N) {
    makemax(farthest, pos-1);
    return;
  }

  for (int j=0; j<D; ++j) {
    xs.push_back(rolls[pos].val[j]);
    int next = pos+1;

    if (next < N) {
      bool progress = true;
      while (progress) {
        progress = false;
        for (int x : xs) {
          int w = rolls[next].where(x);
          if (w != -1) {
            next = rolls[next].next_without[w];
            if (next == N) {
              progress = false;
              break;
            }
            progress = true;
          }
        }
      }
    }
    rek(next, farthest);
    xs.pop_back();
  }
}

void solve1() {
  cin >> N >> D >> k;

  rolls.resize(N);
  num_pos.clear();

  for (int i=0; i<N; ++i) {
    for (int j=0; j<D; ++j) {
      int x;
      cin >> x;
      rolls[i].val[j] = x;
      num_pos.push_back(make_pair(x, i));
    }
  }

  for (int j=0; j<D; ++j) {
    rolls[N-1].next_without[j] = N;
  }

  for (int i=N-2; i>=0; --i) {
    for (int j=0; j<D; ++j) {
      int x = rolls[i].val[j];
      int rv = rolls[i+1].where(x);
      if (rv != -1) {
        rolls[i].next_without[j] = rolls[i+1].next_without[rv];
      } else {
        rolls[i].next_without[j] = i+1;
      }
    }
  }

  pair<int, int> ans(0, 0);
  for (int a=0; a<N; ++a) {
    // debug(a);
    int b = a;
    rek(a, b);
    if (b - a > ans.second - ans.first) {
      ans = make_pair(a, b);
    }
  }
  cout << ans.first << ' ' << ans.second << '\n';
}

int main() {
  cin.sync_with_stdio(0);

  int T; cin >> T;
  for (int tt=1; tt<=T; ++tt) {
    cout << "Case #" << tt << ": ";
    solve1();
  }

  return 0;
}







/***********************************  Proble 2   ************************************/






#define _CRT_SECURE_NO_DEPRECATE
#include <iostream>
#include <cstdio>
#include <cstring>
#include <vector>
#include <algorithm>
#include <bitset>
#include <set>
#include <string>
#include <map>
#include <cmath>
#include <queue>
#include <deque>
#include <ctime>
#pragma comment (linker, "/STACK:256000000")

using namespace std;

int res, u, v;
int n, k, d;
vector < vector < int > > a;

void check(int start, int current, int x, int y, int z) {
	for (int j = current; j < n; ++j) {
		bool nice = false;
		for (int i = 0; i < d; ++i) {
			if (a[j][i] == x || a[j][i] == y || a[j][i] == z) {
				nice = true;
				break;
			}
		}

		if (!nice) {
			break;
		}

		if (j - start + 1 > res) {
			res = j - start + 1;
			u = start;
			v = j;
		}
	}
}

void check(int start, int current, int x, int y) {
	for (int j = current; j < n; ++j) {
		bool nice = false;
		for (int i = 0; i < d; ++i) {
			if (a[j][i] == x || a[j][i] == y) {
				nice = true;
				break;
			}
		}

		if (!nice) {
			if (k == 3) {
				for (int i = 0; i < d; ++i) {
					check(start, j, x, y, a[j][i]);
				}
			}
			break;
		}

		if (j - start + 1 > res) {
			res = j - start + 1;
			u = start;
			v = j;
		}
	}
}

void check(int current, int start, int position) {
	if (start + res - 1 >= n) {
		return ;
	}

	for (int i = current; i < n; ++i) {
		bool nice = false;
		for (int j = 0; j < d; ++j) {
			if (a[i][j] == a[start][position]) {
				nice = true;
				break;
			}
		}

		if (!nice) {
			for (int j = 0; j < d; ++j) {
				check(start, i, a[start][position], a[i][j]);
			}
			return ;
		}

		if (i - start + 1 > res) {
			res = i - start + 1;
			u = start;
			v = i;
		}
	}
}

void solve(int test) {
	cin >> n >> d >> k;
	a.assign(n, vector < int > (d, 0));
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < d; ++j) {
			cin >> a[i][j];
		}
		sort(a[i].begin(), a[i].end());
	}

	res = min(n, k);
	u = 0;
	v = min(n - 1, k - 1);
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < d; ++j) {
			check(i, i, j);
		}
	}

	printf("Case #%d: %d %d\n", test, u, v);
}

int main() {
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);

	int tests;
	scanf("%d", &tests);
	for (int test = 0; test < tests; ++test) {
		solve(test + 1);
		cerr << test << endl;
		cerr << "Current time: " << clock() << endl;
	}

	cerr << "Time: " << clock() << endl;
	return 0;
}










/***********************************  Proble 3   ************************************/






#define _CRT_SECURE_NO_WARNINGS
#pragma comment(linker, "/STACK:134217728")

#include <iostream>
#include <sstream>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cctype>
#include <cstring>
#include <vector>
#include <list>
#include <queue>
#include <deque>
#include <stack>
#include <map>
#include <set>
#include <algorithm>
#include <numeric>
using namespace std;

typedef long long Int;
typedef pair<int,int> PII;
typedef vector<int> VInt;

#define FOR(i, a, b) for(i = (a); i < (b); ++i)
#define RFOR(i, a, b) for(i = (a) - 1; i >= (b); --i)
#define CLEAR(a, b) memset(a, b, sizeof(a))
#define SIZE(a) int((a).size())
#define ALL(a) (a).begin(),(a).end()
#define PB push_back
#define MP make_pair

int A[1 << 17][4];
int R[1 << 17][4];
map<PII, int> Map;
map<pair<int, Int>, int> Map3;

int N, D, K;

int find(int a, int b)
{
	if(a == N - 1)
		return N;

	int& res = R[a][b];
	if(res != -1)
		return res;

	int i;
	FOR(i, 0, D)
		if(A[a + 1][i] == A[a][b])
			return res = find(a + 1, i);

	return res = a + 1;
}

int find2(int a, int b, int c)
{
	PII key(4*a + b, c);
	if(Map.find(key) != Map.end())
		return Map[key];

	int pos = find(a, b);
	if(pos == N)
		return Map[key] = N;
//		return N;

	int i;
	FOR(i, 0, D)
		if(A[pos][i] == c)
			return Map[key] = find2(pos, i, A[a][b]);
//			return find2(pos, i, A[a][b]);

	return Map[key] = pos;
//	return pos;
}

int find3(int a, int b, int c, int d)
{
	pair<int, Int> key(4*a + b, (Int(c) << 20) + d);
	if(Map3.find(key) != Map3.end())
		return Map3[key];

	int pos = max(find2(a, b, c), find2(a, b, d));

	if(pos == N)
		return Map3[key] = N;
//		return N;

	int i;
	FOR(i, 0, D)
	{
		if(A[pos][i] == c)
			return Map3[key] = find3(pos, i, A[a][b], d);

		if(A[pos][i] == d)
			return Map3[key] = find3(pos, i, A[a][b], c);
	}
//			return find2(pos, i, A[a][b]);

	return Map3[key] = pos;
//	return pos;
}

int findTwo(int a, int b, int c)
{
	int pos = find(a, b);
	if(pos == N)
		return N;

	return find2(pos, c, A[a][b]);
}

int findThree(int a, int b, int c, int d)
{
	int p = find(a, b);
	int pos = findTwo(a, b, c);
	if(pos == N)
		return N;

	return find3(pos, d, A[a][b], A[p][c]);
}

int SolveTest(int test)
{
	scanf("%d%d%d", &N, &D, &K);

	int i, j, k, l;
	FOR(i, 0, N)
		FOR(j, 0, D)
			scanf("%d", &A[i][j]);

	CLEAR(R, -1);
	Map.clear();
	Map3.clear();

	int res = 0;
	int pos = 0;

	if(K == 2)
	{
		FOR(i, 0, N)
			FOR(j, 0, D)
				FOR(k, 0, 4)
				{
					int a = findTwo(i, j, k);
					if(res < a - i)
					{
						res = a - i;
						pos = i;
					}
				}
	}
	else
	{
		FOR(i, 0, N)
			FOR(j, 0, D)
				FOR(k, 0, 4)
					FOR(l, 0, 4)
					{
						int a = findThree(i, j, k, l);
						if(res < a - i)
						{
							res = a - i;
							pos = i;
						}
					}
	}

	printf("Case #%d: %d %d\n", test + 1, pos, pos + res - 1);
	return 0;
}

int main()
{
	freopen("d.in", "r", stdin);
	freopen("d.out", "w", stdout);

	int T, t;
	char buf[1 << 7];
	gets(buf);
	sscanf(buf, "%d", &T);
	FOR(t, 0, T)
	{
		fprintf(stderr, "Solving %d/%d\n", t + 1, T);
		SolveTest(t);
	}

	return 0;
};
