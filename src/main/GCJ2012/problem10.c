/*
 
 Problem

 Ashish has forgotten his password. 
 He remembers that he used the following algorithm to create his password: 
 Ashish took up to k consecutive words from a passage of text, 
 and took the first letter from each word. 
 Then, he might have changed some of the letters to their "l33tspeak" equivalents. 
 Specifically, he might have changed 
 "o" to "0", 
 "i" to "1", 
 "e" to "3", 
 "a" to "4", 
 "s" to "5", 
 "t" to "7",
 "b" to "8" 
 and/or 
 "g" to "9".

 For example, if Ashish took his password 
 from the first sentence of 
 The Fellowship of the Ring -- 

 "This book is largely concerned with Hobbits, 
  and from its pages a reader may discover much
  of their character and a little of their history" 
  
  -- 
  
  Ashish would have reduced that to "tbilcwhafiparmdmotcaaloth". 
  Then the password might be "tbilcwh", "7b1lcwh4f", "a", "4", or "4al07h", etc.

  Ashish has a special extension installed in his browser 
  that will prevent his computer from uploading any string that contains his password. 
  In order to figure out which passage of text he took his password from,
  Ashish has created a webpage to take advantage of this extension. 
  Every second, the webpage will tell the browser to post a "password string" 
  for a new passage of text: 
  a string that contains all of the possible passwords 
  that Ashish could have chosen from that passage of text. 
  As soon as his browser fails to post such a string, 
  Ashish will know where he took his password from.

  For example, if k = 2 and the passage of text contains words 
  starting with the letters "google", 
  then one password string for that passage is "goo0og00gle9o909l3". 
  All substrings of length ≤ 2 from the original string, 
  and all of their l33tspeak equivalents, are contained in the new string.

   Given the first letters of the words in a passage of text, 
   what is the minimum number of characters in the "password string" of that passage?

   Input

   The first line of the input gives the number of test cases, T. 
   T test cases follow. Each test case consists of two lines. 
   The first line contains the integer k. 
   The second line contains a string S, 
   representing the first letters of the words in a passage of text. 
   S contains only the characters 'a' - 'z', with no spaces.

  Output

  For each test case, output one line containing "Case #x: y", 
  where x is the case number (starting from 1) and y is 
  the minimum number of characters in the password string for S.

  Limits

  1 ≤ T ≤ 20.
 
  S will contain at least 2 * k characters.
  There will exist a password string with at most 1018 characters.
 
  Small dataset

  S will contain at most 1000 characters.
  k = 2.
  Large dataset

  S will contain at most 5000 characters.
  2 ≤ k ≤ 500.
 
  Sample
 
  Input 
 
  Output 
 
  4
  2
  poppop
  2
  google
  2
  tbilcwhafiparmdmotcaaloth
  10
  tbilcwhafiparmdmotcaaloth
  Case #1: 6
  Case #2: 18
  Case #3: 53
  Case #4: 1136

  In the first sample input, one possible password string is "0ppop0".
  In the second sample input, one possible password string is "goo0og00gle9o909l3".






  This is certainly an intimidating problem. 
  Even small strings, like the example from The Fellowship of the Ring, 
  are difficult to work through. 
  
  When you are trying to combine passwords of length at most 500 
  into a password string, 
  there are so many possibilities to consider that optimization quickly becomes overwhelming.


  The Small Input

  Let's begin with the small input. 
  In this case, we need to connect up pairs of letters. 
  The key insight here is to imagine everything as a graph. 
  
  If you have heard of De Bruijn sequences before, 
  that is the perfect thing to build from. 
  
  We will make a vertex for each letter 
  (the 26 normal letters as well as all the 133t variations), 
  and add an edge between every pair of letters. 
  
  Each 2-letter word that we need in our password string 
  corresponds to an edge on this graph. 
  
  For example, "t0" corresponds to the edge 't' -> '0'. 
  Let's call these candidate edges.

  Next let's consider a password string. 
  We can think of this as a path on the graph. 
  We start at the first letter, 
  and then continually move to the next letter in the string. 
  
  For example, "abc0" corresponds 
  to the path 'a' -> 'b' -> 'c' -> '0'. 
  
  Therefore, the problem can be re-stated as follows: 
  what is the length of the shortest path on this graph 
  that includes all the candidate edges?

  Here is it is helpful to think back 
  to another classic algorithm problem: Eulerian paths. 
  
  The problem could also be re-stated as: 

  if we start with just the candidate edges, 
  what is the minimum number of edges we need to add 
  so that the resulting graph has an Eulerian path? 
  
  Fortunately for us, the Eulerian path problem has been completely solved!

  Fact: 

  A directed graph has an Eulerian path if and only if 
  
  (1) every vertex has in-degree equal 
      to its out-degree except possibly 
	  for two vertices that are off by one, and 
	  
  (2) every vertex with positive degree is connected 
      in the underlying undirected graph.

  If you play with some examples, 
  you will see that you will always be connected here, 
  but let's come back to a formal proof 
  when we talk about the large input. 
  
  The fact that connectivity comes for free 
  is really what makes this problem solvable, 
  and so it is a good thing to think about!

  The remaining condition says that all we need to do 
  is add edges to balance in-degrees and out-degrees. 
  
  We can add any edge we want at any time, so the method is simple: choose a vertex u with higher in-degree than out-degree and a vertex v with higher out-degree than in-degree, and then add an edge from u to v. Repeat until there are only two vertices left that are only slightly imbalanced, and we're done! After all that talk, we end up with a very simple greedy algorithm.


  http://samplecodebank.blogspot.com/2011/04/eulerian-path-in-c.html


  http://chessprogramming.wikispaces.com/De+Bruijn+Sequence+Generator

  The Large Input

  In fact, this solution already has most of the key ideas that 
  go into solving the large as well, but we just need to take them further. 
  There are three main challenges:

  When passwords are length greater than 2, 
  how do we interpret them as edges on a graph?

  When it comes time to balancing degrees, 
  some edges will be impossible to add. 
  How do we adapt the greedy algorithm from before?

  The output password could be huge! 
  Is it really possible to solve the problem in time less 
  than the size of the output password?

  The first challenge is the most important, and again, 
  De Bruijn sequences provide a good model to build from. 
  If we are trying to construct all passwords of length k, 
  we will create a graph with vertices corresponding to all lengh-(k-1) phrases. 
  Each candidate password corresponds to the edge between 
  its prefix phrase and its suffix phrase. 
  
  Let's begin by making this nice and rigorous, 
  although the intuition is exactly the same as in the small input.

  A Minimum-Cost Flow Interpretation

  Let us call any length-k substring of S 
  (and any of its "l33tspeak" equivalents) a "candidate", 
  
  and any length-(k-1) string composed of digits 
  and lowercase letters a "phrase" 
  (whether it is contained in a candidate or not). 
  
  For each phrase s, 
  define its weight w(s) 
  to be the number of candidates that end with s 
  minus the number of candidates that begin with s. 
  Note that the sum of w(s) is 0. 
  
  (The weight of a phrase will measure how far 
   the corresponding vertex is from supporting an Eulerian path.)

  Form a directed graph G with vertices corresponding to the phrases. 
  For phrases a and b, add an edge from a to b 
  if you can make a into b by adding a letter to the end of a 
  and removing the first letter of a. 
  Now we set up a flow problem: 
  if a phrase s has positive weight, 
  it is a source with capacity |w(s)|; 
  otherwise it is a sink with capacity |w(s)|. 
  All edges have infinite capacity and cost one.

  Let ANSWER denote the shortest possible length of a password string for S, 
  let C denote the set of all candidates, 
  and let FLOW_EDGES denote the minimum number of edges (i.e. minimum cost) 
  in an "almost-maximum" flow. 
  (Specifically, this is a flow that leaves at most 1 capacity unused in some source 
  and at most one 1 capacity unused in some sink).
  
  Lemma 1: ANSWER = FLOW_EDGES + |C| + k - 1.

  Proof Part 1: ANSWER ≤ FLOW_EDGES + |C| + k - 1

  Let G' be the directed graph formed as follows:

   1. Begin with the directed multi-graph formed by the minimum-cost almost-maximum flow on G.
   2. For each candidate c, add an edge from the prefix phrase of c to the suffix phrase of c.

  For a given phrase s, let's calculate its out-degree minus its in-degree in G'. 

  After the first step, this value is exactly w(s), 
  except for two vertices that are off by 1. 
  (This is due to the fact that our flow is only almost maximum.) 
  
  After the second step, this value becomes exactly 0, 
  again except for two vertices that are off by 1.
  
  Therefore, we know G' satisfies the condition on in-degrees 
  and out-degrees for it to have an Eulerian path. 
  (See the "Fact" in the Small Input discussion.) 
  We now show it also satisfies the connectivity condition. 

  This is where we use the specific nature of the problem 
  and the fact that |S| ≥ 2k. 
  Actually, we suspect the last condition is unnecessary in the end, 
  but the proof becomes much harder if this is not true!

  Let's say a vertex s is a core vertex if it corresponds to a phrase in S, 
  or to a 133tspeak variation of such a phrase.

  If s a core vertex, then s is adjacent in G' to its predecessor and successor phrases within S. 
  (Note that there may be multiple predecessors or successors if the new letter 
  has a leet variation or if S appears multiple times.) 
  Therefore, we can certainly follow the edges of G' to go from s to some phrase a(s) 
  that starts at the first letter of S. We can then walk from there to a phrase b(s) 
  that ends at the last letter of S. 
  Since a(s) and b(s) are completely disjoint, 
  we can choose b(s) to be completely non-leet by always adding on non-leet successor letters. 
  This means b(s) does not depend on s, and hence we have demonstrated every core vertex 
  is connected to a single vertex in G'.

  Now consider a non-core vertex t with positive degree in G'. 
  This can only happen if t has positive degree in G, 
  and therefore it must be connected via the flow to some core vertex s. 
  Since we just showed all core vertices are connected, 
  we conclude the non-core vertices are connected as well.

  Therefore, the underlying undirected graph of G' is indeed connected, 
  and hence G' has an Eulerian path consisting of some vertices s1, s2, ... s
  FLOW_EDGES + |C| + 1. We can now construct a password string as follows:

  Begin the password string with the k - 1 letters in s1.
  For each subsequent vertex si, append to the password string 
  the one letter at the end of si that is not in si-1.

  This string has length exactly k - 1 + FLOW_EDGES + |C|, as required. 
  Notice that after appending the letter for si, the final k-1 letters 
  in the password string are always precisely equal to si. 
  (This invariant can easily be proven by induction.) 
  Now consider an arbitrary candidate c. Because of how we constructed G', 
  c has prefix si-1 and suffix si for some i. But then after appending the letter for si, 
  the last k letters precisely spell out c. Hence, every candidate is in this password string, 
  and the inequality is proven.


  Proof Part 2: ANSWER ≥ FLOW_EDGES + |C| + k - 1

  For the second part of the proof, we just need to reverse the previous argument. 
  It is actually a little easier because we do not have to worry about connectivity.
  Consider a password string P of length ANSWER. 
  By definition, we know each candidate must appear somewhere in P. 
  Therefore, for each candidate c, we can define pos(c) 
  to the be the character in P where the first occurrence of c begins. 
  
  Now let's order the candidates c1, c2, ..., c|C| by pos(c).
  For each i, consider the substring of P starting with character pos(ci) + 1 
  and ending with character pos(ci+1) + k-2. 
  Note that this substring begins with the suffix phrase of ci 
  and ends with the prefix phrase of ci+1.

  By reversing the construction from the previous section, 
  we can interpret this substring as a path in the graph G 
  from the suffix phrase of ci to the prefix phrase of ci+1. 
  This path has pos(ci+1) - pos(ci) - 1 edges in it. 
  Now let's think about what happens when we combine all these paths. 
  We get a flow with a source at every suffix phrase 
  except c|C| and a sink at every prefix phrase except c1. 
  When we add all these sources and sinks together, 
  we get precisely an almost max-flow on G. 
  Furthermore, this flow uses exactly sumi [pos(ci+1) - pos(ci) - 1] = pos(c|C|) - pos(c1) - |C| + 1 edges.

  It follows that FLOW_EDGES ≤ pos(c|C|) - pos(c1) - |C| + 1. 
  Finally, we know pos(c|C|) ≤ |P| - k = ANSWER - k, and also pos(c1) ≥ 0. 
  Therefore, ANSWER ≥ FLOW_EDGES + k + |C| - 1, as required.

  A Greedy Solution

  At this point, we still have not solved the whole problem, 
  but we have reduced it to something more tractable. 
  Minimum-cost flow is a complicated problem but it is well known, 
  and at least it can be solved in polynomial time. 
  We can either try to optimize here, or we can use more clever idea to make our life much simpler.

  Lemma 2: 
  Fix the graph G and assign arbitrary source and sink capacities to arbitrary nodes. 
  Then, a minimum-cost almost-max flow can be achieved by repeatedly taking 
  the shortest path from an unused source to an unused sink, 
  and pushing flow along there 
  (without ever reverting any flow that's already been pushed).

  Proof: 

  Let F denote a minimum-cost almost-max flow on G, 
  and suppose the shortest path in G between a source and a sink 
  goes from source u to sink x. 
  Furthermore, suppose that F contains a path from u to a different sink y, 
  and a path from a different source v to x. 
  

  We have 2 vertexes: u, v - sources, x, y - sinks
  Paths: 
  u -> x, the shortest path
  u -> y,
  v -> x

  We claim that we could replace these two paths 
  with a path from u to x and with a path from v to y 
  to achieve a flow with no more edges. 
  
  (Since F is only an almost-max flow, it might also be 
   that u and/or x is simply not used in F, but that case is a lot simpler.) 
   
  Recall that every edge in G has infinite capacity, 
  so the only challenge here is making sure the path lengths work out.

  Given two phrases p and q, 
  let's define A(p, q) to be the longest string that is both a suffix of p and a prefix of q. 
  Then the distance from p to q in G is precisely equal to k - 1 - |A(p, q)|. 
  
  This means we can reformulate the claim as follows: 
  given that |A(u, x)| ≥ max(|A(u, y)|, |A(v, x)|), 
  prove that |A(u, x)| + |A(v, y)| ≥ |A(u, y)| + |A(v, x)|.

  Now, notice that A(u, x) and A(u, y) are both suffixes of u, 
  but A(u, x) is at least as long. 
  
  Therefore, A(u, y) is a suffix of A(u, x). 
  Similarly, A(v, x) is a prefix of A(u, x). 
  Let t = |A(u, y)| + |A(v, x)| - |A(u, x)|. 
  If t ≤ 0, the claim is trivial. 
  
  Otherwise, there must be a length-t string z that 
  is a suffix of A(v, x) and a prefix of A(u, y). 
  Then z is also a suffix of v and a prefix of y. 
  Therefore, |A(v, y)| ≥ |z| = t, and the claim is proven.

  We have shown that F can be modified to contain the 
  path from u to x without increasing the number of edges. 
  Now consider the rest of F. 
  
  It defines a smaller flow, and we can repeat the same argument to show 
  this residual flow can also be modified to contain the shortest path 
  between a remaining source and a remaining sink, 
  and that this modification will not increase the number of edges. 
  Applying this argument repeatedly gives us the flow proposed 
  in the lemma without ever increasing the number of edges, 
  and so we have shown that this flow is indeed optimal.
  

  The Implementation

  Almost there! 
  
  We have outlined a greedy algorithm, but what does it actually mean 
  when it is put back into the context of the original problem, 
  and how can it be implemented quickly?

  The first thing to do is to construct the prefix phrases 
  and the suffix phrases of all candidates. 
  
  We need to construct an almost max-flow from the suffixes to the prefixes. 
  If a phrase is both a suffix and a prefix, then these two instances cancel out.

  First we should look for a source u and a sink x 
  that are separated by distance 1 in the underlying graph. 
  This is equivalent to saying that there is a length k-2 string 
  that is a suffix of u and a prefix of x.

  Next we should look for a source u and a sink x 
  that are separated by distance 2 in the underlying graph. 
  This is equivalent to saying that there is a length k-3 string 
  is a suffix of u and a prefix of x.

  And so on...

  This can be summarized as follows:

  Let P = the multi-set of prefix phrases of all candidates.
  Let S = the multi-set of suffix phrases of all candidates.

  Let x = k + |C| and i = 0.(*) 
  While |P| ≥ 2 and |P intersect S| ≥ 1: 
  delete one copy of a common element from both P and S and increment x by i.

  Remove the last letter from every element in P 
  and the first letter from every element in S.

  Increment i by 1, and repeat again from (*) until P and S have just one element.
  Output x.

  Unfortunately, this is still a little too slow. 
  It will run in time proportional to the output password string length, 
  which could be 1018. 
  
  The final optimization is that if you look at any element in P (or S) in the algorithm above, 
  then all 133tspeak variations of that element should also be in P (or S). 
  You can always treat all variations as one big batch:

  Let P = the map from phrase to the number of times a 133tspeak variation 
  of that phrase appears as the prefix of a candidate.

  Define S similarly for suffixes.
  Let x = k + |C| and i = 0.(*) 
  
  While P and S are non-empty:
  While P and S have a common element t: 
  delete min(P[t], S[t]) from P[t] and S[t] and increment x by i * min(P[t], S[t]).

  Remove the last letter from every element in P 
  and the first letter from every element in S.

  Increment i by 1 and repeat again from (*) until P and S are empty.
  Output x-i+1.


*/

