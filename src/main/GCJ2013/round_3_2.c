
/*


Problem


You have recently purchased a nice big farmyard, 
and you would like to build a fence around it. 
There are already N fence posts in your farmyard.

You will add lengths of fence in straight lines 
connecting the fence posts. 

Unfortunately, for reasons you don't fully understand, 
your lawyers insist you actually have to use all the fence posts, 
or things will go bad.

In this problem, the posts will be represented 
as points in a 2-dimensional plane. 

You want to build the fence by ordering the posts in some order, 
and then connecting the first with the second, second with third, 
and finally the last one with the first. 

The fence segments you create should 
be a polygon without self-intersections. 

That is, at each fence-post there are only two fence segments, 
and at every other point there is at most one fence segment.

Now that's easy, but you also actually want 
to preserve the fact your farmyard is big! 

It's not really fun to wall off most of your farmyard with the fences. 
So you would like to create the fence in such a way 
that the enclosed area is more than half 
of the maximum area 
you could enclose if you were allowed not to use all the posts.



Input



The first line of the input gives the number of test cases, T. 
T test cases follow. 
The first line of each test case contains the number N of posts. 
The posts are numbered from 0 to N - 1. 
Each of the next N lines contains two integers Xi and Yi 
separated by a single space: the coordinates of the i-th post.



Output



For each test case, output one line containing "Case #x: ", 
where x is the case number (starting from 1), 
followed by N distinct integers from 0 to N - 1, 
separated by spaces. 
They are the numbers of the posts, 
in either clockwise or counter-clockwise direction, 
that you will use to build the fence. 
Note that the first and last posts are connected. 


If there are multiple solutions, print any of them.

Limits

The posts will be at N unique points, 
and will not all lie on the same line.

Small dataset

1 ≤ T ≤ 100
3 ≤ N ≤ 10
-100 ≤ Xi, Yi ≤ 100
Large dataset

1 ≤ T ≤ 30
3 ≤ N ≤ 1000
-50000 ≤ Xi, Yi ≤ 50000
Sample


Input 
 	
Output 
 
3
4
1 2
2 0
0 0
1 1
5
0 0
1 1
2 2
0 2
2 0
3
0 0
1 0
0 1
Case #1: 0 1 2 3
Case #2: 0 1 4 2 3
Case #3: 0 2 1

In the first test case, 
there are three polygons we can construct, 
and two of them have a large enough area 
— the ones described by sequences 0 1 2 3 and 0 2 1 3. 
The polygon described by 0 1 3 2 would be too small. 
In the second test case, 
we have make sure the polygon does not intersect itself, 
so, for instance, 0 1 2 3 4 or 0 1 3 4 2 would be bad. 
In the third case, any order describes the same triangle and is fine.





Introduction


In this problem, we are given a set of points on a 2D plane. 
The goal is to construct a single simple polygon that uses all of the points. 
To make things interesting, the area of the resulting polygon has an additional constraint; 
it must be strictly more than half of the area of the largest polygon 
constructed from any subset of points. 

A polygon that takes up maximum area 
but can use a subset of the points is always 
the convex hull of the points.



Small


For the small data case, we choose an exhaustive approach 
that finds the polygon with maximal area. 
Assuming that it is possible to construct a polygon 
with a large enough area 
(see the explanation of the large case for justification), 
the maximal area polygon will be a solution. 
The question now becomes how to construct a polygon of largest area. 
We permute the order of the fence posts. 
For each ordering, we verify the polygon has no intersecting edges 
(being careful of edges that pass through fence posts). 
We keep the polygon with the largest area. 
With N=10 fence posts, this takes no more than O(N! * T), 
where T is how long it takes to validate a polygon and compute its area. 
Verification can be implemented easily in O(N2) time. 
Polygon area can be done in O(N) time. 
So the resulting runtime is O(N! * N^2). 
For N=10, this is around 500 million basic operations, 
which is relatively quick.


Large


For the large case, N = 1000, 
exhausting even a subset of fence post permutations it too costly. 
An algorithm over O(N^3) may take too long to compute. 
We explore a more direct method.


As mentioned earlier, 
the polygon with the largest possible area 
using a subset of the fence posts is the convex hull. 
Suppose we break the convex hull into two polygons, 
one of these polygons must contain at least half of the convex hull’s area. 
Notice that regardless of how the convex hull is split, 
one of the resulting polygons is always at least half of the area.


Suppose we have a way to split the convex hull using the interior fence posts.
This results in a polygon with at least half of the area of the convex hull, 
and at least one isolated vertex on the exterior of the polygon. 
We just need to connect the isolated vertices 
to the polygon without introducing intersecting edges. 
This process could only possibly add more area to the polygon, 
hence the final polygon is strictly more than half of the convex hull’s area.

Making the above observation as the basis of our solution, 
we can arbitrarily split the convex hull into the upper half and lower half.


Note that the union of upper and lower polygon 
contains the full area of the convex hull. 
Furthermore the interior path cannot cause intersecting edges, 
because we chose to sort the points from left to right 
(note, special care must be taken when ties are involved, see special cases). 
One of these polygons has at least half the area of the convex hull, as explained above.


As mentioned, the larger of the two polygons will not contain all of the points. 
However, we can extend the polygon to use all the points. 
This is done by iteratively adding a point to the polygon. 
Because all exterior points are on the convex hull, 
the polygon must increase in area as we do this. 
The points are added to the polygon by taking an exterior point 
and “connecting” it to the point left and right of it 
on the polyline dividing the convex hull. 
For an optimization, 
the exterior points can be added at the same time 
as forming the polyline if you know the exterior points when constructing the polyline.


Special cases


On the left example, the leftmost and rightmost points are connected. 
In this case, the convex hull is split into the full polygon and a line segment. 
The full polygon, a.k.a. the convex hull, can simply be used as the final answer.

The second special case, above on the right, 
can occur when points share the same x coordinate 
and “left to right” is not well defined. 
An elegant way to handle this case is to project all of the points onto 
a “nearly horizontal” line and retain the scalar project values. 
If we pick a line that is not parallel 
or orthogonal to the line formed by any two points in our input, 
then each projected point will have a unique position on the “horizontal” line. 
A line that passes through points (0,0) and (200000,1) will work.


Once all points are projected onto the line, 
the sort order of the points is determined by the projected value. 
If we take a collection of points 
and go through them in this order we form a polyline.


We can show that running this algorithm 
including the two endpoints of one of our half hulls 
and all the middle points will create a closed polygon. 
Notice that when you also expand the middle set 
to also contain all points that are in the half hulls that the polygon grows in size. 
This is because the points added are outside our current polygon 
as a property of the half hull. 
A line coming from the middle points 
will move away from the first polygon towards the hull 
and back to the middle points when following our order. 
This technique ensures that the area of our polygon 
grows bigger than the polygon with just the middle points.

Putting it all together

To sum up the solution, we will reiterate the main steps of our algorithm:

Take the input and turn it into three sets of points cut using line (0,0) to (200000,1):
upper hull
lower hull
middle points
Run the closing algorithm on using upper hull and combining lower hull and middle points.
Run the closing algorithm on using lower hull and combining upper hull and middle points.
Print the solution with the larger polygon.



*/


