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
#include <cstdio>
#include <cstdlib>
#include <algorithm>
using namespace std;

//#define DEBUG

#ifdef DEBUG
#define dprintf(fmt,...) fprintf(stderr,fmt,__VA_ARGS__)
#else
#define dprintf(fmt,...)
#endif

int gcd(int x, int y) {
  x=abs(x),y=abs(y);
  while(y) {
    int t=x%y;x=y;y=t;
  }
  return x;
}

int main() {
  int tc; scanf("%d", &tc);
  for(int tci = 0; tci < tc; tci++) {
    int h,w,d; scanf("%d%d%d", &h, &w, &d);
    int sY=-1,sX=-1;
    static char tbl[40][40];
    for(int y = 0; y < h; y++) {
      scanf(" %s", tbl[y]);
      for(int x = 0; x < w; x++) {
        if(tbl[y][x]=='X') sY=y, sX=x;
      }
    }
    static int vis[101][101];
    for(int y = -d; y <= d; y++) {
      for(int x = -d; x <= d; x++) {
        vis[y+d][x+d] = false;
      }
    }
    int cnt = 0;
    for(int tY = -d; tY <= d; tY++) {
      for(int tX = -d; tX <= d; tX++) {
        if(tY*tY+tX*tX > d*d) continue;
        if(tY==0 && tX==0) continue;
        int yDen = max(1,abs(tX))*2;
        int xDen = max(1,abs(tY))*2;
        int pY = tY>0 ? 1 : tY<0 ? -1 : 0;
        int pX = tX>0 ? 1 : tX<0 ? -1 : 0;
        int cy = (sY*2+1)*yDen/2;
        int cx = (sX*2+1)*xDen/2;
        int rest = max(abs(tY)*yDen, abs(tX)*xDen);
        while(true) {
          dprintf("t=(%d,%d), p=(%d/%d,%d/%d), c=(%d/%d,%d/%d), rest=%d\n",tY,tX,pY,yDen,pX,xDen,cy,yDen,cx,xDen, rest);
          int horiz =
            pY>0 ? (yDen-cy%yDen)%yDen :
            pY<0 ? cy%yDen :
            rest+1;
          int vert =
            pX>0 ? (xDen-cx%xDen)%xDen :
            pX<0 ? cx%xDen :
            rest+1;
          if(horiz==0 && vert==0) {
            int yy = pY>0 ? cy/yDen : cy/yDen-1;
            int yyR = pY>0 ? cy/yDen-1 : cy/yDen;
            int xx = pX>0 ? cx/xDen : cx/xDen-1;
            int xxR = pX>0 ? cx/xDen-1 : cx/xDen;
            if(tbl[yy][xx]!='#') {
              horiz=yDen;
              vert=xDen;
            } else if(tbl[yy][xxR]=='#') {
              if(tbl[yyR][xx]=='#') {
                pY = -pY;
                pX = -pX;
                continue;
              } else {
                pY = -pY;
                continue;
              }
            } else {
              if(tbl[yyR][xx]=='#') {
                pX = -pX;
                continue;
              } else {
                dprintf("fail.\n");
                break;
              }
            }
          }
          if(horiz==0) {
            int yy = pY>0 ? cy/yDen : cy/yDen-1;
            int xx = cx/xDen;
            if(tbl[yy][xx]=='#') {
              pY = -pY;
              continue;
            } else horiz=yDen;
          }
          if(vert==0) {
            int yy = cy/yDen;
            int xx = pX>0 ? cx/xDen : cx/xDen-1;
            if(tbl[yy][xx]=='#') {
              pX = -pX;
              continue;
            } else vert=xDen;
          }
          if(rest==0) {
            if(cy == yDen*(sY*2+1)/2 && cx == xDen*(sX*2+1)/2) {
              // succeed
              int ry = tY/gcd(tX,tY);
              int rx = tX/gcd(tX,tY);
              dprintf("success.\n");
              if(!vis[ry+d][rx+d]) {
                dprintf("added.\n");
                cnt++;
              }
              vis[ry+d][rx+d]=true;
              break;
            } else {
              dprintf("fail.\n");
              break;
            }
          }
          int aug = min(rest,min(horiz,vert));
          rest -= aug;
          cy += pY * aug;
          cx += pX * aug;
        }
      }
    }
    printf("Case #%d: %d\n", tci+1, cnt);
  }
  return 0;
}
