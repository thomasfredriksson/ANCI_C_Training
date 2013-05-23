/*

  
  Problem

  I have a set of positive integers S. 
  Can you find two non-empty, distinct subsets with the same sum?

  Note: 
  A subset is a set that contains only elements from S, 
  and two subsets are distinct 
  if they do not have exactly the same elements.

  Input

  The first line of the input gives the number of test cases, T. 
  T test cases follow, one per line. 
  Each test case begins with N, 
  the number of positive integers in S. 
  It is followed by N distinct positive integers, all on the same line.

  Output

  For each test case, first output one line containing "Case #x:", 
  where x is the case number (starting from 1).

  If there are two different subsets of S that have the same sum, 
  then output these subsets, one per line. 
  Each line should contain the numbers in one subset, separated by spaces.

  If it is impossible, then you should output the string "Impossible" on a single line.

  If there are multiple ways of choosing two subsets with the same sum, any choice is acceptable.

  Limits

  No two numbers in S will be equal.
  1 ≤ T ≤ 10.

  Small dataset

  N is exactly equal to 20.
  Each number in S will be a positive integer less than 105.

  Large dataset

  N is exactly equal to 500.
  Each number in S will be a positive integer less than 1012.

  Sample

  Input
  2
  20 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20
  20 120 266 858 1243 1657 1771 2328 2490 2665 2894 3117 4210 4454 4943 5690 6170 7048 7125 9512 9600


  Output

  Case #1:
  1 2
  3

  Case #2:
  3117 4210 4943
  2328 2894 7048



  The Small Input

  If you want to forget about the large input 
  and go straight for the small, 
  then Equal Sums might look like a classic dynamic programming problem. 
  Here is a sketch of one possible solution in Python:


  def GetSetWithSum(x, target):
      if target == 0: return []
      return GetSetWithSum(x, target - x[target]) + [x[target]]

  def FindEqualSumSubsets(S):
     x = [0] + [None] * (50000 * 20)
     for s in S:
        for base_sum in xrange(50000 * 20, -1, -1):
          if x[base_sum] is not None:
             if x[base_sum + s] is None:
                x[base_sum + s] = s
             else:
                subset1 = GetSetWithSum(x, base_sum + s)
                subset2 = GetSetWithSum(x, base_sum) + [s]
          return subset1, subset2
  return None

  The idea is that, in x, 
  we store a way of obtaining every possible subset sum. 
  If we reach a sum in two different ways, 
  then we can construct two subsets with the same sum.

  For the small input, this approach should work fine. 
  However, x would have to have size 500 * 1012 for the large input. 
  That is too big to work with, and you will need a different approach there.
  


  The Large Input

  The first step towards solving the large input 
  is realizing that the statement is very misleading! 
  
  Let's suppose you have just 50 integers less than 10^12. 
  There are 2^50 ways of choosing a subset of them, 
  and the sum of the integers in any such subset is at most 50 * 10^12 < 2^50. 
  
  By the Pigeonhole Principle, this means that 
  there are two different subsets with the same sum.

  So what does that mean for this problem? 
  Well, even if you had just 50 integers to choose from instead of 500, 
  the answer would never be "Impossible". 
  
  You need to find some pair of subsets that have the same sum, 
  and there is a lot of slack to work with. 
  The trick is figuring out how to take advantage of that slack.

  The Birthday "Paradox" Method

  The simplest solution is based on a classic mathematical puzzle: the birthday paradox.

  The birthday paradox says that if you have just 23 people in a room, 
  then some two of them probably have the same birthday. 
  This can be surprising because there are 365 possible days, 
  and 23 is much smaller than 365. 
  
  But it is true! 

  One good way to look at is this: 
  
  We have n people and how many different pairs of people can be chosen 
  We choose the first people and we have (n - 1) candidates to make pair.
  The first pair can be chosen n(n - 1) variants
  (n - 2) people left. The second pair can be chosen by (n - 3) variants

  6 people: Pair = [LeftPeople][RightPeople] 23 * 22 / 2 - we do not take order into account

  there are 23 choose 2 = 253 pairs of people, 

  and each pair of people has a 1/365 chance of having the same birthday. 

  In particular, the expected (aka average) number of pairs of people 
  with the same birthday is 253 / 365 = 0.693... 
  
  Once you write it that way, it is not too surprising that 
  the probability of having at least one pair matching is about 0.5.

  It turns out this exact reasoning applies to the Equals Sums problem just as well. 
  Here is a simple algorithm:

  Choose 6 random integers from S, add them up, and store the result in a hash set. 
  (Why 6? We'll come back to that later...)

  Repeat until two sets of 6 integers have the same sum, then stop.

  After choosing t sets, there will be t choose 2 pairs, 
  and each set will have sum at most 6 * 10^12. 
  
  Choose a random 6 numbers from the set - the max number value is 10^12.
  The max value for the sum is 6 * 10^12.

  sum(set(1)), sum(set(2)), ..., sum(set(t).)
  How many different pairs  t^2. 

  Therefore, the expected number of collisions would be approximately t^2 / ( 2 * 6 * 10^12). 

  When t is around 10^6, this expectation will be near 1, 
  and the reasoning from the birthday paradox says that we'll likely have our collision.

  That's it! Since we can quickly generate 10^6 small subsets and put their sums into a hash-set, 
  this simple algorithm will absolutely solve the problem.
  You may be worried about the randomness, but for this problem at least, you should not be. 
  This algorithm is extremely reliable. 
  By the way, this method will work on the small input as well, 
  so you don't need to do the dynamic programming solution if you do not want to.

  Note: There are many similar approaches here that can work. 
  For example, in our internal tests, one person solved the problem by only focusing on the first 50 integers, 
  and trying random subsets from there. 
  The proof below works for this algorithm, as well as many others.
 
*/

public Pr20SmallSolve()
{
	int n, i;
	int s[100];
	int mark[1000000];
    int bitMask;
	int found;

    memset(mark, 0, sizeof(mark));
 
	int m1, m2, found;
    found = 0;

	/* We need the list of all possible bitMasks
	 * n = 5
	 * bitMask: 00001
	 * bitMask: 00010
	 * bitMask: 00011
	 * bitMask: 00100
	 * bitMask: 00110
	 * bitMask: 00111
	 * ....
	 * bitMask: 10000
	 * bitMask: 11000
	 * ....
	 * bitMask: 11111
	 */
	for (bitMask = 1; bitMask < (1 << n) && !found; bitMask++)
	{
        int sum = 0;
	    for (i = 0; i < n; i++)
		   if (bitMask & (1 << i)) sum += s[i];    

		if (mark[sum] == 0)
			mark[sum] = bitMask;
		else
		{
			m1 = mark[sum];
			m2 = bitMask;
			found = 1;
		}
	}
}