/*  Here it is an array which contains 
    a letter and a digit 
    
    "o" to "0", 
    "i" to "1", 
    "e" to "3", 
    "a" to "4", 
    "s" to "5", 
    "t" to "7",
    "b" to "8" 
    and/or 
    "g" to "9".
 */

 #include <string.h>

char images[26][3] = 
{
"a4",
"b8",
"c",
"d",
"e3",
"f",
"g9",
"h",
"i1",
"j",
"k",
"l",
"m",
"n",
"o0",
"p",
"q",
"r",
"s5",
"t7",
"u",
"v",
"w",
"x",
"y",
"z" 
};

int Pr10GetIndex(char c)
{
   int code = c - 'a';
   int digit; 

   if (code > 0)
	   return code;

   digit = c - '0';

   if (digit == 0) return 26 + 0;
   if (digit == 3) return 26 + 1;
   if (digit == 4) return 26 + 2;
   if (digit == 5) return 26 + 3;
   if (digit == 7) return 26 + 4;
   if (digit == 8) return 26 + 5;
   if (digit == 9) return 26 + 6;

   return -1;
}

void Pr10Solve()
{
   char* S = "google";

   int K = 2;
   int N = 6;

   int n;
   int a;
   int b;
   int i;
   int j;
   int add = 0;

   char* Edges[100];
   int EdgesCount = 0;
   
   int MapDegree[26 + 8];
   int result;

   for (n = 0; n < 26 + 8 - 1; n++)
   {
       MapDegree[n] = 0;
   }

   /* This code build a graph with all possible 
    * edges in the password:
	* the first two letters [go] can be:
	* go, g0, 9o, 90  the first possible edges
	* ol, 0l          the second
	* le, l3          the third
	* 
	* All these edges are added into edges array
    */
   for (n = 0; n < N - 1; n++)
   {
       a = S[n] - 'a';
	   b = S[n + 1] - 'a';
       
       for (i = 0; i < strlen(images[a]); i++) 
	   {
           for (j = 0; j < strlen(images[b]); j++) 
		   {
              char* tmp;

			  strcpy(tmp, images[a][i]);
              strcpy(tmp, images[b][j]);

              Edges[EdgesCount] = tmp;
		   }
	   }
   }
   
   /* For all vertexes we need to calculate in-depth and out-depth
    * number of vertexes
    */
   for (n = 0; n < EdgesCount; n++)
   {
	   MapDegree[Pr10GetIndex(Edges[n][0])]++;
       MapDegree[Pr10GetIndex(Edges[n][1])]--;
   }
   
   /* Go through all vertexes and find those which out-depth 
    * number of vertexes is higher than the number of 
	* in-depth vertexes.
	*
	* We need to prove that the number of (+) vertexes 
	* is equal to the number of (-) vertexes.
	* It cn be proven as the maximum number
	* of (+) vertexes is 2.
    */
   for (n = 0; n < 26 + 8 - 1; n++)
   {
       if (MapDegree[n] > 0)
	   {
           add += MapDegree[n];
	   }
   }

   if (add == 0) 
   {
	   /* If the number of edges is n - the number of letters in
	    * the word is n + 1. We can easy prove it:
		* abcdcgf. All edges are: ab, bc, cd, dc, cg, gf: 
		* the last letter is not accounted
	    */
       result = EdgesCount + 1;
   }
   else
   {
       result = EdgesCount + add;
   }
}   

