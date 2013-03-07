/*
Long ago, an alien civilization built a giant monument. 
The floor of the monument looked like this:
 ###############
#.............#
#.###########.#
#.#.........#.#
#.#.#######.#.#
#.#.#.....#.#.#
#.#.#.###.#.#.#
#.#.#.#.#.#.#.#
#.#.#.###.#.#.#
#.#.#.....#.#.#
#.#.#######.#.#
#.#.........#.#
#.###########.#
#.............#
###############

Each '#' represents a red tile, and each '.' represents a blue tile. 
The pattern went on for miles and miles 
(you may, for the purposes of the problem, assume it was infinite). 

Today, only a few of the tiles remain.
The rest have been damaged by methane rain and dust storms. 
Given the locations and colours of the remaining tiles, 
can you find the center of the pattern?
 
Input
 
The first line of the input gives the number of test cases, T. 
T test cases follow. 

Each one starts with a line containing N, 
the number of remaining tiles. 

The next N lines each contain Xi, Yi, 
and the tile colour (either '#' or '.').


Output
 
For each test case, output one line containing "Case #c: X Y", 
where c is the case number (starting from 1) 
and (X, Y) is the location of the center of the pattern. 

If there is more than one possible answer, 
output the (X, Y) closest to (0, 0) in Manhattan distance 
(the distance in x, plus the distance in y). 

If there are still ties, output the one with the largest X. 
If there are still ties after that, 
output the one with the largest Y. 

If there is no possible answer, output "Case #c: Too damaged".
 
Limits
 
1 ≤ T ≤ 50.
 The list of coordinates in each test case will not contain duplicates. 

Small dataset
 
1 ≤ N ≤ 100.
 -100 ≤ Xi ≤ 100.
 -100 ≤ Yi ≤ 100. 

Large dataset
 
1 ≤ N ≤ 1000.
 -1015 ≤ Xi ≤ 1015.
 -1015 ≤ Yi ≤ 1015. 

Sample

Input 
Output 

6
 1
 0 0 .
 1
 0 0 #
 3
 0 0 #
 0 1 #
 1 0 #
 5
 50 30 #
 49 30 #
 49 31 #
 49 32 #
 50 32 #
 2
 -98 0 #
 99 50 .
 4
 88 88 .
 88 89 .
 89 88 .
 89 89 .
 

Case #1: 0 0
 Case #2: 1 0
 Case #3: 1 1
 Case #4: 50 31
 Case #5: 1 0
 Case #6: Too damaged
 

Solution:

To begin with, let's try to formalize the rules of forming the pattern of tiles. 

.    - Blue
#, + - Red

If the center is at some position x, y, 
then the red tiles are those in positions x', y' 
for which the number max(|x - x'|, |y - y'|) is odd, 
while the blue tiles are those for which this number is even. 

For x', y' we have: max(|x - x'|, |y - y'|) = 2k + 1

This is because the formula max(|x - x'|, |y - y'|) = C for any C 
describes a square ring around x, y, and the rings alternate color with the parity of C

For the small problem, we can prove that if there exists a solution, 
there exists one with |X| + |Y| < 202. 
Thus, we can check all candidates for the center, 
for each one check whether all the tiles have the right colors, and output the best candidate. 

This will not run in time for the large data, of course, 
as we will have over 1030 candidate centers to check.

Single tile analysis

We may now assume we know the parity of x and y. 
We will simply check all four possibilities, 
finding the best possible choice of the center for each of the four assumptions, 
and then pick the one specified by the tie-breaking rules 
(or output "Too damaged" if none of the four assumptions 
led to finding a viable center for the pattern). 

This makes it easier to analyse the information gained 
by knowing the color of a single tile. 

Suppose the tile at some position x', y' is, say, red. 
This means max(|x - x'|, |y - y'|) has to be odd. 

For x', y' we have: max(|x - x'|, |y - y'|) = 2k + 1

Now, we know the parities of x, y, x' and y', and so:

1. if both x - x' and y - y' are odd, 

   max(|x - x'|, |y - y'|) = 2k + 1
   |x - x'| = 2n + 1  
   |y - y'| = 2n + 1

   then any choice of a center (satisfying the parity requirements for x and y) 
   is going to fit our knowledge;

2. if both x - x' and y - y' are even, 
   
   max(|x - x'|, |y - y'|) = 2k + 1
   |x - x'| = 2n 
   |y - y'| = 2p

   then there is no solution satifying the parity requirements;

3. if, say, x - x' is odd, while y - y' is even, 

   max(|x - x'|, |y - y'|) = 2k + 1
   |x - x'| = 2n + 1
   |y - y'| = 2p 

   we have to have |x - x'| ≥ |y - y'|

If there is any tile of the second type, 
we can immediately return "Too damaged" for these parity assumptions.

We can ignore of tiles of the first type, 
and now we are left only with tiles of the second type.

Note that in the third case, 
since the parities of x - x' and y - y' differ, 
it doesn't matter whether we use a strict inequality, 
as the equality case is eliminated from consideration by the parity assumptions. 

Thus, when considering regions defined by these inequalities, 
we can ignore problems related to "what happens on the edges of these regions", 
as - by the reasoning above - 
the edges will necessarily be eliminated from consideration by the parity assumptions.

The first and second cases are easy to analyse; 

the trick is to find out whether a solution exists 
(and if yes, find the best one) satisfying the set of conditions 
of the type |x - x'| ≥/≤ |y - y'| for various x' and y'. 

Transforming the condition |x - x'| ≥ |y - y'| 
we see it is equivalent to saying that one of the following has to hold:

x + y ≥ x' + y' and x - y ≥ x' - y', or
x + y ≤ x' + y' and x - y ≤ x' - y'.


Dividing the plane

The lines x + y = xi + yi and x - y = xi - yi 
(which are the boundaries of the constraint-satisfaction region for the input tiles) 
divide the plane into at most (N + 1)2 rectangles. 

The idea of our algorithm will be as follows:

1. Iterate over the four sets of parity assumptions about the center.
2. Iterate over all rectangles formed by the boundary lines, 
and for each of them check whether it satisfies the constraints posed by all input tiles.
3. For each rectangle satisfying the constraints, find the best 
   (according to the tie-resolution conditions) center candidate within it (if any)
output the best of all center candidates found.
4. A fun fact is that there will be at most N+1 rectangles that satisfy the constraints; 
so we need not worry overly about the performance of the 
"find the optimal within the rectangle" phase 
(as long as it is independent of the size of the rectangle). 

The naive approach to the second phase is O(N3) 
(for each rectangle check all tiles), 
which with N up to a thousand and 50 testcases risks being too slow,

so we'll need to speed it up a bit.
There are many ways to trim down the runtime of the constraint-checking phase for rectangles. 
One sample way is to process the rectangles "row-by-row", as follows: 

Take the set of rectangles with A ≤ x+y ≤ B, 
with A and B being some two adjacent boundary values. 

For each input tile (out of those that set any constraints on the center position),
we have two areas of constraint satisfaction; 

but only one of them is compatible with A ≤ x+y ≤ B, 
because one of the areas satisfies the constraint x+y ≥ C, 
while the other has x+y ≤ C. 

This means that we know which area is the interesting one for this row; 
so we obtain a constraint on x - y that has to be satisfied by all the rectangles in this row. 
This will be either of the form x - y ≤ D, or x - y ≥ D. 
We take the largest of the lower bounds, 
the largest of the upper bounds,
and obtain a rectangle that we have to check. 

This algorithm runs in O(N2) time, which will be easily fast enough.

A more advanced algorithm (using the sweep line technique) 
can be used to obtain a runtime of O(N logN) runtime. 

We will not describe it (
as it is not necessary to obtain a fast enough program with the constraints given), 
but we encourage the reader to figure it out.


Finding the best point within a rectangle


This was the part of the problem that seemed to cause most difficulties for our contestants. 
There are two cases to consider here. 

Let's assume our rectangle is defined by A ≤ x+y ≤ B and C ≤ x-y ≤ D.

Let us define 
g(k, l) = min(|k|, |l|) if k and l are of the same sign, 0 otherwise.

1. If g(A, B) = 0 and g(C, D) = 0, then the point (0, 0) is within our rectangle. 
   If min(|A|,|B|) = 0 and min(|C|,|D|) = 0

   In this case it suffices to check the near vicinity of the origin. Specifically:

   If both x and y are supposed to be even, 
   (0, 0) is obviously the optimal solution.

   If both x and y are supposed to be odd, 
   then the best four points, in order, 
   are (1, 1), (1, -1), (-1, 1) and (-1, -1). 
   
   If B ≥ 2 we can take (1, 1) and we're done. 
   Otherwise, if D ≥ 2, we take (1, -1); and so on. 
   
   If all the four points are infeasible, 
   the rectangle contains no points satisfying the parity constraints.

   If, say, x is supposed to be odd, while y is even, 
   the first eight candidates are 
   (1, 0), (-1, 0), (3, 0), (1, 2), (1, -2), (-1, 2), (-1, -2), (-3, 0). 
   
   Again, one can check that if none of them is feasible, 
   the rectangle contains no points satisfying the parity constraints. 
   
   The same happens when x is even and y is odd.

   Thus, if (0, 0) is within the rectangle, 
   we can check a constant number of points and take the best feasible one of them.

2. When (0, 0) is not within the rectangle, 
   we first look for the smallest Manhattan distance of any point within the rectangle. 
   
   It is equal to M := max(g(A, B), g(C, D)). 
   As all the boundaries have parities disagreeing with the parity assumptions, 
   the smallest Manhattan distance we can hope for is M + 1. 
   
   We now have an interval of points with Manhattan distance M + 1 in our rectangle, 
   the best one of them is the one with the highest X coordinate 
   (out of the ones fulfilling the parity conditions).
   
   The one last special case to deal with here 
   is when the interval contains only one point, 
   and it has the wrong parities - 
   
   in this case we need to look at distance M + 3 
   (the fact that one never needs to look at M + 5 is left as an exercise).

   It was also possible to solve this problem in a number of other ways. 
   
   A pretty standard one was to identify a number 
   of "suspicious points" within a rectangle 
   (the vicinity of (0, 0), 
    the vicinity of the corners, 
	and the vicinity of places 
	where the coordinate axes intersect the edges of the rectangle) 
	and check them all, taking the best solution.



My own comments:
 . . . .
 + + + .   +  max(|x-x0|, |y-y0|) is odd
 + . + .   .  max(|x-x0|, |y-y0|) is even
 + + + .
*/