/*#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <cassert>
#include <iostream>
#include <sstream>
#include <vector>
#include <queue>
#include <set>
#include <map>
#include <utility>
#include <numeric>
#include <algorithm>
#include <bitset>
#include <complex>

using namespace std;

typedef unsigned uint;
typedef long long Int;

const int INF = 1001001001;
const Int INFLL = 1001001001001001001LL;

template<typename T> void pv(T a, T b) { for (T i = a; i != b; ++i) cout << *i << " "; cout << endl; }
template<typename T> void chmin(T& a, T b) { if (a > b) a = b; }
template<typename T> void chmax(T& a, T b) { if (a < b) a = b; }

const double INFF = 1e12;
const double EPS = 1e-9; // !!!
const double PI = acos(-1);

// sign
int sgn(double r) {
    return (r < -EPS) ? -1 : (r > +EPS) ? +1 : 0;
}

// 2D point
struct Pt {
    double x, y;
    Pt(double x = 0, double y = 0) : x(x), y(y) { }
    Pt operator + (const Pt& o) const { return Pt(x + o.x, y + o.y); }
    Pt operator - (const Pt& o) const { return Pt(x - o.x, y - o.y); }
    Pt operator * (const double k) const { return Pt(k*x, k*y); }
    Pt operator / (const double k) const { return Pt(x/k, y/k); }
    double dot(const Pt& o) const { return x*o.x + y*o.y; }
    double det(const Pt& o) const { return x*o.y - o.x*y; }
    double abs2() const { return x*x + y*y; }
    double abs() const { return sqrt(abs2()); }
    double angle() const { return atan2(y, x); }
    Pt rot(const double t) const { return Pt(x*cos(t) - y*sin(t), x*sin(t) + y*cos(t)); }
    Pt rot90() const { return Pt(-y, x); }
    bool operator < (const Pt& o) const { return x != o.x ? x < o.x : y < o.y; }
    bool operator == (const Pt& o) const { return sgn(x - o.x) == 0 && sgn(y - o.y) == 0; }
};
ostream& operator << (ostream& os, const Pt& p) { os << "(" << p.x << ", " << p.y << ")"; return os; }

// 2 * (signed area of a triangle)
double tri(Pt a, Pt b, Pt c)
{
    return (b - a).det(c - a);
}

int iSP(Pt a, Pt b, Pt c)
{
    int cross = sgn(tri(a, b, c));
    if (cross != 0) return cross; // 1: counter clockwise, -1: clockwise
    if (sgn((b - a).dot(c - a)) < 0) return -2; // c-a-b
    if ((b - a).abs2() < (c - a).abs2()) return +2; // a-b-c
    return 0;
}

pair<int, int> convex_hull(int n, Pt p[], Pt lo[], Pt up[])
{
    int m = 0, k = 0, i;
    sort(p, p + n);
    for (i = 0;     i <  n; lo[m++] = p[i++]) for (; m > 1 && iSP(lo[m - 2], lo[m - 1], p[i]) < 0; --m) ;
    for (i = n - 1; i >= 0; up[k++] = p[i--]) for (; k > 1 && iSP(up[k - 2], up[k - 1], p[i]) < 0; --k) ;
    return make_pair(m, k);
}

int convex_hull(int n, Pt p[], Pt q[])
{
    int m = 0, i, r;
    sort(p, p + n);
    for (i = 0; i <  n; q[m++] = p[i++]) 
		for (; m > 1 && sgn(tri(q[m - 2], q[m - 1], p[i])) <= 0; --m) ;
    for (i = n - 2, r = m; i >= 0; q[m++] = p[i--]) for (; m > r && sgn(tri(q[m - 2], q[m - 1], p[i])) <= 0; --m) ;
    return m - 1;
	return 1;
}

vector<Pt> getPolygon(int n, Pt p[], int m, Pt base[])
{
    Pt rem[1024];
    int rc = 0;
    for (int i = 0; i < n; ++i) {
        bool in_ch = false;
        for (int j = 0; !in_ch && j < m; ++j) {
            in_ch |= p[i] == base[j];
        }
        if (!in_ch) {
            rem[rc++] = p[i];
        }
    }

    sort(rem, rem + rc);
    vector<Pt> res;
    for (int i = 0; i < m; ++i) {
        res.push_back(base[m - i - 1]);
    }
    for (int i = 0; i < rc; ++i) {
        res.push_back(rem[i]);
    }
    return res;
}

double getArea(vector<Pt> poly)
{
    const int n = poly.size();
    double res = 0;
    for (int i = 0; i < n; ++i) {
        res += poly[i].det(poly[(i + 1) % n]);
    }
    return fabs(res) / 2;
}

int main()
{
    int T;
    int CN;
    //cin >> T;

    for (CN = 1; CN <= T; ++CN) 
	{
        int N, i, M;
		double A = 0;
        Pt I[1024], P[1024], hull[1024], lo[1024], up[1024];
        //cin >> N;
        for (i = 0; i < N; ++i) {
            //cin >> P[i].x >> P[i].y;
        }
        memcpy(I, P, sizeof(P));
        M = convex_hull(N, P, hull);

        for (i = 0; i < M; ++i) {
            A += hull[i].det(hull[(i+1) % M]);
        }
        A = fabs(A) / 2;

        pair<int, int> ps = convex_hull(N, P, lo, up);

        vector<Pt> los, ups;
        los = getPolygon(N, P, ps.first, lo);
        for (int i = 0; i < ps.second; ++i) {
            up[i] = up[i] * -1;
        }
        for (int i = 0; i < N; ++i) {
            P[i] = P[i] * -1;
        }
        ups = getPolygon(N, P, ps.second, up);
        for (int i = 0; i < ups.size(); ++i) {
            ups[i] = ups[i] * -1;
        }

        double loa = getArea(los), upa = getArea(ups);
        vector<Pt> sol = loa > upa ? los : ups;

        cout << "Case #" << CN << ":";
        for (int i = 0; i < sol.size(); ++i) {
            for (int j = 0; j < N; ++j) {
                if (sol[i] == I[j]) {
                    cout << " " << j;
                    break;
                }
            }
        }
        cout << endl;
    }

    return 0;
}
*/