struct DicItem
{
    char* Key;
    int Value;
};

int KeyExists(struct DicItem Dictionary[], int* DicCount, char* DicKey)
{
	int i;

    for (i = 0; i < DicCount; i++)
	{
		if (strcmp(DicKey,Dictionary[i].Key) == 0)
	        return 1;
	}
	return 0;
}

void DicItemIncrement(
    struct DicItem Dictionary[], 
	int* DicCount, 
	char* DicKey,
	int AddValue)
{
    int i;

    for (i = 0; i < DicCount; i++)
	{
		if (strcmp(DicKey,Dictionary[i].Key) == 0)
	        break;
	}
    
    if (i == DicCount - 1)
	{
		Dictionary[i].Key = DicKey;
        Dictionary[i].Value = 0;
		(*DicCount)++;
	}

	Dictionary[i].Value += AddValue;
}

void DicItemDecrement(
    struct DicItem Dictionary[], 
	int* DicCount, 
	char* DicKey,
	int DecValue)
{
    int i;

    for (i = 0; i < DicCount; i++)
	{
        if (strcmp(DicKey,Dictionary[i].Key) == 0)
	        break;
	}
    
    if (i == DicCount - 1)
	{
		Dictionary[i].Key = DicKey;
        Dictionary[i].Value = 0;
		(*DicCount)++;
	}

	Dictionary[i].Value -= DecValue;
}

