
/* GCJ F 13 2
Problem description

Konstantin and Ilia live in the same house. 

Konstantin lives upstairs, and enjoys activities that 
involve jumping, moving furniture around, and - in general - making noise. 

Ilia lives downstairs, and enjoys sleep.

In order to have a good evening, Konstantin wants to do at least K activities. 
Last night, Ilia asked Konstantin to try not to wake him up; 
and because Konstantin is a very nice neighbor, he agreed. 

Unfortunately, he took Ilia's request a bit too literally, 
and he will choose his activities in such a way as to minimize 
the probability that Ilia is woken up after falling asleep.

Each possible activity for Konstantin has an associated probability ai/bi. 
If Konstantin performs this activity, 
then at the end of it, Ilia will be awake with probability ai/bi, 
and asleep otherwise, regardless of whether he was asleep at the start. 

Moreover, for each possible activity Konstantin can perform it at most ci times 
(more than that would be boring, and Konstantin won't have a good evening if he's bored).

Konstantin wants to choose a number of activities to do, in order, so that:

The total number of activities done is at least K.
The ith activity is performed no more than ci times.
The probability Q that Ilia is woken up one or more times 
during the course of the activities is as small as possible.

Ilia starts awake, so in order for him to be woken up, 
he must be asleep at the end of some activity, and then awake at the end of the next activity.

What is the smallest Q Konstantin can achieve while having a good evening? 
Note that Konstantin cannot tell whether Ilia is awake or asleep, 
and so he cannot adapt his activities using that information.

Input

The first line of the input gives the number of test cases, T. 
T test cases follow. 
Each test case starts with a pair of integers, N, K, on a line by themselves. 
N lines follow, each of which represents an activity that Konstantin can choose. 
Each of those lines is formatted as "ai/bi ci", 
indicating that there is an activity which would 
leave Ilia awake with probability ai/bi 
and which Konstantin can perform at most ci times without being bored.

Output

For each test case, output one line containing 
"Case #x: Q", where x is the case number (starting from 1) 
and Q is the smallest probability of Ilia waking up during 
the course of the activities that Konstantin performs. 
Answers with absolute or relative error no larger than 10-6 will be accepted.

Limits

1 ≤ T ≤ 100.
0 ≤ ai ≤ bi ≤ 1000000 for all i.
1 ≤ bi and 1 ≤ ci for all i.
1 ≤ K ≤ the sum of all ci in that test case.

Small dataset

1 ≤ N ≤ 100.
The sum of all ci is no larger than 100 in each test case.
Large dataset

1 ≤ N ≤ 10000.
The sum of all ci is no larger than 106 in each test case.

nput 
 	
Output 
 
3

4 1

1/2 3
1/5 2
2/5 1
2/2 2

3 2

1/2 2
1/3 2
3/4 2

3 3

99/100 1
1/2 2
1/50 3

Case #1: 0.000000000
Case #2: 0.083333333
Case #3: 0.015000000

Solution

Upstairs/Downstairs was a last-minute addition to the Finals, 
replacing a year-old problem proposal 
that had appeared on a separate contest two months before. 

This problem loosely mirrors an experience the author had once while on vacation. 
He was downstairs.


Solving this problem involves two observations and an algorithm. 
Before making our observations, 
let's start by writing out the formula for the quantity we want to minimize. 
pi will represent the probability that the ith activity 
Konstantin performs will result in Ilia being awake:

P(woken up) =
(1-p0) * (1 - (1-p1)(1-p2)*...* (1 - pK)) +
p0(1-p1) * (1 - (1-p2)(1-p3)*...*(1 - pK)) +
p0p1(1-p2) * (1 - (1-p3)(1-p4)*...*(1 - pK)) +
...

We need firstly to prove this formula. Lets suppose that we only have 2 
p0 - is probability when activity 0 will result in Illia being waking up
p1 - is probability when activity 1 will result in Illia being waking up
When Konstantin is doing this activities consequently: 0 and then 1. We need 
to compute the probability of Illia being waked up.

2 activities:

      0                       1
 t1 ----------------- t2 --------------t3
 [t1 t2] Konstantin is performing activity x 
 Illia was not sleeping in the beginning - after t2 he falls asleep
 K1 - is the possibility when he will be woken up after t3.
 (1 - p0) wont be wake up and p1 - will be wake up during 1



 Explanation:
 pi - is a probability when after i-th activity Illia will be awake.
 Opposite probability means that
 1 - pi - is a probability when after i-th activity Illia will be sleeping.
 According to definition of Illia being woken up we have:
 ....... i-th activity........ | ....... (i + 1)-th activity ........| 
 The necessary condition for Illia being woken up in the end of the (i + 1)th
 activity is he is sleeping after i - th activity.
 (i - pi)*pi+1 = the possibility of being woken up.
 Formula computation:

 1. Illia starts being awake in the beginning of activity 0.
    p0 - is the probability that Illia will be awake after activity 0
	(1 - p0) - is the probability that Illia will be sleeping after activity 0.
 Provided that we need Illia being awake after activity 1 or 2 or 3 or .... k.
 (1 - p1) Illia is sleeping after activity 1
 (1 - p2) Illia is sleeping after activity 2
 (1 - p3) Illia is sleeping after activity 3
 .....
 (1 - pk) Illia is sleeping after activity k
 (1 - p1)*(1 - p2)*(1 - p3)*...*(1 - pk) Illia is sleeping after activity 1 and 2 and 3 ... and k
 1 - (1 - p1)*(1 - p2)*(1 - p3)*...*(1 - pk) Illia is awake after one of the following activities:
 1 or 2 or 3 or .... or k.
 (1 - p0)*(1 - (1 - p1)*(1 - p2)*(1 - p3)*...*(1 - pk)) - a probability when Illia is sleeping after 
 activity 0 and is being awake after one of the activities: 1, 2, 3, ...., k

 2. Illia is awake after the activity 0 p0.
 Then he will be woken up when:
 1. After activity 1 he is sleeping and is being awake after one of the activities: 2, 3, ..., k
    p0*(1 - p1)*(1 - (1 - p2)*(1 - p3)*...*(1 - pk))
 2. After activity 1 he is awake. In this case there are also 2 possible cases:
  2.1 He is sleeping after 2 and awake after one of (3, 4, ... , k)
    p0*p1*(1 - p2)*(1 - (1 - p3)*(1 - p4)*...*(1 - pk))
  Continue this process 
  ....
  The latest 
  When Illia is awake after activity 0, activity 1,..., k - 2.
  Then he sleeps after activity k - 1 and is awake after activity k
  p0*p1*p2*p3*...*pk-2*(1 - pk-1)*(1 - (1 - pk))
  
 3. Lets see how changing order of activities can influence on total probability.
 Parts of the formula which contain pi and pi+1:

 p0*p1*...*(1 - pi)*(1 - (1 - pi+1)*(1 - pi+2)*...*(1 - pk)) +
 p0*p1*...*pi*(1 - pi+1)*(1 - (1 - pi+2)*...*(1 - pk)) = 

 p0*p1*...*pi-1*( 
   (1 - pi)*(1 - (1 - pi+1)*(1 - pi+2)*...*(1 - pk)) 
 + pi*(1 - pi+1)*(1 - (1 - pi+2)*...*(1 - pk))   ) = 

 For simlifying this formula we need to make some definition:
 A = p0*p1*...*pi-1 > 0
 B = (1 - pi+2)*...*(1 - pk)
 Then the formula looks like here:
 
 A*((1 - pi)*(1 - (1 - pi+1)*B) + pi*(1 - pi+1)*(1 - B)) = 
 (1 - pi)(1 - B + Bpi+1) + pi(1 - pi+1)(1 - B) = 
 1 - B + Bpi+1 - pi(1 - B) - Bpipi+1 + pi(1 - B) - pipi+1(1 - B) = 
 1 - B + Bpi+1 - Bpipi+1 - pipi+1(1 - B) = 
 1 - B + Bpi+1 - Bpipi+1 - pipi+1 + Bpipi+1 =
 1 - B + Bpi+1 - pipi+1 

 This means that if we swap i and j activity we decrease the formula.
 In optimal solution activities must be applied from noisiest to quietest.
 And we need to sort them from the noisiest to quietest.
 
 4. Analizing the formula for minimum or maximum.
 P(wake up) = f(p0, p1, p2, p3,...,pk-1, pk)
 0 <= p0 <= 1, ... , 0 <= pi <= 1, ... , 0 <= pk <= 1.

 Statement: For f = fmin we need pi to be max or min. 
 Provement: Lets suppose opposite. f = fmin and 
 min(pi) < pi < max(pi). We fix all other parameters and we get linear function.
 This function can be linear increasing or linear decreasing.
 We have received a contradiction.

 Note that here we're considering an activity that 
 can be repeated c times as c different activities.

 1/3 = p0    Sort them from noisiest to quietest: 
 1/3 = p1    3/4 1/2 1/3 1/3 1/4 1/7
 1/4 = p2
 1/2 = p3
 1/7 = p4
 3/4 = p4


 */
 
 
 /*
 Formula:
 P(woken up) = 
  (1 - p0)(1 - (1 - p1)(1 - p2)...(1 - pk))
+ p0(1 - p1)(1 - (1 - p2)(1 - p3)...(1 - pk))
+ p0p1(1 - p2)(1 - (1 - p3)...(1 - pk))
+ ....
+ p0p1p2................pk-2(1 - pk-1)(1 - (1 - pk))

(1) - after p0 he is asleep and he is awake afetr 1 or 2 or 3 ...
(2) - he is awake after 0 then is asleep after 1 and is awake after 2 or 3 or 4...
(3) - he is awake after 0 and after 1, then he is asleep after 2 and 
      is awake after 3 or 4 or 5 or .... 
.....
(k) - he is awake after 0 and after 1 and after 2 and ... and after k - 2 
      then he is asleep after k - 1 and is awake after k

 Another way is to compute this firmula using Markov processes
 Illia can be in 3 possible states: [awake, asleep, woken up]
 The initial Illia's state is [1, 0, 0]
 The possible state vectors are: 
 [1, 0, 0] - awake    In column definition  [1] [0] [0]
 [0, 1, 0] - asleep                         [0] [1] [0]
 [0, 0, 1] - woken up                       [0] [0] [1]
 
 Lets take ith activity. 
 pi - is a probability that Illia will be awake
 (1 - pi) - is a probability that Illia will be asleep.
 0 - is a probability that woken up. It can be explained as to be woken up
     people need to be asleep
 If the current state is awake. That means that Illia can be asleep
 and can stay awake. It can be explained that the noise is so strong that Illia canno get sleep.
 Or performing some activity Konstantin has noticed that ni times Illia was awake and ki he was asleep.
 So he calculated the probability: ni/ki. 
 If the state is asleep
 0 - is a probability that Illia will be awake. This is 0 because it is impossible 
     to move from asleep state to awake state without being woken up.
 (1 - pi) - is a probability that Illia will be asleep. He is asleep and than he stays to be asleep
 pi - if he is awake after ith activity, that means that he has been woken up.
If we is woken up he can only stay to be woken up further.
 pi      0      0 
 1 - pi  1 - pi 0    is a stohastic matrix matrix of transitions. 
 0       pi     1
 Lets compute 2 adjacent stohastic matrix - it can describe a behaviour of Konstantin
 performing 2 sequentual activities i and i + 1
 |pi   0    0| |pi+1   0      0| |c00 0   0| |p   0   0| |c00p
 |1-pi 1-pi 0|*|1-pi+1 1-pi+1 0|=|c10 c11 0|*|1-p 1-p 0|=|c10
 |0    pi   1| |0      pi+1   1| |c20 c21 1| |0   p   1| |
 C[0,0] = pipi+1 + 0 + = pipi+1
 C[0,1] = 0
 C[0,2] = 0
 C[1,0] = (1-pi)pi+1 + (1-pi)(1-pi+1)
 C[1,1] = (1-pi)(1-pi+1)
 C[1,2] = 0
 C[2,0] = pi(1-pi+1)
 C[2,1] = pi(1-pi+1) + pi+1
 C[2,3] = 1
 |1 0 0| |p   0   0|   |p   0   0|
 |0 1 0|*|1-p 1-p 0| = |1-p 1-p 0|
 |0 0 1| |0   p   1|   |0   p   1|
 Continue this process for getting a recurrent sequence
 |x  0  0| |p   0   0| |xp        0        0|
 |y  s  0|*|1-p 1-p 0|=|yp+s(1-p) s(1-p)   0|
 |z  t  1| |0   p   1| |zp+t(1-p) t(1-p)+p 1|
We are interested in a probability when Illia was awake and after n
processes he is woken up 

t = t(1-p)+p, 
z = zp+t(1-p)

1 - t = 1 - t(1 - p) - p = (1 - t)(1 - p) 
t = 1 - (1 - t)(1 - p)

1. z = 0 t = 0
2. z = 0 t = 1 - (1 - p0)
3. z = (1 - p1)(1 - (1 - p0))
   t = 1 - (1 - p0)(1 - p1)
4. p2(1 - p1)(1 - (1 - p0)) + (1 - p2)(1 - (1 - p0)(1 - p1))
   t = 1 - (1 - p0)(1 - p1)(1 - p2)
5.  p3(p2(1 - p1)(1 - (1 - p0)) + (1 - p2)(1 - (1 - p0)(1 - p1)))
  + (1-p3)(1 - (1 - p0)(1 - p1)(1 - p2)) =

  z =  p3p2(1 - p1)(1 - (1 - p0)) +
       p3(1 - p2)(1 - (1 - p0)(1 - p1)) +
       (1-p3)(1 - (1 - p0)(1 - p1)(1 - p2))

  t = 1 - (1 - p0)(1 - p1)(1 - p2)(1 - p3)

4. z = p0(1-p1)p2 + (p0(1-p1)+p1)(1-p2) t = (1-p2)(p0(1-p1)+p1) + p2
Lets see is swapping pi+1 and pi can change the value.
ti = 
|x  0  0| |x1  0    0| |xx1
|y  s  0|*|y1  s1   0|=|yx1+sy1
|z  t  1| |z1  t1   1| |zx1+ty1+z1
So we need keep track of zx1+ty1+z1

ti = t(1-pi)+pi, zi = zpi+t(1-pi)
ti+1 = ti(1-pi+1)+pi+1 = (t(1-pi)+pi)(1-pi+1)+pi+1
zi+1 = zipi+1+ti(1-pi+1) = (zpi+t(1-pi))pi+1 + (t(1-pi)+pi)(1-pi+1)=
= zpipi+1 + t(1-pi)pi+1 + t(1-pi)(1-pi+1) + pi(1-pi+1) = 
= zpipi+1 + tpi+1 - tpipi+1 + t(1 - pi - pi+1 + pipi+1) + pi - pipi+1=
= pipi+1(z - t + t - 1) + tpi+1 - tpi+1 - tpi + pi + t = 
= pipi+1(z - 1) + (1 - t)pi + t
Here we can make an conclusion that if we swap i and j we will receive differences
Curr[i,j] = pipi+1(z - 1) + (1 - t)pi + t
Curr[j,i] = pi+1pi(z - 1) + (1 - t)pj + t
Curr[j,i] - Curr[i,j] = (1 - t)(pj - pi) < 0 pj < pi
t = 0;          1 - t = 0
t = p0          1 - t = 1 - po
t = p0(1-p1)+p1 1-  t = 1 - p0(1-p1) - p1 = (1-p0)(1-p1)
1 - p2(1-p0)(1-p1) - p1 = (1-p0)...(1-p2)
*/ 

