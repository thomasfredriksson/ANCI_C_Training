/*


Problem


You live in a 2-dimensional plane, 
and one of your favourite places to visit is the Hall of Mirrors. 
The Hall of Mirrors is a room (a 2-dimensional room, of course) 
that is laid out in a grid. 

Every square on the grid contains either a square mirror, empty space, or you. 
You have width 0 and height 0, and you are located 
in the exact centre of your grid square.

Despite being very small, 
you can see your reflection when it is reflected back to you exactly. 
For example, consider the following layout, 
where '#' indicates a square mirror that completely fills its square, 
'.' indicates empty space, 
and the capital letter 'X' indicates 
you are in the center of that square:


######
#..X.#
#.#..#
#...##
######

If you look straight up or straight to the right, you will be able to see your reflection.
Unfortunately in the Hall of Mirrors it is very foggy, 
so you can't see further than D units away. 
Suppose D=3. 

If you look up, your reflection will be 1 unit away (0.5 to the mirror, and 0.5 back). 
If you look right, your reflection will be 3 units away (1.5 to the mirror, and 1.5 back), 
and you will be able to see it. 

If you look down, your reflection will be 5 units away and you won't be able to see it.

It's important to understand how light travels in the Hall of Mirrors. 
Light travels in a straight line until it hits a mirror. 

If light hits any part of a mirror but its corner, 
it will be reflected in the normal way: 
it will bounce off with an angle of reflection equal to the angle of incidence. 

If, on the other hand, the light would touch the corner of a mirror, 
the situation is more complicated. 
The following diagrams explain the cases:

  In the following cases, 
  light approaches a corner and is reflected, 
  changing its direction: 

  In the first two cases, 
  light approached two adjacent mirrors at the point where they met. 
  Light was reflected in the same way as 
  if it had hit the middle of a long mirror. 
  
  In the third case, light approached the corners of three adjacent mirrors, 
  and returned in exactly the direction it came from.

In the following cases, light approaches the corners of one or more mirrors, 
but does not bounce, and instead continues in the same direction: 

This happens when light reaches distance 0 from the corner of a mirror, 
but would not have to pass through the mirror 
in order to continue in the same direction. 

In this way, a ray of light can pass 
between two mirrors that are diagonally adjacent to each other 
-- effectively going through a space of size 0. 
Good thing it's of size 0 too, so it fits!


In the final case, light approaches the corner of one mirror and is destroyed: 

The mirror was in the path of the light, 
and the ray of light didn't approach the corners of any other mirrors.

Note that light stops when it hits you, 
but it has to hit the exact centre of your grid square.

How many images of yourself can you see?



Input


The first line of the input gives the number of test cases, T. 
T test cases follow. 
Each test case starts with a line containing 
three space-separated integers, H, W and D. 

H lines follow, and each contains W characters. 
The characters constitute a map of the Hall of Mirrors for that test case, as described above.


Output


For each test case, output one line containing 
"Case #x: y", where x is the case number (starting from 1) 
and y is the number of reflections of yourself you can see.


Limits

1 ≤ T ≤ 100.
3 ≤ H, W ≤ 30.
1 ≤ D ≤ 50.

All characters in each map will be '#', '.', or 'X'.
Exactly one character in each map will be 'X'.

The first row, the last row, the first column 
and the last column of each map will be entirely filled with '#' characters.

Small dataset

There will be no more than 2W+2H-4 '#' characters.

Large dataset

The restriction from the Small dataset does not apply.

Sample


Input 
 	
Output 
 
6
3 3 1
###
#X#
###
3 3 2
###
#X#
###
4 3 8
###
#X#
#.#
###
7 7 4
#######
#.....#
#.....#
#..X..#
#....##
#.....#
#######
5 6 3
######
#..X.#
#.#..#
#...##
######
5 6 10
######
#..X.#
#.#..#
#...##
######

Case #1: 4
Case #2: 8
Case #3: 68
Case #4: 0
Case #5: 2
Case #6: 28


In the first case, light travels exactly distance 1 
if you look directly up, down, left or right.


In the second case, light travels distance 1.414... 
if you look up-right, up-left, down-right or down-left. 


Since light does not travel through you, 
looking directly up only shows you one image of yourself.


In the fifth case, while the nearby mirror is close enough 
to reflect light back to you, 
light that hits the corner of the mirror is destroyed rather than reflected.





To keep things exciting, 
we ended the qualification round with Hall of Mirrors: 
a truly challenging problem. 

It is difficult to see how to get started, 
and then it is even more difficult to actually get the implementation right!


How To Understand Mirrors


The first step in solving this problem is 
reducing the set of all light ray angles 
to something that you can actually check. 

And to do that, it is helpful to use a famous trick for thinking about mirrors. 
Look at the two diagrams below. 
Do you see why they are really illustrating the exact same thing?


In the left picture, 
there is a light ray in a 2x1 room 
that reflects off three mirrors, 
and then ends back where it started. 

Let's focus on where the light ray reflects off the bottom mirror. 
Instead of drawing the reflected light ray, 
let's take the entire room, 
and draw it reflected about this mirror. 

We can then draw the rest of the light ray's path in this picture. 
This is illustrated in the left diagram below:


Next, once the light ray hits the right mirror, 
we can again draw the entire room reflected about that mirror, 
and then continue drawing the rest of the path there. 

Finally, we can do the same thing for the one remaining mirror 
to get the original picture from above. 
This kind of drawing has two really nice properties for us:


  -  The light ray is perfectly straight, 
     and it never bends or turns.

  -  No matter how many reflections we do, 
     and no matter where they are, 
	 the reflected position of your starting point 
	 will always be in the middle of some square.

Together, these imply the key fact you need to get started: 
for a light ray to even have a chance 
of reflecting back to you in distance at most D, 
it must be directed towards the center of a square 
that is distance at most D from you.



The Small Input

If you get this far, you can either try 
to solve just the small input 
or both the small and the large. 
The small input isn't that much smaller really, 
but it is simpler because you cannot have mirrors in the middle of rooms. 
This means that we can take the original room 
and repeatedly reflect it about each 
of the four walls to cover the whole plane. 
This is shown below:



Now let's look at your position in each of these reflected rooms. 
We claim that these give precisely the directions 
you can look in order to see a reflection of yourself. 
Try tracing through a few cases and you will see why.


So to solve the problem, 
you can iterate over all of these positions 
that are within distance D of you, 
and count how many unique directions they give. 

(Remember that the light ray stops once it returns to you, 
 so if two reflections are in precisely the same direction, 
 only one is visible.)


The Large Input

The large input is not any harder conceptually, 
but you will have to do some more implementation. 
The idea is to iterate over all squares within distance D of your starting location, 
and to follow light rays that are directed towards each of these squares. 

In particular, how many of these light rays 
return to the starting position after distance D? 

The challenge of following rays through a 2-dimensional grid is called "ray casting", 
and solving it efficiently was the key to Wolfenstein 3D and other games of that era.

This isn't too hard in theory, but the implementation can get nasty 
if you do not set things up carefully. 

One approach that helps is to first focus on rays 
that are moving more vertically than they are horizontally. 

Then, iterate over each horizontal line the ray touches. 
Between each of these steps, 
it will touch 0 or 1 vertical lines, 
making the processing fairly straightforward (except for corners!).

If you are stuck, try looking through the submitted solutions.



*/