struct Edge 
{ 
    int source;
	int destination;
    int capacity; 
	int residue;
    int cost;
};

/* Lets illustrate how it works on simple example 
 * 0, 1, 2, 3, 4, 5, 6
 * source = 0;
 * sink = 2;
 *
 * V[0, 1] = (8, 8, 0)
 * V[1, 0] = (8, 0, 0)
 * 
 * V[1, 3] = (3, 3, 0)
 * V[3, 1] = (3, 0, 0)
 *
 * V[1, 4] = (5, 5, 0)
 * V[4, 1] = (5, 0, 0)
 * 
 * V[5, 2] = (6, 6, 0)
 * V[2, 5] = (6, 0, 0)
 * 
 * V[6, 2] = (2, 2, 0)
 * V[2, 6] = (2, 0, 0)
 *
 * V[3, 5] = (3, 3, 123)
 * V[5, 3] = (3, 0, -123)
 *
 * V[3, 6] = (2, 2, 100)
 * V[6, 3] = (2, 0, -100)
 *
 * V[4, 5] = (5, 5, 55)
 * V[5, 4] = (5, 0, -55)
 *
 * V[4, 6] = (2, 2, 80)
 * V[6, 4] = (2, 0, -80)
 */
void MincostMaxflowAddArc(int** V, struct Edge* E, int* ECount,
	int source, int destination, int capacity, int cost)
{
	struct Edge SourceDest = { source, destination, capacity, capacity, cost };
	struct Edge DestSource = { destination, source, capacity, 0, -cost };
    int e = ECount;
	V[source][V[source][0]++] = e;
    V[destination][V[destination][0]++] = e + 1;
    E[(*ECount)++] = SourceDest;
    E[(*ECount)++] = DestSource;
}
/* Lets illustrate on an example how this function calculates 
 * transition cost:
 * long long transition_cost(const string &A, const string &B) 
 * {
 *    for (int n=1; n<=int(A.size()); ++n) 
 *       if (string(A.begin()+n,A.end()) == string(B.begin(),B.end()-n)) 
 *           return n;
 * }
 *
 * A = 'asdef' B = 'asdef'
 * n = 1: string(A.begin()+n,A.end()) = string(0 + 1, 5) = sdef 
 *        string(B.begin(),B.end()-n)) = string(0, 5 - 1) = asde
 * n = 2: def != asd
 * n = 3: ef != as
 * n = 4: f != a
 * n = 5: 0 == 0.
 * For equal strings we have cost = K.
 * We find if suffix(A, n) = prefix(n, B) If we can represent these strings
 * in this way: when n = K - 1 we need only one edge to move, when n = K - 2
 * we need 2 edges to go through.
 */
