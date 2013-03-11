/* GCJ F 13 4

Problem
 

"I say we must move forward, not backward;
 upward, not forward;
 and always twirling, twirling, twirling towards freedom!"
 — Former U.S. Presidential nominee Kodos. 

After hearing this inspirational quote from America's 
first presidential nominee from the planet Rigel VII, 
you have decided that you too would like to twirl (rotate) towards freedom. 

For the purposes of this problem, you can think of "freedom" as being 
as far away from your starting location as possible.
 
The galaxy is a two-dimensional plane. 
Your space ship starts at the origin, position (0, 0). 
There are N stars in the galaxy. 

Every minute, you can choose a star and rotate your space ship 
90 degrees clockwise around the star. You may also choose to stay where you are.
 
How far away can you move from the origin after M minutes? 

The image illustrates the first 3 rotations for a possible path in sample case 1. 
Note that this path is not necessarily a part of any optimal solution.

Input
 
The first line of the input gives the number of test cases, T. 
T test cases follow, beginning with two lines containing integers N and M. 
The next N lines each contain two integers, Xi and Yi, representing the locations of stars.


Output

 
For each test case, output one line containing "Case #x: D", 
where x is the case number (starting from 1) 
and D is the distance from the origin to the optimal final position. 
Answers with absolute or relative error no larger than 10-6 will be accepted.


The Small Input


On most Google Code Jam problems, 
the small input can be solved without worrying about the running time. 
That was not the case here though. 
Even if N = 10 and M = 10, there are still 1010 different rotation patterns you could try. 
That is a lot!

There are various ways you could try to bring the running time down to a more manageable amount, 
but here is one fact that makes it easy:

•The 1st star you rotate around, 
 the 5th star you rotate around, 
 and the 9th star you rotate around should all be the same.

•The 2nd star you rotate around, 
 the 6th star you rotate around, 
 and the 10th star you rotate around should all be the same.

•The 3rd star you rotate around and the 
 7th star you rotate around should be the same.

•The 4th star you rotate around and the 
 8th star you rotate around should be the same.

 If you realize this, there are only 104 possibilities to try. 
 But why is this fact true? Read on and you'll find out!



 Understanding Rotations



 One of the biggest challenges here is just wrapping your head around the problem. 
 How can you intuitively understand what it means to rotate something 100,000 times? 
 The best way to do this is to roll up your sleeves and write down a couple formulas.
 
 When dealing with rotations and translations of points on a plane, 
 complex numbers provide an excellent notation:

 • A point (x, y) can be represented as x + iy. 
 • Complex numbers can be added and subtracted just like vectors. 
 • Complex numbers can be rotated clockwise 90 degrees about 
   the origin simply by multiplying them by -i.

 It is this last property that makes them so clean to work with, 
 and it is what we will use for the analysis. 
 If you prefer, you could also imagine replacing everything with matrices.
 
 We know that rotating a point P0 by 90 degrees about the origin 
 will send it to -i * P0. 
 However, what happens if we rotate it about a different point Q0? 
 
 There is a standard formula for this situation that you may have already seen. 
 The resulting point P1 satisfies the following:

 P1 - Q0 = -i * (P0 - Q0)

 This is our previous formula applied to the fact 
 that rotating P0 - Q0 by 90 degrees about the origin must give you P1 - Q0. 
 (Do you see why that is true? It's really just a coordinate change.) 
 In our case, it will be helpful to group things a little differently in the formula:
 
 P1 = -iP0 + Q0 * (1 - i)

 Now, suppose we rotate P1 by 90 degrees about another point Q1, 
 then by 90 degrees about another point Q2, and so on. 
 What would happen to this formula? 
 Let's write out a few examples:

 • P2 = -iP1 + Q1 * (1 - i) 
      = -P0 + Q1 * (1 - i) + Q0 * (-1 - i). 

 • P3 = -iP2 + Q2 * (1 - i) 
      = iP0 + Q2 * (1 - i) + Q1 * (-1 - i) + Q0 * (-1 + i).

 • P4 = -iP3 + Q3 * (1 - i) 
      = P0 + Q3 * (1 - i) + Q2 * (-1 - i) + Q1 * (-1 + i) + Q0 * (1 + i).

 • P5 = -iP4 + Q4 * (1 - i) 
      = -iP0 + (Q4 + Q0) * (1 - i) + Q3 * (-1 - i) + Q2 * (-1 + i) + Q1 * (1 + i).
 • etc. 

 From here, it's not too hard to guess the general formula. 
 If the origin is rotated by 90 degrees about Q0, then Q1, and so on, 
 all the way through Qm-1, then the final resulting point Pm is given by:

    (1 - i) * (Qm-1 + Qm-5 + Qm-9 + ...) 
 + (-1 - i) * (Qm-2 + Qm-6 + Qm-10 + ...) 
 + (-1 + i) * (Qm-3 + Qm-7 + Qm-11 + ...) 
 + (1 + i) * (Qm-4 + Qm-8 + Qm-12 + ...)

 And once the formula is written down, it's not too hard to check that it always works. 
 So anyway, is this simpler than the original problem formulation? 

 It may look pretty complicated, but for the most part, 
 you're now just adding points together here, and addition is easier than rotation!



 Pick a Direction



 We want to choose stars Q0, Q1, ..., Qm-1 in such a way 
 as to make the following point as far away from the origin as possible:

   (1 - i) * (Qm-1 + Qm-5 + Qm-9 + ...) 
 + (-1 - i) * (Qm-2 + Qm-6 + Qm-10 + ...) 
 + (-1 + i) * (Qm-3 + Qm-7 + Qm-11 + ...) 
 + (1 + i) * (Qm-4 + Qm-8 + Qm-12 + ...)

 There are different ways of thinking about things from here, but the key is always convex hulls.

 Let X be the point furthest from the origin that we can attain, and more generally, 
 let Xv be the point that is furthest in the direction of v that we can attain. 
 Certainly, X = Xv for some v. 
 
 (This is because X is surely the point that is furthest in the direction of X.) 
 Therefore, it suffices to calculate Xv for all v, 
 and we can then measure which of these is furthest from the origin to get our final answer.

 So how do we calculate Xv for some given v? 
 We want (1 - i) * Qm-1 to be as far as possible in the v direction, 
 or equivalently, we want Qm-1 to be the star that is furthest in the (1 + i)*v direction. 
 Of course, the same thing is true for Qm-5, Qm-9, etc. 
 
 We should choose the same star for all of them. 
 (This is the fact that we used for the small input solution!) 
 Similarly, for Qm-2, Qm-6, etc., we want to choose the star is furthest in the (-1 + i)*v direction. 
 
 In general, we want to choose stars from the original set that 
 are as far as possible in the following directions: (1 + i)*v, (-1 + i)*v, (-1 - i)*v, (1 - i)*v.

 We are now almost done (at least conceptually). 
 First we find the convex hull of the stars and solve for one particular v. 
 Now what happens when we rotate v? 
 
 For a while, nothing will change, but eventually, 
 one of the four directions we are trying to optimize 
 will be perpendicular to an edge of the convex hull, and as a result, 
 the optimal star will switch to the next point on the convex hull. 

 This simulation can be done in constant time at each step, 
 and there will be only O(N) steps since each star choice will rotate once around the convex hull.

 At this point, we are done! 
 We have found every Xv, and we can manually check each of them to see which one is best. 

 The implementation here is tricky, 
 but it is an example of a general technique called rotating calipers. 
 You can find plenty more examples of it online. 
 Or you can always download solutions from our finalists!

 Bonus Question!

 When we generated test data for this problem, 
 we were surprised to find that we could not come up with a case 
 to break solutions which misread "clockwise" as "counter-clockwise" in the problem statement. 
 
 In fact, there is no such case! Can you see why?
*/

