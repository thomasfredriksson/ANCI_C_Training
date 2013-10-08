
/*
 
 Problem

 Fair King Tyrone and his four sons conquered the nation of Carrania. 
 His four sons immediately started to squabble about 
 dividing the land between the four of them. 
 
 The main point of contention was the gold mines of Carrania 
 - each son wanted to have no fewer gold mines than any other.

 Fair King Tyrone soon got tired of the squabbling, 
 especially when he learned the number of mines is 4N, 
 so dividing them should be easy. 
 
 He gathered his sons, took a map, drew an X on it 
 and declared each son would get one quarter of the nation, 
 with borders defined by the X he drew.

 Unfortunately, Fair King Tyrone is a bit shortsighted, 
 and the map he drew on was not a map of Carrania. 
 His first minister quickly hid the map, 
 and now tries to draw an identical X on the map of Carrania 
 so that each son gets the same number of gold mines. 
 
 Unfortunately all sons saw King Tyrone draw the X, 
 and know the borders should be two perpendicular straight lines 
 - so the minister has to make them so.

 Help him! 
 Your task is to draw two perpendicular straight lines 
 such that no gold mine lies on a border, 
 and the borders divide the gold mines equally.


 Input


 The first line of the input gives the number of test cases, T. 
 T test cases follow. 
 Each test case begins with a number N, 
 describing the number of gold mines each son should get. 
 
 4N lines follow, each containing two integers, 
 being the coordinates xi, yi of one of the gold mines. 
 No three gold mines are co-linear.


 Output


 For each test case, output one line containing "Case #x: xa ya xb yb", 
 where x is the case number (starting from 1), 
 (xa, ya) are the coordinates of the point 
 where the two borders intersect, 
 and (xb, yb) are the coordinates of some other point on the X.

 All coordinates must be between -109 and 109, 
 have at most 9 digits after the decimal point, 
 and not use exponential notation. 
 
 They must be exact: 
 the resulting X will be drawn exactly at these coordinates. 
 You should output IMPOSSIBLE instead if there is no good placement of borders.



 Median choice

 Since the four quarters formed 
 by the two lines have to contain 
 the same number of mines, 
 each of the two lines has to split the set in half. 
 
 Thus, if we know the inclination of the two lines 
 (given as, e.g., the directed angle the first line forms with the horizontal axis), 
 we can position the each line in any place 
 such that it splits the points in half 
 
 (by, for instance, sorting the points by the value 
  of the cross-product with the direction of the line). 
 
 Let's begin by choosing any angle alpha as our initial angle, 
 and draw the two lines according to the procedure above.

 
 Suppose that the first quarter contains X points. 
 The second quarter contains 2N - X, 
 since there have to be 2N points above the red line. 
 
 The third quarter will again contain X points 
 (because there are 2N points to the right of the green line), 
 and the fourth will contain 2N - X. 
 
 So if X = N, our two lines are a correct solution. 
 This doesn't need to be the case, 
 though, as we can see on the figure above.


 Rotating the lines


 If the angle alpha we chose happens not to be a correct solution to the problem, 
 we will try rotating the lines (by increasing alpha) until it becomes valid.

 Let's consider what happens when we rotate the lines. 
 At some moment, one or both of the lines will rotate to a point 
 where instead of splitting the set neatly in half, 
 the "splitting line" passes through two mines, 
 
 with 2N-1 mines on either side 
 (note that we are taking advantage of the fact 
 that no three points are collinear, 
 so we know the line passes through exactly two points, and not, say, four). 
 
 We will call the moment at which there are two points 
 on at least one of the splitting lines a "discontinuity". 
 
 After we rotate a tiny bit more, the lines split the set neatly again.

 We will prove in a moment that at any discontinuity, 
 X changes at most by one 
 (it can also happen to stay the same).

 Notice, however, that when we increase alpha by 90 degrees, 
 the red and green lines will exchange places, 
 which means that the first quarter 
 (which also rotated by 90 degrees) 
 now contains 2N-X points. 
 
 Thus, somewhere in between X had to be exactly equal to N!
 


 Discontinuity


 Let's analyze what happened after the discontinuity. 
 Obviously, only the points that were on the dividing lines 
 could have changed quarters at the discontinuity. 
 
 One of the points that were on the red line 
 crosses from one of quarters (1, 2) to one of quarters (4, 3), 
 
 and the other point crosses in the other direction. 
 Thus, if the discontinuity had points on one line only, 
 X changes by at most one.

 We will see that even if there were points on both lines at the discontinuity, 
 X will still change only by one. 
 The points on the red line go from quarters (1, 4) to quarters (2, 3). 
 
 So, for X to, say, grow by two, 
 we would have to have a point from 2 go to 3 and point from 4 go to 1 on the red line, 
 while a point from 4 went to 3 and a point from 2 went to 1 on the green line. 

 However, the red line 
 (and the green line as well, 
  but for now it's the red line that matters) 
  is rotating clockwise 
  
  - thus, it's the more leftward point that will go down, 
  and the more rightward point that will go up 
  — so the situation described above is impossible.



  Finding the solution

  We can now use binary search to find a solution to the problem. 
  Begin with an arbitrary angle alpha as the left bound, 
  and alpha + 90 degrees as the right bound, 
  assume that for the left bound X is smaller than N 
  
  (if it's equal, we're done, and if it's larger, 
   take alpha + 90 as left and alpha + 180 as right). 
   
  We know somewhere between the two there is a point in which X = N. 
  So we pick an angle midway between left and right, 
  and check how big is X for this median angle. 
  
  If it's equal to N, we are done. 
  If it's smaller, the median angle is our new left, 
  if it's larger, it's the new right. 
  Proceed until success.

  A single iteration, 
  with the standard implementation, 
  takes O(N logN) time 
  - sort the points twice, 
    assign each to the appropriate quarter, find X. 
	
  If somebody cared enough 
  (one doesn't need to in this problem) 
  it can be done in O(N) time, 
  using a faster algorithm to find the median values 
  (either a randomized one, like quicksort, 
   but recursing only into the bigger of two halves, 
   or even deterministically with the median of medians algorithm.





The key question is "how many iterations of the binary search algorithm will we need to find the angle we are looking for?". This depends on the size of the interval of angles that we will try to hit. This interval will occupy the space between some two discontinuities, and each discontinuity is defined by a line connecting two of the input points - thus, the size of the interval can be expressed as an angle between two lines, each crossing two points with integral coordinates no larger than 10-6. Such an angle can be on the order of 10-12, which means we will need roughly 40 steps of the binary search. Thus, our algorithm will easily run in time.



Median of mediam


In computer science, 
the median of medians algorithm 
is a selection algorithm based on the quickselect algorithm, 
and is optimal, having worst-case linear time complexity 
for selecting the kth largest element. 

The algorithm consists of an algorithm 
to find an approximate median in linear time 
– this is the key step – which is then used as a pivot in quickselect. 

In other words, it uses an (asymptotically) 
optimal approximate median-selection algorithm 
to build an (asymptotically) optimal general selection algorithm.

The approximate median-selection algorithm 
can also be used as a pivot strategy in quicksort, 
yielding an optimal algorithm, 
with worst-case complexity O(n log n). 

Although this approach optimizes quite well, 
it is typically outperformed in practice 
by instead choosing random pivots, 
which has almost certain linear time for selection 
and almost certain linearithmic time for sorting, 
and avoids the overhead of computing the pivot.

[citation needed] 
Median of medians is used in the hybrid introselect algorithm as a fallback, 
to ensure worst-case linear performance: 
introselect starts with quickselect, 
to obtain good average performance, 
and then falls back to median of medians if progress is too slow.

The algorithm was published in Blum et al. (Tarjan), 
and thus is sometimes called BFPRT after the last names of the authors. 
In the original paper the algorithm was referred to as PICK, 
referring to quickselect as "FIND".



Outline[edit]


Quickselect is linear-time on average, 
but it can require quadratic time with poor pivot choices. 
This is because quickselect is a decrease and conquer algorithm, 
with each step taking O(n) time in the size of the remaining search set. 

If the search set decreases exponentially quickly in size 
(by a fixed proportion), 
this yields a geometric series times the O(n) 
factor of a single step, and thus linear overall time. 

However, if the search set decreases slowly in size, 
such as linearly (by a fixed number of elements, 
in the worst case only reducing by one element each time), 
then a linear sum of linear steps yields quadratic overall time 
(formally, triangular numbers grow quadratically). 

For example, the worst case occurs when pivoting 
on the smallest element at each step, 
such as applying quickselect for the maximum element 
to already sorted data and taking the first element as pivot each time.

If one instead consistently chooses "good" pivots, 
this is avoided and one always gets linear performance even in the worst case. 
A "good" pivot is one for which we can establish 
that a constant proportion of elements 
fall both below and above it, 
as then the search set decreases at least by 
a constant proportion at each step, 
hence exponentially quickly, and the overall time remains linear. 

The median is a good pivot 
– the best for sorting, 
and the best overall choice for selection 
– decreasing the search set by half at each step. 

Thus if one can compute the median in linear time, 
this only adds linear time to each step, 
and thus the overall complexity of the algorithm remains linear.

The median-of-medians algorithm does not actually compute the exact median, 
but computes an approximate median, 
namely a point
that is guaranteed to be between the 30th and 70th percentiles 
(in the middle 4 deciles). 

Thus the search set decreases by a fixed proportion at each step, 
namely at least 30% 
(so at most 70% left). 
Lastly, the overhead of computing the pivot consists 
of recursing in a set of size 20% the size of the original search set, 
plus a linear factor, 
so at linear cost at each step, 
the problem is reduced to 90% (20% and 70%) the original size, 
which is a fixed proportion smaller, 
and thus by induction the overall complexity is linear in size.



Algorithm[edit]

The Select algorithm divides the list 
into groups of five elements. 
(Left over elements are ignored for now.) 

Then, for each group of five, 
the median is calculated 
(an operation that can potentially be made very fast 
if the five values can be loaded into registers and compared). 

(If sorting in-place, 
 then these medians are moved into one contiguous block in the list.) 
 Select is then called recursively on this sublist of n/5 elements 
 to find their true median. 
 
 Finally, the "median of medians" is chosen to be the pivot.


  //returns the index of the median of medians.
  //requires a variant of select, "selectIdx"
  //which returns the index of the selected item rather than the value

  function medianOfMedians(list, left, right)
  {
     numMedians = (right - left) / 5
     for i from 0 to numMedians
	 {
         //get the median of the five-element subgroup
         subLeft  := left + i*5
         subRight  := subLeft + 4
         if (subRight > right) 
		     subRight  := right

         //alternatively, use a faster method that works on lists of size 5
         medianIdx  := selectIdx(list, subLeft, subRight, (subRight - subLeft) / 2)

         //move the median to a contiguous block at the beginning of the list
         swap list[left+i] and list[medianIdx]

         //select the median from the contiguous block
         return selectIdx(list, left, left + numMedians, numMedians / 2)
     } 
  }


  We need to describe the problem:

  We have a set A, a set of elements, which consists of n different integer numbers.
  For some i: 1 <= i <= n
  We need to find an element x from A: x i more than other (i - 1) elements from A.
  

  One of possible solutions is sort all elements by increasing order and after that 
  take i-th element.
  It is possible to solve this problem for O(n) 


  Partition(A,p,q)
  {
     x = A[q];
	 i = p - 1;

     for j = p to q - 1
     do if A[j] <= x
        then i = i + 1
		     Swap (A[i], A[j]);

     i = i+1
     Swap (A[i], A[q]);
     return i
  }

  Randomized_Partition(A,p,q)
  {
     i = Random(p,q);  
	 Swap(A[i], A[q]);
     return Partition(A,p,q);
  }
  
  Randomized_Select(A,p,q,k)
  {
     if (p == q)
        return A[p];

	 r = Randomized_Partition(A,p,q);
     t = r - p + 1;

     if (k == t)
        return A[r];
     else if (k < t)
        return Randomized_Select(A,p,r-1,k);
	 else 
        return Randomized_Select(A,r+1,q,k - t);
  }

  We need to calculate the complexity of this algorithm. Suppose taht on every step we 
  have an equal partitioning:

  Complexity = O(n) + O(n/2) + O(n/4) + 

 */