struct Activity
{
    int A;
    int B;
	int C;
};

#define N_MAX 10005
#define F_MAX 1000005

int N, K, F;
struct Activity Activities[N_MAX];
double SortedActivities[N_MAX];
double Prefixes[N_MAX][3][3];
double Suffixes[N_MAX][3][3];

/*
   { { P    , 0    , 0 },
     { 1 - P, 1 - P, 0 },
     { 0    , P    , 0 } };
*/
void CreateScholasticMatrix(double Probability, double Matrix[3][3])
{
	double P = Probability;

	Matrix[0][0] = P;
    Matrix[0][1] = 0;
    Matrix[0][2] = 0;

    Matrix[1][0] = 1 - P;
    Matrix[1][1] = 1 - P;
    Matrix[1][2] = 0;

    Matrix[2][0] = 0;
    Matrix[2][1] = P;
    Matrix[2][2] = 0;
}

/* Here the muptiplication order is important 
 * Result = Left * Right != Right * Left
 */
void SquareMatrixMultix(
	double Left[3][3], 
	double Right[3][3], 
	double Result[3][3])
{
	double c;
    int i, j, t;

    for (i = 0; i < 3; i++)
	{
		for (j = 0; j < 3; j++)
		{
			c = 0;
			for (t = 0; t < 3; t++)
			{
               c += Left[i][t] * Right[t][j];    
			}
			Result[i][j] = c;
		}
	}
}