int transition_cost(char* A, char* B, int K) 
{
	int n;
	char* left = (char*)malloc(K * sizeof(char));
	char* right = (char*)malloc(K * sizeof(char));

    for (n = 1; n <= K; ++n) 
	{
		strncpy(left, A + n, K - n); 
        strncpy(right, A, K - n); 
		if (left == right) return n;
	}
}

struct Pair
{
	int first;
	int second;
};

void QueuePush(int Key, int Value) {}
int IsQueueEmpty() { return 1; }
struct Pair GetTop() 
{ 
	struct Pair ret = { 1, 1 };
	return ret; 
}
void QueuePop() {}

void GetFlow(int source, int sink, int** V, int VCount, 
    struct Edge* E, int* ECount, int* FlowSize, int* FlowCount)
{
    int N = VCount;
	int M = *ECount;

    int flowSize = 0;
	int flowCost = 0;

    int Tinfinity = 1;
	int Uinfinity = 1;
	int Uepsilon = 1;
	int i;
	int canPush;

	int flow[100];
	int flowCount = M;

	int potential[100];
	int potentialCount = N;

	int from[100];
	int fromCount = N;

    int dist[100];
	int distCount = N;

    /* This cycle finds the maximum number of the type (int) */
	while (2 * Tinfinity > Tinfinity) Tinfinity *= 2;
	/* This cycle finds the maximum number of the type (int)  */
	while (2 * Uinfinity > Uinfinity) Uinfinity *= 2;
	for (i = 0; i < 30; i++) Uepsilon /= 2;
	for (i = 0; i < flowCount; i++) flow[i] = 0;
    for (i = 0; i < potentialCount; i++) potential[i] = 0;
    /* We find a shortest path in a residual grapth.
	 * Then find the max possible flow we can put through this 
	 * shortest path.
	 * Lets illustrate how it works on simple example
	 * 0, 1, 2, 3, 4
	 * source = 0;
	 * sink = 2;
	 * V = 
	 * 
	 */
	while(1)
	{
		int Where;
        for (i = 0; i < fromCount; i++) from[i] = -1;
        for (i = 0; i < distCount; i++) dist[i] = Uinfinity;
	    QueuePush(0, source); //From the start the queue is empty and we add source
        from[source] = -2; 
	    dist[source] = 0;
		/* Using Dijkstra algorythm here to find the 
		 * shortest path from s to t.
		 * From the start: Q = [0, 0]
		 */
        while (!IsQueueEmpty()) 
	    {
		   struct Pair top = GetTop();
		   int howFar = top.first;
		   int Where = top.second;
		   QueuePop();
		   if (dist[Where] < howFar) continue;
           
		   /* source = 0 and Adjacent(0) = 1 
		    * dest = 1, cost = 0, potential = 0, dist = inf
			* dist[1] = 0.
			*
			* QueueTop = 1. Start the cycle again
			* Adjacent[1] = {3, 4}
			* dist[3] = 0 + 0 = dist[4] = 0
			* 
			* QueueTop = {3, 4}
			* QueueTop = 3:
			* dist[5] = 123, dist[6] = 100
			* 
			* QueueTop = 4:
			* dist[5] = 55, dist[6] = 80
			* The shortest path is: (0, 1, 4, 5, 2)
            *
            * For the second time it is almost the same
			* but we have already decreased flow through the smallest
			* augumenting path and potentials have been added into vertexes
			*  
			* Deque 0, 1 - nothing changes 
			* Deque 3: dest = 5, 0 + 0 - 55 + 123 
			* 
		    */
		   for (i = 0; i < V[Where][0]; i++)
		   {
			   int dest;
			   int cost;
			   /* residue ==0 means that this vertex is not inside the residual net */
			   if (E[V[Where][i]].residue == 0) continue;
			   dest = E[V[Where][i]].destination;
			   cost = E[V[Where][i]].cost;

			   if (dist[dest] + potential[dest] > dist[Where] + potential[Where] + cost + Uepsilon)
			   {
                   dist[dest] = dist[Where] + potential[Where] - potential[dest] + cost;
				   from[dest] = V[Where][i];
				   QueuePush(dist[dest], dest);
			   }
		   }
	    }

		/* update vertex potentials */ 
		for (i = 0; i < N; i++) 
		{
			if (dist[i]==Uinfinity) potential[i] = Uinfinity;
			else if (potential[i]<Uinfinity) potential[i] += dist[i];
		}

		/* if there is no path, we are done */

		if (from[sink] == -1)
		{
			*FlowSize = flowSize;
			*FlowCount = flowCount;

			return;
		}

		/* construct an augmenting path */
		canPush = Tinfinity;
		Where = sink;

		while (1) 
		{ 
		   if (from[Where]==-2) break;
		   canPush = canPush < E[ from[Where] ].residue ? canPush : E[ from[Where] ].residue;
		   Where = E[ from[Where] ].source;
		}

        /* update along the path */
       Where = sink;
       while (1) 
	   { 
          if (from[Where]==-2) break;
          E[ from[Where] ].residue -= canPush;
		  /* For example: from[Where] = 101 = 2*2 + 1 = 5 
		   * 00101  xor           00100  xor
		   * 00001   =  100 = 4   00001   = 101 = 5
           *
		   */
          E[ from[Where]^1 ].residue += canPush;
          flowCost += canPush * E[ from[Where] ].cost;
          Where = E[ from[Where] ].source;
       }
       flowSize += canPush;
	}
}


