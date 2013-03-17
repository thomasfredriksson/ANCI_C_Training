/* GCJ F 13 5


Problem

You have been walking in the woods for hours, and you want to go home.


The woods contain N clearings labeled 1, 2, ..., N. 
You are now at clearing 1, and you must reach clearing N in order to leave the woods. 
Each clearing from 1 to N-1 has a left path and a right path leading out to other clearings,
as well as some number of one-way paths leading in. 

Unfortunately, the woods are haunted, and any time you enter a clearing, 
one of the two outgoing paths will be blocked by shifty trees. 

More precisely, on your kth visit to any single clearing:

You must leave along the left path if k is odd.
You must leave along the right path if k is even.
All paths are one-way, so you have no choice at each step: 
you must go forward through the one unblocked outgoing path.

So the first time you are in clearing #1, 
you will leave along the left path. 

If you ever come back to clearing #1 for a second time, 
you would leave along the right path; 
the third time, you'd leave along the left path again; and so on.

You begin at clearing #1, 
and when you get to clearing #N, 
you can leave the woods. 

How many paths do you need to follow before you get out?

Input

The first line of the input gives the number of test cases, T. 
T test cases follow, each beginning with a line containing a single integer N.

N-1 lines follow, each containing two integers Li and Ri. 
Here, Li represents the clearing you would end up at 
if you follow the left path out of clearing i, 
and Ri represents the clearing you would end up at 
if you follow the right path out of clearing i.


No paths are specified for clearing N 
because once you get there, you are finished.


Output

For each test case, output one line containing "Case #x: y", 
where x is the case number (starting from 1) 
and y is the number of paths you need to follow to get to clearing N. 
If you will never get to clearing N, output "Infinity" instead.

Limits

1 ≤ T ≤ 30.
1 ≤ Li, Ri ≤ N for all i.
Small dataset

2 ≤ N ≤ 10.

Large dataset

2 ≤ N ≤ 40.

Sample


Input 
 	
Output 
 
2
4
2 1
3 1
2 4
3
2 2
1 2
Case #1: 8
Case #2: Infinity

Sample Explanation

In the first sample case, your route through the woods will be as shown below:

Paths followed	Clearing	Path direction

0	1	Left
1	2	Left
2	3	Left
3	2	Right
4	1	Right
5	1	Left
6	2	Left
7	3	Right
8	4	-



For the small dataset, 
we can directly simulate the process of walking through the forest. 
How many steps can this take? 

There are 2^10 states the trees in the clearings can be in, 
and 10 possible states for the clearing we are standing in. 

So if we reach the final clearing, 
it can't take more than 10 × 2^10 steps, 
and if we take that many steps without reaching 
the final clearing we know the answer is Infinity.

For the large dataset, 40 × 2^40 steps 
is too many to simulate directly, but can an input case approach this many?

A test case can be designed to make the clearings simulate an N-1 bit counter. 

The first clearing has both paths leading to the second clearing. 

Each clearing after that forms a chain where one path leads to the next clearing,
and one path leads back to the first clearing. 

Whenever the trail leads back to the first clearing,
the states of the clearings give the next N-1 bit number, 
and after all of those numbers have been produced, it can reach the final clearing. 

This will take at least 2^39 steps.



So we need a solution that will not simulate every step individually. 

In the previous example, we spend 2^21-2 steps in the first 
20 clearings between each time we visit any of the last 20 clearings. 

If our program could detect that this always happens, 
it could simulate those 2^21-2 steps without having to do them all individually. 
We will only take 2^20-3 steps from any of the second twenty clearings,
so the total runtime would be reasonable.

An implementation of this approach is to split the clearings 
into two sets A and B of roughly equal size. 

We then use dynamic programming to compute, 

for each location in A and each of the 2^|A| states of the clearings in A, 
how many steps it will take to leave A, 
which clearing in B we will be leaving towards, 
and what state the clearings in A will be left in.


After this DP is done, o
ur solution will take an amount of time proportional 
to the number of steps we take from clearings in B 
-- for each of those, we simulate that step, 
and if that takes us to a clearing in A, 
we look up the appropriate result of the DP from a table. 

So to make this solution efficient, 
we need to choose A and B so that only 
a small number of steps are taken from clearings in B.


Find the distance from each clearing to the final clearing, 
where distance is defined as the number of paths you need to take, 
assuming the state of the clearings is optimal. 

If the final clearing cannot be reached from some clearings, 
put those in a separate set. 
If we ever reach one of those, the answer is Infinity.


Choose B to be the closest half of the remaining clearings. 
There can be at most B × 2|B| steps taken from clearings in B, 
because we cannot repeat a state of B without reaching the final clearing.

This can be only at most 20 × 2^20, so this solution is sufficiently fast.



*/

/* The number of clearings in the wood */
#define N 4
#define INF -666