/* Probs - is a sorted array of probabilities in decrease order  
 * For each probability we have a sholastic matrix Si. Prefixes matrix
 * multiplications looks like:
 * Prefix0 = E1
 * Prefix1 = S1
 * Prefix2 = Prefix1*S2 = S1*S2
 * Prefix3 = Prefix2*S3 = S1*S2*S3
 * .....
 * Prefixn = Prefixn-1*Sn = S1*S2*...*Sn-1*Sn
 * Prefixes count will be n
 */
void ComputePrefixes(double Probs[], int n)
{
	int i = 0;
	double prefixMatrix[3][3];

	CreateScholasticMatrix(Probs[i], prefixMatrix);
    memcpy(Prefixes[i++], prefixMatrix, 9 * sizeof(double));

    while (i < n)
	{
		CreateScholasticMatrix(Probs[i], prefixMatrix);

        SquareMatrixMultix(
		    Prefixes[i - 1], 
            prefixMatrix,
            Prefixes[i]);

		i++;
	}
}
/* There is one important property we need to prove for suffixes computation.
 * Our matrixes multiplication must have a commutative property. 
 * 1, 2, 3,..., k, k + 1, .... n 
 * Ak*Ak+1*....*An = An*An-1*...*Ak+1Ak. It depends on the matrixes structure.
 * | P1     0      0 |   | P2     0      0 |   
 * | 1 - P1 1 - P1 0 | * | 1 - P2 1 - P2 0 |
 * | 0      P1     1 |   | 0      P2     1 |
 * 
 * | p1p2   0    0 |  We have received a contradiction. 
 * | (1-p1) . |
 * But we can use associativity property to provide a fast algorythm
 * A(BC)=(AB)C so we need to compute firstly BC and then multiple it with A
 */