#include <math.h>

struct Pr4Point
{
	int x;
	int y;
}

struct Pr4Vector
{
	int x;
	int y;
}

/* Vectors A, B are collinear if A = kB, ax = k*bx, ay = k*by */
int IsCollinear(Pr4Point prevPoint, Pr4Point currPoint, Pr4Point nextPoint)
{
	return (currPoint.x - prevPoint.x)/(currPoint.y - prevPoint.y) ==
		   (nextPoint.x - prevPoint.x)/(nextPoint.y - prevPoint.y);
}
/* Removes all collinear points from polygon */
void RemoveCollPoints(
   struct Pr4Point SourcePoints[], 
   int SN,
   struct Pr4Point DestPoints[],
   int* DN)
{
	int i;
	int isCollinear;
    if ((isCollinear = IsCollinear(SourcePoints[SN - 1], SourcePoints[0], SourcePoints[1])) == 0)
           DestPoints[(*DN)++] = SourcePoints[0];
    for (i = 1; i < SN - 1; i++)
	{
	   if ((isCollinear = IsCollinear(SourcePoints[i - 1], SourcePoints[i], SourcePoints[i + 1])) == 0)
           DestPoints[(*DN)++] = SourcePoints[i];
	}
    if ((isCollinear = IsCollinear(SourcePoints[SN - 2], SourcePoints[SN - 1], SourcePoints[0])) == 0)
           DestPoints[(*DN)++] = SourcePoints[SourcePoints[SN - 1]];
}