int X, Y, D;
char g[31][31];

void Pr25Solve()
{ 
  int t;
  int T = 10, prob=1;
  for (t = 0; t < T; t++) 
  {
    int y, x, sx, sy, gx, gy; //cin >> Y >> X >> D;
	int ret = 0;
    for (y = 0; y < Y; y++) 
		; //cin >> g[y];
    for (y = 0; y < Y; y++)
    for (x = 0; x < X; x++)
      if (g[y][x] == 'X') 
	  {
        sx = x; 
		sy = y;
      }
    for (gy = sy-D; gy <= sy+D; gy++)
    for (gx = sx-D; gx <= sx+D; gx++) 
	{
	  int fact, x, y, startx, starty, dx, dy, nsteps, i, maxy, maxx;

      if ((gy-sy)*(gy-sy) + (gx-sx)*(gx-sx) > D*D) continue;
      if (gx == sx && gy == sy) continue;
      
	  maxy = (abs(gy-sy) > 1) ? abs(gy-sy) : 1;
	  maxx = (abs(gx-sx) > 1) ? abs(gx-sx) : 1;

	  fact = 2 * maxy * maxx; // 2 * |gy-sy| * |gx-sx|
      x = sx * fact + fact/2, startx = x; // (sz + 1/2) * 2 * |gy-sy| * |gx-sx| = (sz + 1/2) * fact
      y = sy * fact + fact/2, starty = y; // (sy + 1/2) * 2 * |gy-sy| * |gx-sx| = (sy + 1/2) * fact
      dy = gy-sy, dx = gx-sx;
      nsteps = fact;
	  //cout << "Trying " << gx << ',' << gy << " " << dx << ',' << dy << endl;
      for (i = 0; i < nsteps; i++) 
	  {
        if (i && x == startx && y == starty) goto fail;
        //int odx = dx, ody = dy;
		/* We need some explanation how we go along a line in grid with cell side 1
		 * |gy-sy| = yy, |gx-sx| = xx  fact = 2yyxx
		 * i = 0. y = ((sx + 0.5)2yyxx + yy) / 2yyxx = sz + 0.5 + yy/2yyxx = sx + 0.5(1 + 1/xx) = 
		 * sx + (xx + 1) / 2xx
		 * 
		 * y - y0 - 0.5) = (yy/xx)(x - x0 - 0.5) - line. 
		 * Vertical line = x = x0 + 0.5 + 0.5, 
		 * 
		 *  (y - y0 - 0.5) = (yy/xx)(x - x0 - 0.5) - line. 
		 *  Vertical line:   x = x0 + 0.5 + 0.5, x0 + 1, x0 + 2, ... x0 + k,...
         *  Horizontal line: y = y0 + 1, y0 + 2, y0 + 3, ..., y0 + k,...
         *  y = y0 + 0.5 + (yy/xx)(x - x0 - 0.5), x = x0 + 1, y = y0 + 0.5 + yy/2xx
         *      y0 + 0.5 + (yy/xx)(x - x0 - 0.5), x = x0 + 2, 
         *    = y0 + 0.5 + (yy/xx)(x0 + 2 - x0 - 0.5) = y0 + 0.5 + (yy/xx)(3/2) = 3
		 *
		 *  we have grid (n,m) and line from (0,0) to (n,m)
		 *  and we need to find the shortest triangle tga = m/n ctga = n/m
		 *  assume that tga < 1 a < 45
		 *  then the first intersection = (0,0)->(1, m/n)
		 *  the second intersection = (1, m/n) -> (1 + (1-m/n)n/m, 1) = (n/m, 1)
		 *  dx, dy
		 *  1. dx = 1,                  dy = m/n
		 *  2. dx = (1-m/n)n/m          dy = 1-m/n
		 *     dx = n/m - 1             1
		 *  3. dx = 1 - (n/m - 1)       dy = 
		 *        = 2 - n/m             dy = (2 - n/m)m/n = 2m/n - 1
		 *  4. dx = 2(1-m/n)n/m         dy = 1 - (2m/n - 1) = 2 - 2m/n
		 *          2(n/m-1)
		 *  5. dx = 1 - 2(n/m - 1)      dy = (3-2n/m)m/n=3m/n-2
		 *          3 - 2n/m
		 *  6.                          dy = 1 - (3m/n-2) = 3(1-m/n)                        
		 *  
		 *   k - (k-1)n/m               k - km/n
		 *   (mk - (k-1)n)/m            (kn - km)/n
         *  
		 *  m = 4/5
		 *  1. 1                   4/5
		 *  2. 5/4 - 1 = 1/4       1/5
		 *  3. 2 - 5/4 = 3/4       2*4/5 - 1 = 3/5
		 *  4. 2(5/4-1)= 1/2       2(1-4/5) = 2/5
		 *  5. 
		 */
        y += dy; x += dx;
		if (y % fact == 0 && x % fact == 0) 
		{
           // Hit a corner
           int curx = x/fact, cury = y/fact;
           char cur1 = g[cury - (dy > 0)][curx - (dx > 0)];
           char cur2 = g[cury - (dy > 0)][curx - (dx < 0)];
           char cur3 = g[cury - (dy < 0)][curx - (dx < 0)];
           char cur4 = g[cury - (dy < 0)][curx - (dx > 0)];
		   if (cur1 == '#') 
              ; // cerr << "ERROR!" << endl;
		   if (cur3 == '#' && cur2 == '#' && cur4 == '#') 
		   {
              dx = -dx; dy = -dy;
           } else if (cur3 == '#' && cur4 == '#') 
		   {
              dy = -dy;
           } else if (cur3 == '#' && cur2 == '#') 
		   {
              dx = -dx;
           } else if (cur3 == '#') 
		   {
              goto fail;
           }
		}
		else if (y % fact == 0) 
		{
          // Hit a horizontal edge
          int curx = x/fact, cury = y/fact;
          char cur1 = g[cury - (dy > 0)][curx];
          char cur2 = g[cury - (dy < 0)][curx];
          if (cur1 == '#')
            ; //cerr << "ERROR!" << endl;
          if (cur2 == '#') dy = -dy;
        } 
		else if (x % fact == 0) 
		{
          // Hit a vertical edge
          int curx = x/fact, cury = y/fact;
          char cur1 = g[cury][curx - (dx > 0)];
          char cur2 = g[cury][curx - (dx < 0)];
          if (cur1 == '#')
            ; //cerr << "ERROR!" << endl;
          if (cur2 == '#') dx = -dx;
        }
fail:
		;
	}
  }
}
}