void ComputeSuffixes(double Probs[], int n)
{
    int i = 0;
	double suffixMatrix[3][3];

    CreateScholasticMatrix(Probs[n - 1 - i], suffixMatrix);
    memcpy(Suffixes[i++], suffixMatrix, 9 * sizeof(double));

	while (i < n)
	{
		CreateScholasticMatrix(Probs[n - 1 - i], suffixMatrix);

		SquareMatrixMultix(
			suffixMatrix,
            Suffixes[i - 1], 
            Suffixes[i]);

		i++;
	}   
}

void Pr2Swap(struct Activity Activities[], int i, int j)
{
    struct Activity temp;
	temp = Activities[i];
    Activities[i] = Activities[j];
    Activities[j] = temp;
}

void Pr2SwapDouble(double Arr[], int i, int j)
{
    double temp;
	temp = Arr[i];
	Arr[i] = Arr[j];
	Arr[j] = temp;
}

/*           [5, 6, 9, 1, 2, 6, 0, -1, 10, 12]
	 *       [5, 1, 9, 6, 2, 6, 0, -1, 10, 12]
     *       [5, 1, 2, 6, 9, 6, 0, -1, 10, 12]
	 *       [5, 1, 2, 0, 9, 6, 6, -1, 10, 12]
	 *       [5, 1, 2, 0, -1, 6, 6, 9, 10, 12] */ 