/* 3
   0 0 #
   0 1 #
   1 0 #  */

void RestorePicture()
{
	int N = 3;
	int X[100] = { 0, 0, 1 };
	int Y[100] = { 0, 1, 0 };
	/* # means max(|xi-x|,|yi-y|) = 2k + 1 odd
	 * . means max(|xi-x|,|yi-y|) = 2k even
	 */
	char C[100] = { '#', '#', '#' };
    /* We have N points (xi, yi) 0<=i<=N-1 
	 * We are looking for a centre point (x, y)
	 * 1. Lets assume that ith point is # we have max(|xi-x|,|yi-y|) = 2k + 1
	 * A parity of xi, yi we know. Lets assume that we know a parity of x, y. So
	 * 1.1 |xi-x|,|yi-y| = 2k+1. Every solution which sutisfies the parity of x and y is fit
	 *                           We miss this case because we need more restricted case.
	 * 1.2 |xi-x|,|yi-y| = 2k. There are no solutions. We also omit this case because we will
	 *                         never find a solution.
	 * 1.3 |xi-x|=2k+1, |yi-y| = 2p. In this case we need |xi-x|>=|yi-y|.
	 * This inequality can be written: x'=xi-x, y'=yi-y
	 * |x|>=|y|                                                Y
	 * x >= 0, y >= 0: x >= y   => y <=              ..y=-x \  |  / y = x ......
	 * x < 0, y < 0  : -x >= -y => y >=x             ....... \ | /..............
	 * x < 0, y >= 0 : -x >= y  => y <= -x           _________\|/____________X
	 * x >= 0,y <= 0 : x >= - y => y >= -x           .......  /|\...............
	 *                                               ....... / | \..............
	 *                                               ......./  |  \...........
	 * |x-x'|>=|y-y'|                                     
     * x >= x', y >= y': x - x' >= y - y' => y <=  x + y' - x'
     * x < x', y < y'  : x' - x >= y'- y =>  y >=  x + y' - x'         
     * x < x', y >= y' : x' - x >= y - y' => y <= -x + y' + x'      
     * x >= x ,y <= y' : x - x' >= y'- y =>  y >= -x + y' + x' 
	 * In this case we move central point (0, 0) to the (x', y') and graph is the same.
	 * We hold y' + x' and y' - x' 
	 */
}

 /*cin >> N;
        X.clear(); X.resize(N);
        Y.clear(); Y.resize(N);
        C.clear(); C.resize(N);
        REP(n,N) {scanf(" %lld %lld %c",&X[n],&Y[n],&C[n]);}

        set<long long> sumset, diffset;
        REP(n,N) { sumset.insert(Y[n]+X[n]); diffset.insert(Y[n]-X[n]); }
        sumset.insert(0);
        diffset.insert(0);*/