int MinimumWidthConvexPolygon(struct Pr4Point ArrPoints[], int N)
{
    struct Pr3Point NewArrPoints[1000];
	int NewN = 0;
	double pi = 4.0 * atan( 1.0 );
	double rotated_angle = 0;
    double min_width = INF;
	/* Caliper A points along the positive x-axis */
	struct Pr4Vector caliper_a = { 1, 0 };
    /* Caliper B points along the negative x-axis */
    struct Pr4Vector caliper_b = { -1, 0 };
	/* index of vertex with minimum y-coordinate */
	int p_a = INF;
    /* index of vertex with maximum y-coordinate */
	int p_b = -INF;

    RemoveCollPoints(ArrPoints, N, NewArrPoints, &NewN);   
    
    
	while (rotated_angle < pi)
	{

	}
}

/*
  Minimum width of a convex polygon
ARRAY points := {P1, P2, ..., PN};
 
 points.delete(middle vertices of any collinear sequence of three points);
  
 REAL p_a := index of vertex with minimum y-coordinate;
 REAL p_b := index of vertex with maximum y-coordinate;
 
 REAL rotated_angle := 0;
 REAL min_width := INFINITY;
 
 VECTOR caliper_a(1,0);    // Caliper A points along the positive x-axis
 VECTOR caliper_b(-1,0);   // Caliper B points along the negative x-axis
 
 WHILE rotated_angle < PI
   
   // Determine the angle between each caliper and the next adjacent edge in the polygon
   VECTOR edge_a(points[p_a + 1].x - points[p_a].x, points[p_a + 1].y - points[p_a].y);
   VECTOR edge_b(points[p_b + 1].x - points[p_b].x, points[p_b + 1].y - points[p_b].y);
   REAL angle_a := angle(edge_a, caliper_a);
   REAL angle_b := angle(edge_b, caliper_b);
   REAL width := 0;
   
   // Rotate the calipers by the smaller of these angles
   caliper_a.rotate(min(angle_a, angle_b));
   caliper_b.rotate(min(angle_a, angle_b));
   
   IF angle_a < angle_b
     p_a++;  // This index should wrap around to the beginning of the array once it hits the end
     width = caliper_a.distance(points[p_b]);
   ELSE
     p_b++;  // This index should wrap around to the beginning of the array once it hits the end
     width = caliper_b.distance(points[p_a]);
   END IF
   
   rotated_angle = rotated_angle + min(angle_a, angle_b);
   
   IF (width < min_width)
     min_width = width;
   
   END IF
 END WHILE
 
 RETURN min_width;

 
 */