void ZeroStates(int States[][], int rowCount, int columnCount)
{
   int i, j;
   
   for (i = 0; i < rowCount; i++)
   {
	   for (j = 0; j < columnCount; j++)
	   {
		   States[i][j] = 0;
	   }
   }
}

void SolveSmall()
{ 
   /* This array contains left paths for each point */
   int Left[N] = { 2, 3, 2, -666 };
   /* This array contains right paths for each point */
   int Right[N] = { 1, 1, 4, -666 };
   /* Snapshot description.
    * The state of the forest can be a man's current clearing, where he is 
	* standing in and the number of thye man's visits for every clearings.
	* Bacause the next path depends only on a parity of the number the man has visited 
	* a clearing it is enough to keep only the parity: 1 is odd, 0 is even.
	* We have at most 2^N possible variants: (2^(2N))/(2*...*2 N times)
	*  000...0  till  111...11
	* If we have ai = 1 on ith position the means that a number we have visited this clearing
	* so far is odd, otherwise if we have ai = 0, this number is even.
	* States[i][j] can be 
	*      1(we have already visited this state) 
	*   or 0 (we have not visited it yet)
    */ 
   int States[1<<N][N];
   int currentClear;
   int currentState;
   int currentBit;
   int steps = 0;
   /* Fill in all states with 0 */
   ZeroStates(States, 1<<N, N);
   /* start from clearing 0 */
   currentClear = 0;
   /* start state is 0 (000....00)N times. This means that 
    * number of visits for each clearings is odd (0 from start)
    */
   currentState = 0;
   /* We exit from cycle when we meet the last N-th clearing or
    * when we get into the state we have already visited.
	* In this case we would start spinning and the time is infinity
    */
   while (1 == 1)
   {
	  /* Check if we have reached the last point */
      if (currentClear == N - 1)
	  {
		  break;
	  }
	  /* If we have already been in some state we break with infinity */
      if (States[currentState][currentClear] == 1)
	  {
		  steps = INF;
          break;
	  }
      /* if we have currentState in bit representation: 1001001..011
	   * and we need to find bit value on the currentClear position from 
	   * the start, (N - start + 1) th position from the end
	   * start = currentClear + 1 so (N - currentClear)-th position from 
	   * currentState >> N - currentClear - 1 moves (N - currentClear)-th
	   * to the first position and after applying &1 we can get the bit value
	   */ 
      currentBit = (currentState >> N - currentClear - 1) & 1;
	  /* After reading this bit we need to change its value to opposite 
	   * 1 << N - currentClear - 1 is the number 000100.000 On (N - currentClear - 1)th
	   * we have 1
	   */
      currentState ^= 1 << N - currentClear - 1;
	  /* Move to the next state */
	  currentClear = (currentBit == 1) 
		  ? Left[currentClear] - 1 
		  : Right[currentClear] - 1;

	  steps++;
      States[currentState][currentClear] = 1;
   }
}

/* The number of clearings in the wood for large solution */
#define NL 10
/* This struct describes a forest state in large solution */
struct StateLarge
{
   int clearing;
   int state;
   int visited;
   int stepsToGetOut;
};
void ZeroStatesLarge(struct StateLarge States[][], int rowCount, int columnCount)
{
   int i, j;
   
   for (i = 0; i < rowCount; i++)
   {
	   for (j = 0; j < columnCount; j++)
	   {
		   States[i][j].clearing = j;
		   States[i][j].state = i;
		   States[i][j].stepsToGetOut = 0;
		   States[i][j].visited = 0;
	   }
   }
}
/* Using dynamic programming model we divide the ser of clearing into 2 sets:
 * A and B. For example if we have 0, 1, 2, ... , NL-2, NL-1, NL clearings we 
 * have: 
 * A = { 0, 1, 2, ... , [NL/2] }
 * B = { [NL/2] + 1, [NL/2] + 2, ... , NL }
 * medium = [NL/2]
 */
void SolveLarge()
{
   /* This array contains left paths for each point */
   int Left[NL] = { 2, 9, 9, 9, 2, 9, 9, 9, 2, -666 };
   /* This array contains right paths for each point */
   int Right[NL] = { 10, 9, 6, 5, 1, 7, 4, 3, 8, -666 };
   /* This is a boundary between A and B sets */
   int medium = NL>>1;
   /* For the set A we have |A|*2^|A| different states. And we need to find 
    * a number of steps to get out of the set A or find out if we go 
	* spinning. In this case we have found a solution and it is INFINITY.
    */
   struct StateLarge StatesA[1<<(NL>>1)][NL>>1];
   /* Fill in all states with 0 */
   ZeroStatesLarge(StatesA, 1<<(NL>>1), NL>>1);
   /* We start moving from the clearing 0 */
   int currentClearA = 0;
   /* We start moving from zero state */
   int currentStateA = 0;
   /* What do we need to achieve. We need for each state in A find the number
    * of steps we need to get out of the set A, a clearing where we leave it in,
	* and a clearing from the set B where we leave towards. 
    */
   
}