void HeapSort(struct Activity Activities[], int start, int end)
{
	int medium, i, j;

	if (start > end)
	{
		return;
	}

	medium = (start + end) / 2;
	Pr2Swap(Activities, start, medium);

    i = start + 1;
    j = start + 1;

	for (; i <= end; i++)
	{
       if (Activities[i].A * Activities[medium].B < 
		   Activities[medium].A * Activities[i].B)
	   {
		   Pr2Swap(Activities, i, j++);
	   }    
	}
  
    Pr2Swap(Activities, start, j - 1);
    HeapSort(Activities, start, medium - 1);
    HeapSort(Activities, medium + 1, end);
}

void Reverse(double Arr[], int N)
{
    int i = 0;
	int j = N - 1;

	while (i < j)
	{
		Pr2SwapDouble(Arr, i++, j--);
	}
}

void SolveCase(int test_case)
{
	int i, j;
	double TotalScholastic[3][3];
	double maxProbability;

    /*
	scanf("%d %d", &N, &K);

    for (i = 0; i < N; i++)
	{
        scanf("%d/%d %d", 
		      &Activities[i].A, 
			  &Activities[i].B, 
			  &Activities[i].C);
	}
	*/
    
	N = 3;
	K = 2;

	Activities[0].A = 1;
	Activities[0].B = 2;
	Activities[0].C = 2;

	Activities[1].A = 1;
	Activities[1].B = 3;
	Activities[1].C = 2;

	Activities[2].A = 3;
	Activities[2].B = 4;
	Activities[2].C = 2;

    HeapSort(Activities, 0, N - 1);
	
	F = 0;

    for (i = 0; i < N; i++)
	{
        for (j = 0; j < Activities[i].C; j++)
		{
            SortedActivities[F++] = (double) Activities[i].A / Activities[i].B;
		}
	}

	Reverse(SortedActivities, F);

    printf("%d\n", F);
    for (i = 0; i < F; i++)
	{
        printf("%.2lf\n", SortedActivities[i]);
	}

    ComputePrefixes(SortedActivities, F);
    ComputeSuffixes(SortedActivities, F);

	maxProbability = N_MAX;
    /* When i == K, that means that we take all K matrixes 
	 * from Prefixes. In this case we do not need to multiple,
	 * but just take the (K - 1)th prefix
	 */
    if (Prefixes[K - 1][2][0] < maxProbability)
	{
        maxProbability = Prefixes[K - 1][2][0];
	}
    /* When i == 0, that means that we take all K matrixes 
	 * from Suffixes. In this case we do not need to multiple,
	 * but just take the (K - 1)th suffix
	 */
    if (Suffixes[K - 1][2][0] < maxProbability)
	{
        maxProbability = Suffixes[K - 1][2][0];
	}

    for (i = 0; i <= K - 1; i++)
	{
		SquareMatrixMultix(
	       Prefixes[i], 
	       Suffixes[i], 
	       TotalScholastic);

		if (TotalScholastic[2][0] < N_MAX) 
		{
           maxProbability = TotalScholastic[2][0];
		}
	}
}

void SolveAll()
{
	int T; 
    int tc;

	//scanf("%d", &T);

    //for (tc = 1; tc <= T; tc++)
	//{
        SolveCase(0);
	//}

    return 0;
}