int Pr10SolveLarge()   
{
    int K = 10;
	char* S = "tbilcwhafiparmdmotcaaloth";
	int N = 25;

    char* Edges[100];
	int EdgesCount = 0;

	struct DicItem Dictionary[100];
	int DicCount = 0;

	struct DicItem VON[100];
	int VONCount = 0;

    struct DicItem DNU[100];
	int DNUCount = 0;

    struct DicItem ID[100];
	int IDCount = 0;

	int next_id;
	int add;

	int celkovo_hran;

    int n;
	int k, i, j;

	char* startSubStr = (char*)malloc((K - 1) * sizeof(char));
    char* endSubStr = (char*)malloc((K - 1) * sizeof(char)); 
	char* prefixSubStr = (char*)malloc((K - 1) * sizeof(char)); 
	char* suffixSubStr = (char*)malloc((K - 1) * sizeof(char));

	int V[100][100];
	int VCount = 0;

    struct Edge E[100];
	int ECount = 0;

	/* Copy all possible substrings into Edges array 
	 * The length of all substrings is K
	 */
    for (n = 0; n <= N - K; n++)
	{
        Edges[n] = (char*)malloc(K * sizeof(char));
        strncpy(Edges[n], S + n, K);
	}

	EdgesCount = N - K;
    
	startSubStr = (char*)malloc((K - 1) * sizeof(char));
    endSubStr = (char*)malloc((K - 1) * sizeof(char));

    strncpy(Edges[n], S, K - 1);
    strncpy(Edges[n], S + (N - 1) - (K - 1), K - 1);
    
	/* Here we need some explanation. Lets choose a simple example:
	 * google. g->9, o->0, e->3, K = 3
	 * Edges: goo, oog, ogl, gle
	 * Graph vertexes will be all possible substrings by length: K - 1 = 2.
	 * 
	 * Lets choose the first edge: goo
	 * vrcholov = len('g9') * len('o0') = 2 * 2 = 4 - is the number of all possible 
	 * vertexes created from 'go' by applying all substitutions: g->9, o->0
	 * All possible vertexes: go, g0, 9o, 90.
	 * vrcholov is the number of all possible source vertexes for 'goo'
	 *  
	 * hran = vrcholov * strlen(images[ E[K-1]-'a' ]) - is the number of all possible 
	 * edges needed to represent 'goo' with all its variations. If the last letter
     * has 2 possible variants, we will have 4 * 2 possible edges.
	 * 
	 * DEG[ string(E.begin(),E.end()-1) ] -= strlen(images[ E[K-1]-'a' ]);
	 *
     * string(E.begin(),E.end()-1) = 'go' is one of the vertexes
	 * strlen(images[ E[K-1]-'a' ]) = 2 is a number of destination symbols
	 * That means that we have 2 edges which go out from the vertex 'go'
	 * All outgoing edges we mark with '-'
	 * 
	 * DEG[ string(E.begin()+1,E.end()) ] += strlen(images[ E[0]-'a' ]);
	 * string(E.begin()+1,E.end()) = 'oo' is is one of the vertexes 
	 * strlen(images[ E[0]-'a' ]) = 2 is a number of source vertexes
	 * All ingoing edges we mark with '+'
	 * We need to prove here some rule:
	 * We have goo. And NumOut(go) = x, NumIn(oo) = y
	 * From NumOut(9o) = NumOfVariances(o) = NumOut(g0) = NumOut(90)
	 */

    for (n = 0; n <= EdgesCount - 1; n++)
	{
        char* edge = Edges[n];
		char symbol;

        int vrcholov = 1;
		int hran;

		for (k = 0; k < K - 1; k++)
		{
			symbol = edge[k] - 'a';
            vrcholov *= strlen(images[symbol]);
		}

		symbol = edge[K - 1] - 'a';
		hran = vrcholov * strlen(symbol);
        celkovo_hran += hran;

        strncpy(prefixSubStr, edge, K - 1);
        strncpy(suffixSubStr, edge + 1, K - 1);

        symbol = edge[0] - 'a';
		// DEG[ string(E.begin()+1,E.end()) ] += strlen(images[ E[0]-'a' ]);
        DicItemIncrement(Dictionary, &DicCount, suffixSubStr, strlen(images[symbol]));

    	symbol = edge[K - 1] - 'a';
		// DEG[ string(E.begin(),E.end()-1) ] -= strlen(images[ E[K-1]-'a' ]);
        DicItemIncrement(Dictionary, &DicCount, prefixSubStr, strlen(images[symbol]));
	}

    next_id = 3;
    add = 0;

    for (n = 0; n <= EdgesCount - 1; n++)
	{
		int mul = 1;
        struct DicItem it = Dictionary[n];
		/* In this case we have the flow through this vertex set to 0
		 * We miss thease vertexes.
		 */
		if (it.Value == 0)
			continue;

		if (!KeyExists(ID, &IDCount, it.Key))
		{
            DicItemIncrement(ID, &IDCount, it.Key, next_id);
            ++next_id;
		}
        /* In this case we have key = go. and mul is the number 
		 * of all possible variations of this case by applying g->9 and o->0
		 */
        for (k = 0; k < K - 1; k++) mul *= strlen(images[it.Key[k] - 'a']);
		/* The vertex has positive flow 
		 * For example
		 * it.Key = go, it.Value = +2 - is the flow through the vertex 'go'
		 * mul is 4: go, 9o, g0, 90 all possible variations of the substring 'go'
		 * And all of them have the same flow. 4 * (+2) is the total flow through
		 * the vertex 'go' including all possible variations.
		 */
		if (it.Value > 0)
		{
			DicItemIncrement(VON, &VONCount, it.Key, mul * it.Value);
			add += mul * it.Value;
		}
		else
		{   
			/* Vertexes with negative flow we also add into an nother array */
            DicItemIncrement(DNU, &DNUCount, it.Key, -mul * it.Value);
		}

		if (add == 0) 
		{   
			/* celkovo_hran is the total number of edges in the letter
			 * graph. The total number of letter in the word is for 
			 * K - 1 letters higher than the total number of edges
			 * For example:
			 * asdfgdgg  K = 3
			 * asd, sdf, dfg, fgd, gdg, dgg - the last 2 (K - 1) letters are 
			 * not accounted.
			 */
			return celkovo_hran + K - 1;
        }
	}
    /* Zero all arrays needed for minimal cost flow algorythm implementation */ 
    for (i = 0; i < 100; i++) 
		V[i][0] = 0;

	/* Here we need explanation how we build a graph
	 *
	 * 1. Vertex 0, Vertex 1 and between them an edge: C = Add - 1
	 *    Capacity means how much flow we can run on this edge.
	 *    Our total flow is Add: FLOW = Add.
	 *    0, 1 vertexes are artificial and the capacity is as max as possible:
	 *    and cost is 0.
	 * 
	 * 2. VON is an array of vertexes with positive value add. We enumerate them 
	 *    from the number of 3: 3,4,5,...n. Small agreement to make our provement 
	 *    more convenient. 
	 *    We numerate: 3,4,5,...,k     are numbers with positive add
	 *                 k+1,k+2,...,n are numbers with negative add
	 * 
	 *    We build edges: (1,3), (1,4), (1,5),..., (1,k-1), (1,k)
	 *    The capacity C(1,i),  3<=i<=k = Add(i)
	 *    VON are all vertexes from the source.
	 *  
	 * 3. DNU are all vertexes from the sink. k+1,k+2,...,n
	 *    We build edges: (k+1,2), (k+2,2), (k+3,2), (k+4,2), ..., 
	 *    The capacity C(i,2),  k+1<=i<=n =  Add(i)
	 *  
	 *    In our case we have multiple source and multiple sink. 
	 *    To facilitate implementing of minimal-cost flow algorythm we need to 
	 *    transform this graph into a graph with only one source and only one sink.
	 *  
	 * 4. Build edges between all vertexes from the source VON to the vertexes DNU
	 *    For each vertex from VON we match the vertex from DNU
	 *    VONi: (1,i)  Add(1,i) 
	 *    DNUj: (j,2)  Add(i,2)
	 *    We build edge (1,i)->(j,2) C(i,j) = Min(Add(1,i), Add(j,2))
	 *    That means that we i has a certain number of outgoing edges that are needed
	 *    to be added. 
	 */

    MincostMaxflowAddArc(V, E, &ECount, 0, 1, add - 1, 0);

    for (i = 0; i < VONCount; i++) 
	{
		MincostMaxflowAddArc(V, E, &ECount, 1, -1, /*ID[VON[i].Key]*/ VON[i].Value, 0);
	}

    for (i = 0; i < DNUCount; i++) 
	{
		MincostMaxflowAddArc(V, E, &ECount, -1, /* ID[DNU[i].Key] */ 2, DNU[i].Value, 0); 
	}

    for (i = 0; i < VONCount; i++)
	{
		for (j = 0; j < DNUCount; j++)
		{
			int MinCapacity = (VON[i].Value < DNU[i].Value) 
				              ? VON[i].Value 
							  : DNU[i].Value;
            
			int Cost = transition_cost(VON[i].Key, DNU[j].Key, K);
            /* ID[VON[i].Key], ID[DNU[j].Key] */
            MincostMaxflowAddArc(V, E, &ECount, -1, -1, MinCapacity, Cost); 
		}
	}

	VCount = VONCount + DNUCount + 3;
}


